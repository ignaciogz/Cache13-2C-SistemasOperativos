#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <commons/string.h>
#include <commons/collections/dictionary.h>

#include "estructuras.h"
#include "algoritmos.h"

//Variables de configuracion:
	extern double Retardo;
	extern int Cant_Marcos;
	extern int Tamanio_Marco;
	extern int Entradas_TLB;

//Estructuras IMPORTANTES del proceso:
	extern	t_structMP structMemoriaPrincipal;
	extern	t_structTLB structTLB;
	extern	t_dictionary* dicInfoProcesos;

//Mutex para las Estructuras IMPORTANTES del proceso
	extern pthread_mutex_t mutexStructMP;
	extern pthread_mutex_t mutexStructTLB;
	extern pthread_mutex_t mutexDicInfoProcesos;

#define COLUMNAS_TABLA_PAGINAS 4
#define COLUMNAS_TLB 3


/* --------------------  Memoria Principal  -------------------------- */
char ** inicializarMemoriaPrincipal()
{
	int i;
	char** structMP;
	structMP = malloc(sizeof(char*) * Cant_Marcos);
	
		//Creo los MARCOS
		for (i = 0; i < Cant_Marcos; i++)
		{
			structMP[i] = reservarMemoriaParaMarco();
		}
	
	return structMP;
}
	
	/* --------------------------  FAUX para inicializar  -------------------------------- */
		char* reservarMemoriaParaMarco()
		{
			char* tmp = malloc(sizeof(char) * (Tamanio_Marco + 1) );

			int tamanioDeTextoPorDefecto = strlen("Vacio :(");
				memcpy(tmp, "Vacio :(", tamanioDeTextoPorDefecto);
				tmp[tamanioDeTextoPorDefecto] = '\0';
			return tmp;
		}
	
	/* --------------------  FAUX para los algoritmos de reemplazo  -------------------------- */
		void cargarMarco_MemoriaPrincipal(int numeroMarco, char* contenido)
		{
			//ACCESO A MEMORIA PRINCIPAL, PARA OBTENER EL CONTENIDO DE UN MARCO ESPECIFICO
		  pthread_mutex_lock(&mutexStructMP);
			usleep(Retardo*1000000);
				memcpy(structMemoriaPrincipal.sMemoria[numeroMarco], contenido, strlen(contenido));
				structMemoriaPrincipal.sMemoria[numeroMarco][strlen(contenido)] = '\0';
		  pthread_mutex_unlock(&mutexStructMP);
		  return;
		}

		char* obtenerContenido_MemoriaPrincipal(int numeroMarco)
		{
			//ACCESO A MEMORIA PRINCIPAL, PARA OBTENER EL CONTENIDO DE UN MARCO ESPECIFICO
		  pthread_mutex_lock(&mutexStructMP);
			usleep(Retardo*1000000);
				char* contenido = structMemoriaPrincipal.sMemoria[numeroMarco];
		  pthread_mutex_unlock(&mutexStructMP);
			return contenido;
		}

	/* --------------------------  FAUX para los procesos  -------------------------------- */
		void asignarMarcos_MemoriaPrincipal(t_list* marcosAsignados)
		{
			asignarUnMarco_MemoriaPrincipal(marcosAsignados);
			return;
		}

		
		int asignarUnMarco_MemoriaPrincipal(t_list* marcosAsignados)
		{
			int* marcoAsignado = malloc(sizeof(int));
			*marcoAsignado = algoritmo_FirstFit_MEMORIA();

			if(*marcoAsignado != -1)
			{
				indicarMarcoOcupado_MemoriaPrincipal(*marcoAsignado);

				list_add(marcosAsignados, marcoAsignado);
			}
			
			return *marcoAsignado;
		}


		void eliminarMarcos_MemoriaPrincipal(t_list* marcosAsignados)
		{
			list_iterate(marcosAsignados, resetearMarco);
			list_clean(marcosAsignados);
			list_destroy(marcosAsignados);
			return;
		}

			void resetearMarco(void * numeroMarco)
			{
				int * numMarco = (int*)numeroMarco;
				//cargarMarco_MemoriaPrincipal(*numMarco, "Vacio :(");

				indicarMarcoLiberado_MemoriaPrincipal(*numMarco);
				return;
			}





/* --------------------  Gestion de Memoria Principal LIBRE   -------------------------- */
int* inicializarMemoriaLibre()
{
	int* structLibre;
	/*
		Array de marcos libres:
			Posicion = 0 -> LIBRE  |  Posicion = 1 -> OCUPADA
	*/
	structLibre = calloc(Cant_Marcos, sizeof(int));

	return structLibre;
}

int inicializarCantMemoriaLibre()
{
	int cantMemoriaLibre = Cant_Marcos;
	return cantMemoriaLibre;
}


	/* --------------------------  FAUX para Memoria Principal Libre  -------------------------------- */
		void indicarMarcoOcupado_MemoriaPrincipal(int numeroMarco)
		{
		  pthread_mutex_lock(&mutexStructMP);
			structMemoriaPrincipal.sMemoriaLibre[numeroMarco] = 1;
			(structMemoriaPrincipal.sCantMemoriaLibre)--;
		  pthread_mutex_unlock(&mutexStructMP);
			return;
		}
		
		void indicarMarcoLiberado_MemoriaPrincipal(int numeroMarco)
		{
		  pthread_mutex_lock(&mutexStructMP);
			structMemoriaPrincipal.sMemoriaLibre[numeroMarco] = 0;
			(structMemoriaPrincipal.sCantMemoriaLibre)++;
		  pthread_mutex_unlock(&mutexStructMP);
			return;
		}

		int hayEspacioParaSolicitudDeMarcos(int cantidadDeMarcosRequeridos)
		{
			int hayEspacio = 0;
			
		  pthread_mutex_lock(&mutexStructMP);
			if ( (structMemoriaPrincipal.sCantMemoriaLibre) >= cantidadDeMarcosRequeridos)
			{
				hayEspacio = 1;
			}
		  pthread_mutex_unlock(&mutexStructMP);
			return hayEspacio;
		}





/* --------------------  TLB  -------------------------- */
int ** inicializarTLB()
{
	int ColNumPagina = 0;
	int entrada;
	/*	
		Columnas de la tabla:
			Numero_Pagina | PID | Numero_Marco
	*/
	int** TLB = crearTabla(Entradas_TLB, COLUMNAS_TLB);
		//Indico que la entrada de la TLB esta vacia:
		for (entrada = 0; entrada < Entradas_TLB; entrada++)
		{
			TLB[entrada][ColNumPagina] = -1;
		}

	return TLB;
}


	int escribirEntrada_TLB(int numeroPagina, int PID, int numeroMarco)
	{
		//Busca un espacio libre, o busca victima y escribe.
		int* entrada = malloc(sizeof(int));
		*entrada = algoritmo_FirstFit_TLB();

		if(*entrada == -1)
		{
			*entrada = algoritmo_SeleccionVictima_TLB();
		}

	  pthread_mutex_lock(&mutexStructTLB);
		structTLB.sCacheTLB[*entrada][0] = numeroPagina;
		structTLB.sCacheTLB[*entrada][1] = PID;
		structTLB.sCacheTLB[*entrada][2] = numeroMarco;

		queue_push(structTLB.sOrdenDeIngresoTLB, entrada);
	  pthread_mutex_unlock(&mutexStructTLB);
		
		return *entrada;
	}


	int paginaCargada_TLB(int numeroPagina, int PID)
	{
		int entrada;
		int paginaCargada = 0;

		pthread_mutex_lock(&mutexStructTLB);
		for (entrada = 0; entrada < Entradas_TLB; entrada++)
		{
			if( (structTLB.sCacheTLB[entrada][0] == numeroPagina) && (structTLB.sCacheTLB[entrada][1] == PID) )
			{
				paginaCargada = 1;
				break;
			}
		}
		pthread_mutex_unlock(&mutexStructTLB);
		
		return paginaCargada;
	}


	int obtenerMarcoDePagina_TLB(int numeroPagina, int PID)
	{
	  pthread_mutex_lock(&mutexStructTLB);
		int* entrada = malloc(sizeof(int));
		*entrada = entradaDePagina_TLB(numeroPagina, PID);
		
		int numeroMarco = structTLB.sCacheTLB[*entrada][2];
	  pthread_mutex_unlock(&mutexStructTLB);

		free (entrada);
		return numeroMarco;
	}


	int entradaDePagina_TLB(int numeroPagina, int PID)
	{
		//El mutex para esta lo tome en la funcion de arriba :D
		int entrada;

		for (entrada = 0; entrada < Entradas_TLB; entrada++)
		{
			if( (structTLB.sCacheTLB[entrada][0] == numeroPagina) && (structTLB.sCacheTLB[entrada][1] == PID) )
				return entrada;
		}
		return -1;
	}


	void quitarPagina_TLB(int PID, int numeroDePagina)
	{
		int ColNPag = 0;
		int ColPID = 1;
		int entrada;

			
		  pthread_mutex_lock(&mutexStructTLB);
			for (entrada = 0; entrada < Entradas_TLB; entrada++)
			{
				if(structTLB.sCacheTLB[entrada][ColNPag] == numeroDePagina && structTLB.sCacheTLB[entrada][ColPID] == PID)
				{
					structTLB.sCacheTLB[entrada][0] = -1;
					structTLB.sCacheTLB[entrada][1] = 0;
					structTLB.sCacheTLB[entrada][2] = 0;

					actualizarOrdenDeIngreso_TLB(entrada);
					break;
				}
			}
		  pthread_mutex_unlock(&mutexStructTLB);
		return;
	}


	void eliminarDatosDelProceso_TLB(int PID)
	{
		int ColPID = 1;
		int entrada;

			//Vaciando la TLB...
		  pthread_mutex_lock(&mutexStructTLB);
			for (entrada = 0; entrada < Entradas_TLB; entrada++)
			{
				if(structTLB.sCacheTLB[entrada][ColPID] == PID)
				{
					structTLB.sCacheTLB[entrada][0] = -1;
					structTLB.sCacheTLB[entrada][1] = 0;
					structTLB.sCacheTLB[entrada][2] = 0;

					actualizarOrdenDeIngreso_TLB(entrada);
				}
			}
		  pthread_mutex_unlock(&mutexStructTLB);
			return;
	}


	void actualizarOrdenDeIngreso_TLB(int entrada)
	{
		int i = 0;
		int cantElementos = queue_size(structTLB.sOrdenDeIngresoTLB);
		t_queue* colaAuxiliar = queue_create();

		//Copio en una cola auxiliar, para respetar el orden de ingreso a la TLB
		while(i < cantElementos)
		{
			int* elementoDeLaCola = queue_pop(structTLB.sOrdenDeIngresoTLB);
				int* elementoCopia = malloc(sizeof(int));
				*elementoCopia = *elementoDeLaCola;
				queue_push(colaAuxiliar, elementoCopia);
			queue_push(structTLB.sOrdenDeIngresoTLB, elementoDeLaCola);

			i++;
		}

		
		i = 0;
		//Busco la entrada a eliminar y la elimino
		while(i < cantElementos)
		{	
			int* elementoDeLaCola = queue_pop(structTLB.sOrdenDeIngresoTLB);
			if(*elementoDeLaCola == entrada)
			{
				free(elementoDeLaCola);
				break;
			}
			else
			{
				queue_push(structTLB.sOrdenDeIngresoTLB, elementoDeLaCola);
			}

			i++;
		}


		i = 0;
		t_queue* actualizadoOrdenDeIngresoTLB = queue_create();
		//Comparo las colas y agrego solo los elementos que estan en ambas
		while(i < cantElementos)
		{	
			int* elementoPosible = queue_pop(colaAuxiliar);
			
			if(agregarALaCola(*elementoPosible, structTLB.sOrdenDeIngresoTLB))
			{
				queue_push(actualizadoOrdenDeIngresoTLB, elementoPosible);
			}else
			{
				free(elementoPosible);
			}

			i++;
		}

		queue_destroy(colaAuxiliar);
			queue_clean(structTLB.sOrdenDeIngresoTLB);
			queue_destroy(structTLB.sOrdenDeIngresoTLB);
		structTLB.sOrdenDeIngresoTLB = actualizadoOrdenDeIngresoTLB;
		return;
	}

	
	void vaciar_TLB()
	{
		int ColNumPagina = 0;
		int entrada, columna;

		//Vaciando la TLB...
	  pthread_mutex_lock(&mutexStructTLB);
		for (entrada = 0; entrada < Entradas_TLB; entrada++)
		{
			for (columna = 0; columna < COLUMNAS_TLB; columna++)
			{
				if(columna == ColNumPagina)
					structTLB.sCacheTLB[entrada][columna] = -1;
				else
					structTLB.sCacheTLB[entrada][columna] = 0;
			}
		}
		queue_clean(structTLB.sOrdenDeIngresoTLB);
	  pthread_mutex_unlock(&mutexStructTLB);
		return;
	}





/* -----------------------------  PROCESOS  ----------------------------------- */
	//DICCIONARIO dicInfoProcesos - Clave: PID, Valor: t_proceso 
		/*struct t_proceso
			- totalDePaginas	 INT
			- tablaDePaginas     INT**
			- marcosAsignados    LIST*
			- marcos  			 QUEUE*      
			    ->   t_infoDeMarco: numeroMarco | numeroPagina | bitUso | bitModificado
			- contadorPageFault    INT Contador
			- contadorAccesos       INT Contador
		*/

void inicializarProceso(int PID, int totalDePaginas)
{
	t_proceso* ProcesoX = malloc(sizeof(t_proceso));
	
	//Le CARGO los DATOS necesarios al struct del proceso:
	(*ProcesoX).totalDePaginas = totalDePaginas;

	(*ProcesoX).tablaDePaginas = inicializarTablaDePaginas(totalDePaginas);

		t_list* marcosAsignados = list_create();
		asignarMarcos_MemoriaPrincipal(marcosAsignados);
		
	(*ProcesoX).marcosAsignados = marcosAsignados;

	(*ProcesoX).marcos = queue_create();
	
	(*ProcesoX).contadorPageFault = 0;
	(*ProcesoX).contadorAccesos = 0;


	//Registro el proceso en el diccionario de informacion:
	char* key = string_itoa(PID);
	
  pthread_mutex_lock(&mutexDicInfoProcesos);
	dictionary_put(dicInfoProcesos, key, ProcesoX);
  pthread_mutex_unlock(&mutexDicInfoProcesos);	
	
	free(key);
	return;
}


t_proceso* obtenerProceso(int PID)
{
	char* key = string_itoa(PID);
  
  pthread_mutex_lock(&mutexDicInfoProcesos);
	t_proceso* ProcesoX = (t_proceso*)dictionary_get(dicInfoProcesos, key);
  pthread_mutex_unlock(&mutexDicInfoProcesos);
	
	free(key);
	return ProcesoX;
}


int** obtenerTPProceso(int PID)
{
	char* key = string_itoa(PID);

  pthread_mutex_lock(&mutexDicInfoProcesos);
	t_proceso* ProcesoX = (t_proceso*)dictionary_get(dicInfoProcesos, key);
  pthread_mutex_unlock(&mutexDicInfoProcesos);

	free(key);
	return (*ProcesoX).tablaDePaginas;
}


t_queue* obtenerMarcosProceso(int PID)
{
	char* key = string_itoa(PID);

  pthread_mutex_lock(&mutexDicInfoProcesos);
	t_proceso* ProcesoX = (t_proceso*)dictionary_get(dicInfoProcesos, key);
  pthread_mutex_unlock(&mutexDicInfoProcesos);

	free(key);
	return (*ProcesoX).marcos;
}


void eliminarProceso(int PID)
{
	char* N_PID = string_itoa(PID);
  
  pthread_mutex_lock(&mutexDicInfoProcesos);		
		t_proceso* ProcesoX = dictionary_remove(dicInfoProcesos, N_PID);
  pthread_mutex_unlock(&mutexDicInfoProcesos);

	free(N_PID);

	//Obtengo la referencia a cada elemento del proceso:
		int totalDePaginas = (*ProcesoX).totalDePaginas;
		int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
		t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
		t_queue* marcos = (*ProcesoX).marcos;
	//Elimino cada elemento del proceso:
		eliminar_TP(tablaDePaginas, totalDePaginas);
		eliminarMarcos_MemoriaPrincipal(marcosAsignados);
		eliminarDatosDelProceso_TLB(PID);
		queue_clean(marcos);
		queue_destroy(marcos);

		int nPageFault = (*ProcesoX).contadorPageFault;
		int nAccesos = (*ProcesoX).contadorAccesos;

		printf("\tPID: %i- Cantidad de fallos: %i VS Total accedidas: %i\n", PID, nPageFault, nAccesos);
	
	free(ProcesoX);
	return;
}





/* ----------------------------  TABLA DE PAGINAS de 1 PROCESO  --------------------------------- */
int ** inicializarTablaDePaginas(int filasTablaPagina)
{
	/*	
		Columnas de la tabla:
			BitPresencia | NumeroMarco
				0			    1
	*/
	int** structTBPAGINA = crearTabla(filasTablaPagina, COLUMNAS_TABLA_PAGINAS);

	return structTBPAGINA;
}


void actualizarEntrada_TP(int** TablaPagina, int NumeroPagina, int BitPresencia, int NumeroMarco)
{
	usleep(Retardo*1000000);
	if(BitPresencia != -1)
		TablaPagina[NumeroPagina][0] = BitPresencia;
	if(NumeroMarco != -1)
		TablaPagina[NumeroPagina][1] = NumeroMarco;
	
	return;
}


int paginaPresente_TP(int** TablaPagina, int NumeroPagina)
{
	usleep(Retardo*1000000);
	if(TablaPagina[NumeroPagina][0] == 1)
		return 1;
	else
		return 0;
}


int obtenerMarcoDePagina_TP(int** TablaPagina, int NumeroPagina)
{
	usleep(Retardo*1000000);
	return TablaPagina[NumeroPagina][1];
}


void resetear_TP(int** TablaPagina, int numeroDePagina)
{
	usleep(Retardo*1000000);
	TablaPagina[numeroDePagina][0] = 0;
	return;
}


void eliminar_TP(int ** structTBPAGINA, int cantTotalPag)
{
	usleep(Retardo*1000000);
	int i;
	for(i=0; i<cantTotalPag; i++)
		free(structTBPAGINA[i]);
	free(structTBPAGINA);

	return;
}





/* ----------------------------   Funciones Auxiliares PRO   --------------------------------- */
int** crearTabla(int filas, int columnas)
{
	int i;

    int** rows = malloc(filas*sizeof(int*));
    for (i=0; i<filas; ++i)
    {
    	rows[i] = calloc(columnas, sizeof(int));
    }
    return rows;
}


int agregarALaCola(int entrada, t_queue* colita)
{
	int i = 0;
	int estaEnLaCola = 0;
	int cantElementos = queue_size(colita);
	int* elementoDeLaCola;

	while(i < cantElementos)
	{
		elementoDeLaCola = queue_pop(colita);

		if((*elementoDeLaCola) == entrada)
		{
			estaEnLaCola = 1;
		}
		queue_push(colita, elementoDeLaCola);

		i++;
	}
	return estaEnLaCola;	
}
