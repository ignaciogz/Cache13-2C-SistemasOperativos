// MANEJO DEL ARCHIVO DE SWAP Y DE LAS LISTAS PARA LOS ESPACIOS OCUPADOS Y VACIOS

#ifndef MANEJOSWAP_H_
#define MANEJOSWAP_H_

#include <commons/collections/list.h>
#include "Paginacion.h"


/**********
ESTRUCTURAS
**********/

typedef struct{
	int pid;
	int comienzoDelHueco;
	int cantidadDePaginas;
	int paginasLeidas;
	int paginasEscritas;
}t_mProc; // tipo de dato de los elementos de la lista de espacios ocupados

typedef struct{
	int comienzoDelHueco;
	int cantidadDePaginas;
}t_espacioLibre; // tipo de dato de los elementos de la lista de espacios vacios


typedef struct{
	int pid;
	int comienzo;
}t_elementoAuxiliar;

/***************************************************
MANEJO DE PAGINAS PARA EL ESPACIO OCUPADO Y EL VACIO
****************************************************/

//Dados el pid, el comienzo en la particion y la cantidad de paginas q ocupa de un mProc, lo agrega a la lista que representa el espacio ocupado
void agregarmProc(t_list* espacioOcupado,t_mProc* mProcNuevo,int comienzo);

//Dados el comienzo del hueco libre y la cantidad de paginas que representa, agrega el espacio libre a la lista que representa el espacio libre
void agregarEspacioLibre(t_list* espacioLibre,t_espacioLibre* espacioNuevo,int comienzo,int paginas);

//Retorna la posicion de un espacio de memoria ocupado por un proceso con el pid pasado por parametro o -1 en caso que no este en la lista
int buscarEspacio(t_list* espacios,int unNumero,bool(*funcionDeComparacion)(void*,int));

//Devuelve un dato de tipo bool indicando si el pid del parametros de tipo t_mProc es igual al numero pasado por parametros
bool tienePidIgual(void* mProc,int pid);

//Devuelve un bool indicando si el byte inicial pasado por parametro se corresponde con el espacio libre
bool tieneByteInicialIgual(void* elemento,int byteInicial);

//Dado un espacio libre, retorna la cantidad de paginas que representa
int espacioHueco(t_espacioLibre* espacioLibre);

// Dado un espacio y la cantidad de paginas, devuelve si un espacio representa esa o mas cantidad de paginas
bool tieneEspacioHueco(void* elemento,int cantidadDePaginas);

//Dado una lista de espacios libres y el tamanio por pagina, retorna la cantidad de espacio libre que hay (en paginas)
int cantidadDeEspacioLibre(t_list* espaciosLibres,int tamanioPagina);

/*Dadas la lista de espacios libres, un mProc al que se le reservo memoria, el hueco en el q se reservo memoria para el mProc y el tamanio por pagina,
elimina el hueco de la lista de espacios libres y crea uno nuevo cuyo comienzo sera el byte siguiente a donde termina el mProc agregado y su cantidad de paginas sera la cantidad de paginas que hay libres luego de reservar la memoria */
 void actualizarEspacioLibre(t_list* espaciosLibres,t_mProc* mProcAgregado,t_espacioLibre* hueco, int tamanioPagina,int espacioDisponible);

 /*dados el pid de un mProc, las listas con los espacios vacios y los ocupados y el tamanio por pagina, borra el mProc de la lista de espacios ocupados
 y lo agrega a la lista de espacios vacios. Ademas escribe esta situacion en el logger*/
 void liberarmProc(int pid,t_list* espaciosVacios,t_list* espaciosOcupados,int tamanioPagina);

// combina huecos que son contiguos cuando se libera un mProc y se genera un nuevo espacio libre
void combinarContiguos(t_list* espaciosVacios,t_espacioLibre* espacioLiberado,int tamanioPagina);

//actualiza la lista de espacios ocupados sumandole los bytes correspondientes a los elementos cuyo byte inicial sea menor a inicio
void actualizarEspacioOcupado(t_list* espaciosOcupados,int bytesASumar,int inicio);

// dados dos espacios vacios devuelve un booleano indicando si el primero tiene byte inicial menor que el segundo
bool tieneMenorInicio(void* elemento1,void* elemento2);

// dado un puntero a mProc devuelve su byte inicial
int obtenerComienzo(t_mProc* mProc);

// dadas la lista de espacios libres y el tamanio por pagina, agrupa el espacio libre en uno solo y devuelve el byte final del nuevo hueco
void agruparEspacioLibre(t_list* espaciosLibres,int tamanioPagina);

// dado un puntero a mProc, devuelve su PID
int obtenerPID(t_mProc* mProc);

// dado un puntero a espacio devuelve su byte inicial
int obtenerByteInicial(t_espacioLibre * espacio);

//dado un t_elementoAuxiliar, retorna el byte donde empieza
int comienzoAuxiliar(t_elementoAuxiliar* auxiliar);

// dado un puntero a mProc y el tamanio de pagina devuelve su byte final
long int finmProc(t_mProc* mProc,int tamanioPagina);

//dado un puntero a un espacio libre y el tamaño por pagina devuelve su byte final
long int finHueco(t_espacioLibre* hueco,int tamanioPagina);

// agrega un elemento a la lista auxiliar si su byte inicial es menor a "inicio"
void agregarElementoAuxiliar(void* mProc,t_list* auxiliar,int inicio);


/*************************
MANEJO DEL ARCHIVO DE SWAP
*************************/

//Dados el nombre del archivo de swap, su ubicacion y el tamanio y la cantidad de las paginas, crea un archivo en dicha ubicacion con el nombre q se le paso y de un tamanio igual a cantidad de paginas * tamanio de pagina
void crearArchivoSwapping(char* ubicacionArchivos,char* nombreArchivo, int tamanio, int paginas);

// abre el archivo de swap comprobando errores en el modo indicado. En caso de error termina el proceso
FILE * abrirArchivo(char* ubicacion,char* modo);

// posiciona el puntero al archivo en el desplazamiento indicado. En caso de error termina el proceso
void posicionar(FILE* archivo,long desplazamiento);

// lee la cantidad de elementos de tamanio "tamanioElemento" del archivo y retorna los datos leidos
char* leer(size_t tamanioElemento,size_t cantidadElementos,FILE* archivo);

// escribe la los datos pasados por parametro en el archivo
void escribir(char* datos,size_t tamanioElemento,size_t cantidadElementos,FILE* archivo);

//dados un numero de pagina, la ubicacion del archivo de swap y el tamanio por pagina devuelve el contenido de dicha pagina
char* consultarPagina(t_pagina pagina,char* ubicacionArchivoSwap, int tamanioPagina);

// dados un numero de pagina, la unbicacion del archivo de swap, el tamanio de pagina y los datos a escribir, escribe dichos datos en la pagina
void escribirEnPagina(t_pagina paginaNueva,char* ubicacionArchivoSwap,int tamanioPagina);

// dados una pagina, la ubicaicon del archivo de swap y el tamanio por pagina, rellena con '/0' el contenido de la pagina
void borrarContenidoPagina(t_pagina paginaNueva,char* ubicacionArchivoSwap, int tamanioPagina);

//compacta el archivo de swap dejando el espacio libre desde el byte 0 en adelante
void compactar(t_list* espaciosLibres,t_list* espaciosOcupados,char* ubicacionArchivoSwap,int tamanioPagina);

//actualiza el archivo de swap moviendo los espacios ocupados que figuran en la lista mProcsAMover a su nueva ubicacion
void actualizarArchivo(t_list* mProcsAMover,t_list* espaciosOcupados,int tamanioPagina,char* ubicacionArchivoSwap);

//dadas la ubicacion del archivo de swap,un inicio de pagina y su inicio nuevo, escribe los datos de la pagina en su nueva ubicacion
void actualizarPagina(char*ubicacionArchivoSwap,int inicioOriginal,int inicioNuevo,int tamanioPagina);

// mueve un mProc a su nueva ubicacion en el archivo de swap
void movermProc(t_mProc* mProc,int inicioOriginal,int tamanioPagina,char* ubicacionArchivoSwap);
#endif /* MANEJOSWAP_H_ */
