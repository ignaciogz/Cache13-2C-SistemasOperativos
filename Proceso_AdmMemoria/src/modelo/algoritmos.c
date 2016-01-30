#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include "algoritmos.h"
#include "logeador.h"
#include "../controladores/sendController.h"

//Variables de configuracion:
	extern int Entradas_TLB;
	extern int Cant_Marcos;

	extern int Max_Marcos_Por_Proceso;
	extern char* Algoritmo_De_Memoria;
	extern char* TLB_Habilitada;

//Estructuras IMPORTANTES del proceso:
	extern	t_structMP structMemoriaPrincipal;
	extern	t_structTLB structTLB;

//Mutex para las Estructuras IMPORTANTES del proceso
	extern pthread_mutex_t mutexStructMP;
	extern pthread_mutex_t mutexStructTLB;



void algoritmo_reemplazo_pagina(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	if(strcmp(Algoritmo_De_Memoria, "FIFO") == 0)
		algoritmo_FIFO(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
	if(strcmp(Algoritmo_De_Memoria, "LRU") == 0)
		algoritmo_LRU(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
	if(strcmp(Algoritmo_De_Memoria, "CLOCKMod") == 0)
		algoritmo_CLOCKMod(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
	return;
}



/* --------------------  Algoritmos de REEMPLAZO DE PAGINAS  -------------------------- */
void algoritmo_FIFO(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;
	int cantElementos = queue_size(colitaDeMarcos);
	int marco = 0;

		t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
		int cantidadLista = list_size(marcosAsignados);

	if(cantidadLista < Max_Marcos_Por_Proceso)
	{
		if(cantElementos > 0)
			marco = asignarUnMarco_MemoriaPrincipal(marcosAsignados);
		
		if(marco != -1)
		{
			//t_queue* colaInicial = obtenerColaInicial(colitaDeMarcos);
			
			int nMarcoLibre = cantElementos;
			int* marcoLibre = (int*)list_get(marcosAsignados, nMarcoLibre);
			
			
			t_infoDeMarco* elemento = malloc(sizeof(t_infoDeMarco));
				(*elemento).NumeroPagina = numeroDePagina;
				(*elemento).NumeroMarco = *marcoLibre;
			switch(TipoOperacion)
			{
			case 'L':
				(*elemento).BitMod = 0;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			case 'E':
				(*elemento).BitMod = 1;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			}

			// LOGEO ACCESO A MEMORIA
				LOG_accesoMemoria(PID, numeroDePagina, *marcoLibre);
			((*ProcesoX).contadorAccesos)++;

			cargarMarco_MemoriaPrincipal(*marcoLibre, contenido);

			// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
				actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, *marcoLibre);
			queue_push(colitaDeMarcos, elemento);

			return;
		}
		else
		{
			algoritmo_victima_FIFO(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
		}
	}else
	{
		algoritmo_victima_FIFO(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
	}

	return;
}

	
void algoritmo_LRU(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;
	int cantElementos = queue_size(colitaDeMarcos);
	int marco = 0;

		t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
		int cantidadLista = list_size(marcosAsignados);

	if(cantidadLista < Max_Marcos_Por_Proceso)
	{
		if(cantElementos > 0)
			marco = asignarUnMarco_MemoriaPrincipal(marcosAsignados);

		if(marco != -1)
		{
			int nMarcoLibre = cantElementos;
			int* marcoLibre = (int*)list_get(marcosAsignados, nMarcoLibre);


			t_infoDeMarco* elemento = malloc(sizeof(t_infoDeMarco));
				(*elemento).NumeroPagina = numeroDePagina;
				(*elemento).NumeroMarco = *marcoLibre;
			switch(TipoOperacion)
			{
			case 'L':
				(*elemento).BitMod = 0;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			case 'E':
				(*elemento).BitMod = 1;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			}

			// LOGEO ACCESO A MEMORIA
				LOG_accesoMemoria(PID, numeroDePagina, *marcoLibre);
			((*ProcesoX).contadorAccesos)++;

			cargarMarco_MemoriaPrincipal(*marcoLibre, contenido);

			// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
				actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, *marcoLibre);
			queue_push(colitaDeMarcos, elemento);

			return;
		}
		else
		{
			algoritmo_victima_LRU(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
		}
	}else
	{
		algoritmo_victima_LRU(numeroDePagina, ProcesoX, contenido, PID, TipoOperacion);
	}

	return;
}


void algoritmo_CLOCKMod(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;
	int cantElementos = queue_size(colitaDeMarcos);
	int marco = 0;

		t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
		int cantidadLista = list_size(marcosAsignados);

	if(cantidadLista < Max_Marcos_Por_Proceso)
	{
		if(cantElementos > 0)
			marco = asignarUnMarco_MemoriaPrincipal(marcosAsignados);

		if(marco != -1)
		{
			int nMarcoLibre = cantElementos;
			int* marcoLibre = (int*)list_get(marcosAsignados, nMarcoLibre);


			t_infoDeMarco* elemento = malloc(sizeof(t_infoDeMarco));
				(*elemento).NumeroPagina = numeroDePagina;
				(*elemento).NumeroMarco = *marcoLibre;
			switch(TipoOperacion)
			{
			case 'L':
				(*elemento).BitUso = 1;
				(*elemento).BitMod = 0;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			case 'E':
				(*elemento).BitUso = 1;
				(*elemento).BitMod = 1;
				if(strcmp(TLB_Habilitada, "SI") == 0){
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, *marcoLibre);
						LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				}
				break;
			}
			// LOGEO ACCESO A MEMORIA
				LOG_accesoMemoria(PID, numeroDePagina, *marcoLibre);
			((*ProcesoX).contadorAccesos)++;

			cargarMarco_MemoriaPrincipal(*marcoLibre, contenido);

			// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
				actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, *marcoLibre);
			queue_push(colitaDeMarcos, elemento);

			return;
		}
		else
		{
			algoritmo_victima_CLOCKMod(numeroDePagina, cantElementos, ProcesoX, contenido, PID, TipoOperacion);
		}
	}else
	{
		algoritmo_victima_CLOCKMod(numeroDePagina, cantElementos, ProcesoX, contenido, PID, TipoOperacion);
	}

	return;
}





/* -----------------------  Algoritmos de la TLB  ----------------------------- */
int algoritmo_FirstFit_TLB()
{
	int entrada;
	int posicionLibre = -1;
	int columna_NumeroPagina = 0;

  pthread_mutex_lock(&mutexStructTLB);
	for (entrada = 0; entrada < Entradas_TLB; entrada++)
	{
		if(structTLB.sCacheTLB[entrada][columna_NumeroPagina] == -1)
		{
			posicionLibre = entrada;
			break;
		}
	}
  pthread_mutex_unlock(&mutexStructTLB);
	return posicionLibre;
}

	int algoritmo_SeleccionVictima_TLB()
	{
	  pthread_mutex_lock(&mutexStructTLB);
		int* victima = queue_pop(structTLB.sOrdenDeIngresoTLB);
	  pthread_mutex_unlock(&mutexStructTLB);
		int victimaSeleccionada = *victima;

		free(victima);
		return victimaSeleccionada;
	}





/* ------------------------  Algoritmos de MEMORIA  --------------------------- */
int algoritmo_FirstFit_MEMORIA()
{
	int posicionActual;
	int posicionLibre = -1;

  pthread_mutex_lock(&mutexStructMP);
  	if ( (structMemoriaPrincipal.sCantMemoriaLibre) > 0)
  	{
		for (posicionActual = 0; posicionActual < Cant_Marcos; posicionActual++)
		{
			if(structMemoriaPrincipal.sMemoriaLibre[posicionActual] == 0)
			{
				posicionLibre = posicionActual;
				break;
			}
		}
	}
  pthread_mutex_unlock(&mutexStructMP);
	return posicionLibre;
}





/* ------------------------  FAuxs de Algoritmos de REEMPLAZO DE PAGINAS  --------------------------- */
int posicionDelPuntero(t_proceso* ProcesoX)
{
	int marcoConPuntero;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;
	
	int cantElementos = queue_size(colitaDeMarcos);

	if(cantElementos != Max_Marcos_Por_Proceso)
	{
		int posMarcoSiguiente = cantElementos;
		
		t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
		int* marcoLibre = (int*)list_get(marcosAsignados, posMarcoSiguiente);
		marcoConPuntero = *marcoLibre;
	}

	if(cantElementos == Max_Marcos_Por_Proceso)
	{
		t_infoDeMarco* elementoDeLaCola = (t_infoDeMarco*)queue_peek(colitaDeMarcos);
		marcoConPuntero = (*elementoDeLaCola).NumeroMarco;	
	}
	
	return marcoConPuntero;
}


void ponerBitDeUso(int numeroDePagina, t_queue* colitaDeMarcos)
{
	int i = 0;
	int cantElementos = queue_size(colitaDeMarcos);
	t_infoDeMarco* elementoDeLaCola;

	if(strcmp(Algoritmo_De_Memoria, "LRU") != 0)
	{
		while(i < cantElementos)
		{
			elementoDeLaCola = queue_pop(colitaDeMarcos);

			if((*elementoDeLaCola).NumeroPagina == numeroDePagina)
			{
				cambiarBitUso(elementoDeLaCola, 1);
			}
			queue_push(colitaDeMarcos, elementoDeLaCola);

			i++;
		}
	}else
	{
		while(i < cantElementos)
		{
			elementoDeLaCola = queue_pop(colitaDeMarcos);

			if((*elementoDeLaCola).NumeroPagina == numeroDePagina)
			{
				cambiarBitUso(elementoDeLaCola, 1);

				queue_push(colitaDeMarcos, elementoDeLaCola);
				break;
			}
			queue_push(colitaDeMarcos, elementoDeLaCola);

			i++;
		}
	}

	return;
}


void ponerBitDeModificado(int numeroDePagina, t_queue* colitaDeMarcos)
{
	int i = 0;
	int cantElementos = queue_size(colitaDeMarcos);
	t_infoDeMarco* elementoDeLaCola;

	if(strcmp(Algoritmo_De_Memoria, "LRU") != 0)
	{
		while(i < cantElementos)
		{
			elementoDeLaCola = queue_pop(colitaDeMarcos);

			if((*elementoDeLaCola).NumeroPagina == numeroDePagina)
			{
				cambiarBitUso(elementoDeLaCola, 1);
				cambiarBitMod(elementoDeLaCola, 1);
			}
			queue_push(colitaDeMarcos, elementoDeLaCola);

			i++;
		}
	}else
	{
		while(i < cantElementos)
		{
			elementoDeLaCola = queue_pop(colitaDeMarcos);

			if((*elementoDeLaCola).NumeroPagina == numeroDePagina)
			{
				cambiarBitUso(elementoDeLaCola, 1);
				cambiarBitMod(elementoDeLaCola, 1);

				queue_push(colitaDeMarcos, elementoDeLaCola);
				break;
			}
			queue_push(colitaDeMarcos, elementoDeLaCola);

			i++;
		}
	}

	return;
}


void cambiarBitUso(t_infoDeMarco* elementoDeLaCola, int nuevoValor)
{
	(*elementoDeLaCola).BitUso = nuevoValor;
	return;
}


void cambiarBitMod(t_infoDeMarco* elementoDeLaCola, int nuevoValor)
{
	(*elementoDeLaCola).BitMod = nuevoValor;
	return;
}


t_queue* obtenerColaInicial(t_queue* colaOriginal)
{
	int i = 0;
	int cantElementos = queue_size(colaOriginal);
	t_queue* colaInicial = queue_create();

	//Copio el contenido en otra cola
	while(i < cantElementos)
	{
		t_infoDeMarco* elementoDeLaCola = queue_pop(colaOriginal);
		t_infoDeMarco* elementoCopia = malloc(sizeof(t_infoDeMarco));
			(*elementoCopia).NumeroPagina = (*elementoDeLaCola).NumeroPagina;
			(*elementoCopia).NumeroMarco = (*elementoDeLaCola).NumeroMarco;
			(*elementoCopia).BitUso = (*elementoDeLaCola).BitUso;
			(*elementoCopia).BitMod = (*elementoDeLaCola).BitMod;
			queue_push(colaInicial, elementoCopia);
		queue_push(colaOriginal, elementoDeLaCola);

		i++;
	}

	return colaInicial;
}





/* ------------------------  Algoritmos de SELECCION y TRATAMIENTO de marcos victima  ------------------------ */
void algoritmo_victima_FIFO(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;

	t_queue* colaInicial = obtenerColaInicial(colitaDeMarcos);

	// OBTENGO VICTIMA y sus datos
	t_infoDeMarco* elementoDeLaCola = queue_pop(colitaDeMarcos);
		int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
		int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

	if(strcmp(TLB_Habilitada, "SI") == 0)
	{
		int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
		quitarPagina_TLB(PID, numPaginaVictima);
		switch(TipoOperacion)
		{
			case 'L':		
					LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				break;
			case 'E':
					LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				break;
		}
	}
	
	// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
		actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);

	// SWAPEO si es necesario, y luego CARGO el NUEVO CONTENIDO en el marco correspondiente.
		if((*elementoDeLaCola).BitMod == 1)
		{
			// LOGEO ACCESO A MEMORIA
				LOG_accesoMemoria(PID, numPaginaVictima, numMarcoVictima);
			((*ProcesoX).contadorAccesos)++;

			char* contenidoASwapear = obtenerContenido_MemoriaPrincipal(numMarcoVictima);
			actionEscrituraDePaginaAlSWAP_SC(PID, numPaginaVictima, contenidoASwapear);
		}
	// LOGEO ACCESO A MEMORIA
		LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
	((*ProcesoX).contadorAccesos)++;
	
	// CARGO el NUEVO CONTENIDO en el marco correspondiente.
	cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
		(*elementoDeLaCola).NumeroPagina = numeroDePagina;

	switch(TipoOperacion)
	{
		case 'L':
				cambiarBitMod(elementoDeLaCola, 0);
			break;
		case 'E':
				cambiarBitMod(elementoDeLaCola, 1);
			break;
	}
	
	// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
		actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
	queue_push(colitaDeMarcos, elementoDeLaCola);

	// LOGEO ACCESO A SWAP
		LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, -1, -1);
	return;
}





void algoritmo_victima_LRU(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;

	t_queue* colaInicial = obtenerColaInicial(colitaDeMarcos);

	// OBTENGO VICTIMA y sus datos
	t_infoDeMarco* elementoDeLaCola = queue_pop(colitaDeMarcos);
		int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
		int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

	if(strcmp(TLB_Habilitada, "SI") == 0)
	{
		int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
		quitarPagina_TLB(PID, numPaginaVictima);
		switch(TipoOperacion)
		{
			case 'L':		
					LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				break;
			case 'E':
					LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
				break;
		}
	}
	
	// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
		actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);
	
	// SWAPEO si es necesario, y luego CARGO el NUEVO CONTENIDO en el marco correspondiente.
		if((*elementoDeLaCola).BitMod == 1)
		{
			// LOGEO ACCESO A MEMORIA
				LOG_accesoMemoria(PID, numPaginaVictima, numMarcoVictima);
			((*ProcesoX).contadorAccesos)++;

			char* contenidoASwapear = obtenerContenido_MemoriaPrincipal(numMarcoVictima);
			actionEscrituraDePaginaAlSWAP_SC(PID, numeroDePagina, contenidoASwapear);
		}
	// LOGEO ACCESO A MEMORIA
		LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
	((*ProcesoX).contadorAccesos)++;

	// CARGO el NUEVO CONTENIDO en el marco correspondiente.
	cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
		(*elementoDeLaCola).NumeroPagina = numeroDePagina;

	switch(TipoOperacion)
	{
		case 'L':
				cambiarBitMod(elementoDeLaCola, 0);
			break;
		case 'E':
				cambiarBitMod(elementoDeLaCola, 1);
			break;
	}
	
	// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
		actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
	queue_push(colitaDeMarcos, elementoDeLaCola);

	// LOGEO ACCESO A SWAP
		LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, -1, -1);
	return;
}





void algoritmo_victima_CLOCKMod(int numeroDePagina, int cantElementos, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion)
{
	int i;
	t_infoDeMarco* elementoDeLaCola;
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_queue* colitaDeMarcos = (*ProcesoX).marcos;
	int ptrColaFinal;

	int ptrColaInicial = posicionDelPuntero(ProcesoX);
	t_queue* colaInicial = obtenerColaInicial(colitaDeMarcos);

	/* APLICO PASO 1 */
	for(i = 0; i < cantElementos; i++)
	{
		elementoDeLaCola = queue_pop(colitaDeMarcos);
		//Me fijo si es 0 0:
			if((*elementoDeLaCola).BitUso == 0 && (*elementoDeLaCola).BitMod == 0)
			{
				// OBTENGO DATOS DE LA VICTIMA
				int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
				int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

				if(strcmp(TLB_Habilitada, "SI") == 0)
				{
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
					quitarPagina_TLB(PID, numPaginaVictima);
					switch(TipoOperacion)
					{
						case 'L':		
								LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
						case 'E':
								LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
					}
				}

					// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
						actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);
					
					// LOGEO ACCESO A MEMORIA
						LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
					((*ProcesoX).contadorAccesos)++;
					
					// CARGO el NUEVO CONTENIDO en el marco correspondiente.
					cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
						(*elementoDeLaCola).NumeroPagina = numeroDePagina;

					switch(TipoOperacion)
					{
						case 'L':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 0);
							break;
						case 'E':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 1);
							break;
					}
						
					// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
						actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
				queue_push(colitaDeMarcos, elementoDeLaCola);

				ptrColaFinal = posicionDelPuntero(ProcesoX);
				// LOGEO ACCESO A SWAP
					LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, ptrColaInicial, ptrColaFinal);
				return;
			}
		queue_push(colitaDeMarcos, elementoDeLaCola);
	}


	/* APLICO PASO 2 */
	for(i = 0; i < cantElementos; i++)
	{
		elementoDeLaCola = queue_pop(colitaDeMarcos);
		//Me fijo si es 0 1:
			if((*elementoDeLaCola).BitUso == 0 && (*elementoDeLaCola).BitMod == 1)
			{
				// OBTENGO DATOS DE LA VICTIMA
				int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
				int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

				if(strcmp(TLB_Habilitada, "SI") == 0)
				{
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
					quitarPagina_TLB(PID, numPaginaVictima);
					switch(TipoOperacion)
					{
						case 'L':		
								LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
						case 'E':
								LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
					}
				}

					// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
						actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);
					
					// SWAPEO, y luego CARGO el NUEVO CONTENIDO en el marco correspondiente.
						if((*elementoDeLaCola).BitMod == 1)
						{
							// LOGEO ACCESO A MEMORIA
								LOG_accesoMemoria(PID, numPaginaVictima, numMarcoVictima);
							((*ProcesoX).contadorAccesos)++;

							char* contenidoASwapear = obtenerContenido_MemoriaPrincipal(numMarcoVictima);
							actionEscrituraDePaginaAlSWAP_SC(PID, numeroDePagina, contenidoASwapear);
						}
					// LOGEO ACCESO A MEMORIA
						LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
					((*ProcesoX).contadorAccesos)++;

					// CARGO el NUEVO CONTENIDO en el marco correspondiente.
					cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
						(*elementoDeLaCola).NumeroPagina = numeroDePagina;

					switch(TipoOperacion)
					{
						case 'L':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 0);
							break;
						case 'E':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 1);
							break;
					}
					
					// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
						actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
				queue_push(colitaDeMarcos, elementoDeLaCola);

				ptrColaFinal = posicionDelPuntero(ProcesoX);
				// LOGEO ACCESO A SWAP
					LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, ptrColaInicial, ptrColaFinal);
				return;
			}
		//Caso contrario, CAMBIO el Bit de Uso, a 0:
			else
			{
				cambiarBitUso(elementoDeLaCola, 0);
				queue_push(colitaDeMarcos, elementoDeLaCola);
			}
	}
	
	
	/* APLICO PASO 1 NUEVAMENTE */
	for(i = 0; i < cantElementos; i++)
	{
		elementoDeLaCola = queue_pop(colitaDeMarcos);
		//Me fijo si es 0 0:
			if((*elementoDeLaCola).BitUso == 0 && (*elementoDeLaCola).BitMod == 0)
			{
				// OBTENGO DATOS DE LA VICTIMA
				int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
				int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

				if(strcmp(TLB_Habilitada, "SI") == 0)
				{
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
					quitarPagina_TLB(PID, numPaginaVictima);
					switch(TipoOperacion)
					{
						case 'L':		
								LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
						case 'E':
								LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
					}
				}

					// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
						actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);
					
					// LOGEO ACCESO A MEMORIA
						LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
					((*ProcesoX).contadorAccesos)++;

					// CARGO el NUEVO CONTENIDO en el marco correspondiente.
					cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
						(*elementoDeLaCola).NumeroPagina = numeroDePagina;

					switch(TipoOperacion)
					{
						case 'L':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 0);
							break;
						case 'E':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 1);
							break;
					}
						
					// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
						actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
				queue_push(colitaDeMarcos, elementoDeLaCola);
				
				ptrColaFinal = posicionDelPuntero(ProcesoX);
				// LOGEO ACCESO A SWAP
					LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, ptrColaInicial, ptrColaFinal);
				return;
			}
		queue_push(colitaDeMarcos, elementoDeLaCola);
	}


	/* APLICO PASO 4 */
	for(i = 0; i < cantElementos; i++)
	{
		elementoDeLaCola = queue_pop(colitaDeMarcos);
		//Me fijo si es 0 1:
			if((*elementoDeLaCola).BitUso == 0 && (*elementoDeLaCola).BitMod == 1)
			{
				// OBTENGO DATOS DE LA VICTIMA
				int numPaginaVictima = (*elementoDeLaCola).NumeroPagina;
				int numMarcoVictima = (*elementoDeLaCola).NumeroMarco;

				if(strcmp(TLB_Habilitada, "SI") == 0)
				{
					int entradaTLB = escribirEntrada_TLB(numeroDePagina, PID, numMarcoVictima);
					quitarPagina_TLB(PID, numPaginaVictima);
					switch(TipoOperacion)
					{
						case 'L':		
								LOG_solicitudLecturaConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
						case 'E':
								LOG_solicitudEscrituraConTLB(PID, numeroDePagina, "MISS", entradaTLB);
							break;
					}
				}

					// ACTUALIZO la Tabla de Pagina (Quito la presencia de memoria de la victima)
						actualizarEntrada_TP(tablaDePaginas, numPaginaVictima, 0, -1);
					
					// SWAPEO, y luego CARGO el NUEVO CONTENIDO en el marco correspondiente.
						if((*elementoDeLaCola).BitMod == 1)
						{
							// LOGEO ACCESO A MEMORIA
								LOG_accesoMemoria(PID, numPaginaVictima, numMarcoVictima);
							((*ProcesoX).contadorAccesos)++;

							char* contenidoASwapear = obtenerContenido_MemoriaPrincipal(numMarcoVictima);
							actionEscrituraDePaginaAlSWAP_SC(PID, numeroDePagina, contenidoASwapear);
						}
					// LOGEO ACCESO A MEMORIA
						LOG_accesoMemoria(PID, numeroDePagina, numMarcoVictima);
					((*ProcesoX).contadorAccesos)++;

					// CARGO el NUEVO CONTENIDO en el marco correspondiente.
					cargarMarco_MemoriaPrincipal(numMarcoVictima, contenido);
						(*elementoDeLaCola).NumeroPagina = numeroDePagina;

					switch(TipoOperacion)
					{
						case 'L':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 0);
							break;
						case 'E':
								cambiarBitUso(elementoDeLaCola, 1);
								cambiarBitMod(elementoDeLaCola, 1);
							break;
					}
					
					// ACTUALIZO la Tabla de Pagina (Le pongo el bit de presencia al elemento cargado)
						actualizarEntrada_TP(tablaDePaginas, numeroDePagina, 1, numMarcoVictima);
				queue_push(colitaDeMarcos, elementoDeLaCola);

				ptrColaFinal = posicionDelPuntero(ProcesoX);
				// LOGEO ACCESO A SWAP
					LOG_accesoSWAP(PID, colaInicial, colitaDeMarcos, ptrColaInicial, ptrColaFinal);
				return;
			}
		//Caso contrario, CAMBIO el Bit de Uso, a 0:
			else
			{
				cambiarBitUso(elementoDeLaCola, 0);
				queue_push(colitaDeMarcos, elementoDeLaCola);
			}
	}
}
