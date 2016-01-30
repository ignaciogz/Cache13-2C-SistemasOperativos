#include <stdio.h>
#include <stdlib.h>
#include "ManejoSwap.h"
#include <string.h>
#include <sys/mman.h> // para el mmap()
#include <unistd.h> // para close()
#include <fcntl.h> // para usar open() y close()
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/error.h>
#include "Logueos.h"
#include "InteraccionConPantalla.h"
#include <pthread.h>

pthread_mutex_t mutexEspacioLibre = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexEspacioOcupado = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexArchivo = PTHREAD_MUTEX_INITIALIZER;


void agregarmProc(t_list* espacioOcupado,t_mProc* mProcNuevo,int comienzo)
{
	pthread_mutex_lock(&mutexEspacioOcupado);
	mProcNuevo->comienzoDelHueco = comienzo;
	mProcNuevo->paginasEscritas = 0;
	mProcNuevo->paginasLeidas = 0;
	list_add(espacioOcupado,mProcNuevo);
	pthread_mutex_unlock(&mutexEspacioOcupado);
}


void agregarEspacioLibre(t_list* espacioLibre,t_espacioLibre* espacioNuevo,int comienzo,int paginas)
{
	espacioNuevo->cantidadDePaginas = paginas;
	espacioNuevo->comienzoDelHueco = comienzo;
	list_add(espacioLibre,espacioNuevo);
}


int buscarEspacio(t_list* espacios,int unNumero,bool(*funcionDeComparacion)(void*,int))
{
	t_link_element *elemento = espacios->head;
	int posicion = 0;
	while (elemento != NULL && !(*funcionDeComparacion)(elemento->data,unNumero))
	{
		elemento = elemento->next;
		posicion++;
	}
	return elemento != NULL ? posicion : -1;
}


bool tienePidIgual(void * elemento,int pid)
{
	int pidmProc = obtenerPID((t_mProc*)elemento);
	bool evaluacion = pidmProc == pid;
	return evaluacion;
}

int obtenerPID(t_mProc* mProc)
{
	return mProc->pid;
}

bool tieneByteInicialIgual(void* elemento,int byteInicial)
{
	int comienzo = obtenerByteInicial((t_espacioLibre*)elemento);
	bool evaluacion = comienzo == byteInicial;
	return evaluacion;
}

int obtenerByteInicial(t_espacioLibre * espacio)
{
	return espacio->comienzoDelHueco;
}


bool tieneEspacioHueco(void* elemento,int cantidadDePaginas)
{
	bool value = espacioHueco((t_espacioLibre*)elemento) >= cantidadDePaginas;
	return value;
}

int espacioHueco(t_espacioLibre* espacioLibre)
{
	return espacioLibre->cantidadDePaginas;
}

int cantidadDeEspacioLibre(t_list* espaciosLibres,int tamanioPagina)
{
	pthread_mutex_lock(&mutexEspacioLibre);
	int i,espacioLibre=0;
	int cantidadDeEspacios = list_size(espaciosLibres);
	for(i=0;i<cantidadDeEspacios;i++)
	{
		t_espacioLibre * espacio = list_get(espaciosLibres,i);
		espacioLibre = espacioLibre + espacio->cantidadDePaginas;
	}
	pthread_mutex_unlock(&mutexEspacioLibre);
	return espacioLibre;
}

void actualizarEspacioLibre(t_list* espaciosLibres,t_mProc* mProcAgregado,t_espacioLibre* hueco,int tamanioPagina,int espacioDisponible)
{
	pthread_mutex_lock(&mutexEspacioLibre);
	int posicion = buscarEspacio(espaciosLibres,hueco->comienzoDelHueco,&tieneByteInicialIgual);
	t_espacioLibre * espacioPorAgregar = malloc(sizeof(t_espacioLibre));
	espacioPorAgregar = list_remove(espaciosLibres,posicion);
	if(mProcAgregado->cantidadDePaginas < espacioPorAgregar->cantidadDePaginas)
	{ //Si el mProc no ocupa el hueco entero creo un nuevo hueco con lo que quedo sin usar. En caso contrario su tamanio es igual que el del hueco
		int comienzo = mProcAgregado->comienzoDelHueco + (mProcAgregado->cantidadDePaginas * tamanioPagina);
		int paginas =  espacioPorAgregar->cantidadDePaginas - mProcAgregado->cantidadDePaginas;
		agregarEspacioLibre(espaciosLibres,espacioPorAgregar,comienzo,paginas);
	}
	pthread_mutex_unlock(&mutexEspacioLibre);
}

void liberarmProc(int pid,t_list* espaciosVacios,t_list* espaciosOcupados,int tamanioPagina)
{
	t_mProc * mProc = malloc(sizeof(t_mProc));
	t_espacioLibre * hueco = malloc(sizeof(t_espacioLibre));
	pthread_mutex_lock(&mutexEspacioOcupado);
	int posicion = buscarEspacio(espaciosOcupados,pid,&tienePidIgual);
	mProc = list_remove(espaciosOcupados,posicion);
	pthread_mutex_unlock(&mutexEspacioOcupado);
	pthread_mutex_lock(&mutexEspacioLibre);
	hueco->comienzoDelHueco = mProc->comienzoDelHueco;
	hueco->cantidadDePaginas = mProc->cantidadDePaginas;
	list_add(espaciosVacios,hueco);
	combinarContiguos(espaciosVacios,hueco,tamanioPagina);
	pthread_mutex_unlock(&mutexEspacioLibre);
	int cantidadDeBytesLiberados = mProc->cantidadDePaginas * tamanioPagina;
	loguearmProcLiberado(mProc->pid,mProc->comienzoDelHueco,cantidadDeBytesLiberados,mProc->paginasEscritas,mProc->paginasLeidas);
	mostrarFinDemProc(mProc->pid,mProc->paginasEscritas,mProc->paginasLeidas);
	free(mProc);
}


void combinarContiguos(t_list* espaciosVacios,t_espacioLibre* espacioLiberado,int tamanioPagina)
{
	list_sort(espaciosVacios,&tieneMenorInicio);
	int i = 0;
	while(i < list_size(espaciosVacios))
	{
		t_espacioLibre* hueco = list_get(espaciosVacios,i);
		t_espacioLibre* huecoSiguiente= list_get(espaciosVacios,i+1);
		if(huecoSiguiente == NULL) // si pasa esto estoy en el ultimo elemento de la lista
		{
			break;
		}
		int finPrimero = finHueco(hueco,tamanioPagina);
		int comienzoSiguiente = obtenerByteInicial(huecoSiguiente);
		if(finPrimero == comienzoSiguiente-1)
		{
			huecoSiguiente = list_remove(espaciosVacios,i+1);
			hueco->cantidadDePaginas = hueco->cantidadDePaginas + huecoSiguiente->cantidadDePaginas;
			i++;
		}
		else
		{
			i++;
		}
	}
}


void agruparEspacioLibre(t_list* espaciosLibres,int tamanioPagina)
{
	int i = 1;
	list_sort(espaciosLibres,&tieneMenorInicio);
	t_espacioLibre * primero = list_get(espaciosLibres,0);
	primero->comienzoDelHueco = 0;
	while(i < list_size(espaciosLibres))
	{
		t_espacioLibre * hueco = list_remove(espaciosLibres,i);
		primero->cantidadDePaginas = primero->cantidadDePaginas + hueco->cantidadDePaginas;
		i++;
	}
}

long int finmProc(t_mProc* mProc,int tamanioPagina)
{
	long int fin = mProc->comienzoDelHueco + mProc->cantidadDePaginas -1;
	return fin;
}

long int finHueco(t_espacioLibre* hueco,int tamanioPagina)
{
	long int fin = hueco->comienzoDelHueco + (hueco->cantidadDePaginas * tamanioPagina) -1 ;
	return fin;
}

bool tieneMenorInicio(void* elemento1,void* elemento2)
{
	int comienzoPrimero = obtenerByteInicial((t_espacioLibre*)elemento1);
	int comienzoSegundo = obtenerByteInicial((t_espacioLibre*)elemento2);
	bool evaluacion = comienzoPrimero < comienzoSegundo;
	return evaluacion;
}

bool tieneMayorInicio(void* elemento1,void* elemento2)
{
	int comienzoPrimero = obtenerByteInicial((t_espacioLibre*)elemento1);
	int comienzoSegundo = obtenerByteInicial((t_espacioLibre*)elemento2);
	bool evaluacion = comienzoPrimero > comienzoSegundo;
	return evaluacion;
}


int obtenerComienzo(t_mProc* mProc)
{
	return mProc->comienzoDelHueco;
}

void armarListaAuxiliar(t_list* espaciosOcupados,t_list* auxiliar,int inicio)
{
	t_link_element *elemento = espaciosOcupados->head;
	while (elemento != NULL )
	{
		agregarElementoAuxiliar(elemento->data,auxiliar,inicio);
		elemento = elemento->next;
	}
}


void agregarElementoAuxiliar(void* mProc,t_list* auxiliar,int inicio)
{
	int comienzo = obtenerComienzo((t_mProc*)mProc);
	if(comienzo < inicio)
	{
		t_elementoAuxiliar* elemento = malloc(sizeof(t_elementoAuxiliar));
		elemento->pid = obtenerPID((t_mProc*)mProc);
		elemento->comienzo = comienzo;
		list_add(auxiliar,elemento);
	}
}

bool tieneComienzoMasGrande(void* elemento1,void* elemento2)
{
	int comienzo1 = comienzoAuxiliar((t_elementoAuxiliar*)elemento1);
	int comienzo2 = comienzoAuxiliar((t_elementoAuxiliar*)elemento2);
	bool value = comienzo1 > comienzo2;
	return value;
}

int comienzoAuxiliar(t_elementoAuxiliar* auxiliar)
{
	return auxiliar->comienzo;
}

void actualizarEspacioOcupado(t_list* espaciosOcupados,int bytesASumar,int inicio)
{
	int i = 0;
	while(i < list_size(espaciosOcupados))
	{
		t_mProc * mProc = list_get(espaciosOcupados,i);
		if(mProc->comienzoDelHueco <= inicio)
		{
			mProc->comienzoDelHueco = mProc->comienzoDelHueco + bytesASumar;
			i++;
		}
		else
		{
			i++;
		}
	}
}


void compactar(t_list* espaciosLibres,t_list* espaciosOcupados,char* ubicacionArchivoSwap,int tamanioPagina)
{
	pthread_mutex_lock(&mutexEspacioOcupado);
	pthread_mutex_lock(&mutexEspacioLibre);
	pthread_mutex_lock(&mutexArchivo);
	list_sort(espaciosLibres,&tieneMayorInicio);
	t_espacioLibre* primero = list_get(espaciosLibres,0);
	t_list* auxiliar = list_create();
	armarListaAuxiliar(espaciosOcupados,auxiliar,primero->comienzoDelHueco);
	t_link_element *elemento = espaciosLibres->head;
	while (elemento != NULL)
	{
		int bytesASumar = espacioHueco((t_espacioLibre*)elemento->data) * tamanioPagina; // -> crear esta funcion
		int inicio = obtenerByteInicial((t_espacioLibre*)elemento->data);
		actualizarEspacioOcupado(espaciosOcupados,bytesASumar,inicio);
		elemento = elemento->next;
	}
	agruparEspacioLibre(espaciosLibres,tamanioPagina);
	list_sort(auxiliar,&tieneComienzoMasGrande);
	actualizarArchivo(auxiliar,espaciosOcupados,tamanioPagina,ubicacionArchivoSwap);
	list_clean(auxiliar);
	list_destroy(auxiliar);
	pthread_mutex_unlock(&mutexEspacioOcupado);
	pthread_mutex_unlock(&mutexEspacioLibre);
	pthread_mutex_unlock(&mutexArchivo);
}



void crearArchivoSwapping(char* ubicacionArchivos,char* nombreArchivo, int tamanio, int paginas)
{ //la onda es lograr que en la terminal se ejecute un codigo de este estilo:
 // cd /home/utnso/tp-2015-2c-404-name-not-found/Administrador_de_Swap/Archivos && dd if=/dev/zero of=swap.data bs=1024 count=1048576
	char * orden = string_new();
	char * tamanioPagina = string_itoa(tamanio);
	string_append(&orden,"cd ");
	string_append(&orden,ubicacionArchivos);
	string_append(&orden," && dd if=/dev/zero of=");
	string_append(&orden,nombreArchivo);
	string_append(&orden," bs=");
	string_append(&orden,tamanioPagina);
	string_append(&orden," count=");
	char * cantidadPaginas = string_itoa(paginas);
	string_append(&orden,cantidadPaginas);
	system(orden);
}

FILE * abrirArchivo(char* ubicacion,char* modo) //-> modo = "r+" (abre para lectura o escritura)
{
    FILE *descriptor = fopen(ubicacion,modo);
    if(descriptor == NULL)
    {
      perror("fopen");
      exit(0);
    }
    return descriptor;
}

void posicionar(FILE* archivo,long desplazamiento)
{
    int resultado = fseek(archivo,desplazamiento,SEEK_SET);
    if(resultado == -1)
    {
      perror("fseek");
      exit(0);
    }
}

char* leer(size_t tamanioElemento,size_t cantidadElementos,FILE* archivo) //cantidad de Elementos parece ser 1 siempre en nuestro caso
{
    int tamanio = tamanioElemento * cantidadElementos;
	char*buffer = malloc(tamanio + 1);
	memset(buffer, '\0', tamanio + 1);
	size_t elementosLeidos = fread(buffer,tamanioElemento,cantidadElementos,archivo);
    if(elementosLeidos != cantidadElementos)
    {
      perror("fread");
      exit(0);
    }
    return buffer;
}

void escribir(char* datos,size_t tamanioElemento,size_t cantidadElementos,FILE* archivo) // datos es un malloc(tamanioElemento * cantidadElementos)
{
    size_t elementosEscritos = fwrite(datos,tamanioElemento,cantidadElementos,archivo);
    if(elementosEscritos != cantidadElementos)
    {
      perror("fwrite");
      exit(0);
    }
}


char* consultarPagina(t_pagina pagina,char* ubicacionArchivoSwap, int tamanioPagina)
{
	pthread_mutex_lock(&mutexArchivo);
	FILE* descriptor = abrirArchivo(ubicacionArchivoSwap,"r+");
	posicionar(descriptor,pagina.inicio);
	char* datos = leer(sizeof(char),tamanioPagina,descriptor);
	fclose(descriptor);
	pthread_mutex_unlock(&mutexArchivo);
	return datos;
}

void escribirEnPagina(t_pagina paginaNueva,char* ubicacionArchivoSwap,int tamanioPagina)
{
	pthread_mutex_lock(&mutexArchivo);
	FILE* descriptor = abrirArchivo(ubicacionArchivoSwap,"r+");
	posicionar(descriptor,paginaNueva.inicio);
	char * datosDeEscritura = adaptarContenidoAPagina(paginaNueva.datos,tamanioPagina);
	escribir(datosDeEscritura,tamanioPagina,1,descriptor);
	fclose(descriptor);
	pthread_mutex_unlock(&mutexArchivo);
}

void borrarContenidoPagina(t_pagina paginaNueva,char* ubicacionArchivoSwap, int tamanioPagina)
{

	FILE* descriptor = abrirArchivo(ubicacionArchivoSwap,"r+");
	posicionar(descriptor,paginaNueva.inicio);
	char * datosDeEscritura = malloc(tamanioPagina);
	memset(datosDeEscritura,'\0',tamanioPagina);
	escribir(datosDeEscritura,tamanioPagina,1,descriptor);
	fclose(descriptor);
}

void actualizarPagina(char*ubicacionArchivoSwap,int inicioOriginal,int inicioNuevo,int tamanioPagina)
{
	FILE* descriptor = abrirArchivo(ubicacionArchivoSwap,"r+");
	posicionar(descriptor,inicioOriginal);
	char* datos = leer(tamanioPagina,1,descriptor);
	if(strlen(datos) != 0) //si es 0 esta rellenado con '\0' por lo que no me interesa escribir datos
	{
		posicionar(descriptor,inicioNuevo);
		char * datosDeEscritura = adaptarContenidoAPagina(datos,tamanioPagina);
		escribir(datosDeEscritura,tamanioPagina,1,descriptor);
	}
	fclose(descriptor);
}


void movermProc(t_mProc* mProc,int inicioOriginal,int tamanioPagina,char* ubicacionArchivoSwap)
{
	int i=0;
	int inicioPagina = inicioOriginal;
	int cantidadDePaginas = mProc->cantidadDePaginas;
	while(i < cantidadDePaginas)
	{
		inicioPagina = inicioOriginal + (i * tamanioPagina);
		int inicioNuevo  = mProc->comienzoDelHueco + (i * tamanioPagina);
		actualizarPagina(ubicacionArchivoSwap,inicioPagina,inicioNuevo,tamanioPagina);
		i++;
	}
}


void actualizarArchivo(t_list* mProcsAMover,t_list* espaciosOcupados,int tamanioPagina,char* ubicacionArchivoSwap)
{
	int i=0,cantidadElementos = list_size(mProcsAMover);
	while(i < cantidadElementos)  //primero busco los mProcs del ultimo movido hasta el primer para no pisar los datos
	{
		t_elementoAuxiliar* aux = list_get(mProcsAMover,i);
		int pos = buscarEspacio(espaciosOcupados,aux->pid,&tienePidIgual);
		t_mProc* mProc = list_get(espaciosOcupados,pos);
		movermProc(mProc,aux->comienzo,tamanioPagina,ubicacionArchivoSwap);
		i++;
	}
}
