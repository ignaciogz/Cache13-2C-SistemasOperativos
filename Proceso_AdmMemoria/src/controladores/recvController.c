#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "recvController.h"
#include "sendController.h"
#include "../modelo/logeador.h"
#include "../modelo/estructuras.h"
#include "../modelo/algoritmos.h"

//Variables de configuracion:
	//extern int Max_Marcos_Por_Proceso;
	extern char* TLB_Habilitada;

//Contadores Globales
	extern int Aciertos_Historica;
	extern int Accesos_Historica;
	extern pthread_mutex_t mutexHistorica;


//
void actionCrearProceso_RC(t_data * datosDeSolicitud)
{
	int* PID = umpackPID(datosDeSolicitud);
	int* CantidadDePaginas = umpackCantidadDePaginas(datosDeSolicitud);
	int* CPU = umpackCPU(datosDeSolicitud);

		if(hayEspacioParaSolicitudDeMarcos(1))
		{
			printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m CREAR el proceso: %i con: %i paginas\033[0;37m\n", *CPU, *PID, *CantidadDePaginas);
			intentarCrearProcesoNuevo(datosDeSolicitud, PID, CantidadDePaginas);
		}
		else
		{
			printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m CREAR el proceso: %i con: %i paginas\033[0;37m\n", *CPU, *PID, *CantidadDePaginas);
			//Informo al CPU, que no se puede crear el mProc!
			actionCrearProcesoFAIL_SC(datosDeSolicitud->SocketSolicitante);
			printf("\033[34m::\033[30m No se puede crear un nuevo mProc, NO hay espacio en MEMORIA para el proceso\033[37m\n");
		}

	free(PID); free(CantidadDePaginas); free(CPU);
	return;
}





//
void actionLecturaDePagina_RC(t_data * datosDeSolicitud)
{
	int* PID = umpackPID(datosDeSolicitud);
	int* NumeroDePagina = umpackNumeroDePagina(datosDeSolicitud);
	int* CPU = umpackCPU(datosDeSolicitud);
	int NumeroDeMarco; char* Contenido = NULL; t_proceso* ProcesoX; t_queue* ColitaDeMarcos; int entradaTLB;

	printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m LEER del proceso: %i la pagina: %i\033[0;37m\n", *CPU, *PID, *NumeroDePagina);
	ProcesoX = obtenerProceso(*PID);
			pthread_mutex_lock(&mutexHistorica);
				Accesos_Historica++;
			pthread_mutex_unlock(&mutexHistorica);


	if(strcmp(TLB_Habilitada, "SI") == 0)
	{
		if( paginaCargada_TLB(*NumeroDePagina, *PID) )
		{
			pthread_mutex_lock(&mutexHistorica);
				Aciertos_Historica++;
			pthread_mutex_unlock(&mutexHistorica);

			NumeroDeMarco = obtenerMarcoDePagina_TLB(*NumeroDePagina, *PID);
			LOG_solicitudLecturaConTLB(*PID, *NumeroDePagina, "HIT", NumeroDeMarco);
				/*printf("\033[34m::\033[30m ESTA en TLB, Obtengo su marco y Busco los datos ----> MP\033[37m\n");*/
			LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
			Contenido = obtenerContenido_MemoriaPrincipal(NumeroDeMarco);

			//Debo cambiarle el bit de uso a la pagina
				ColitaDeMarcos = obtenerMarcosProceso(*PID);
				ponerBitDeUso(*NumeroDePagina, ColitaDeMarcos);
		}else
		{
			int** tablaDePaginas = obtenerTPProceso(*PID);
			if( paginaPresente_TP(tablaDePaginas, *NumeroDePagina) )
			{
				NumeroDeMarco = obtenerMarcoDePagina_TP(tablaDePaginas, *NumeroDePagina);
				//Debo actualizar la TLB
					entradaTLB = escribirEntrada_TLB(*NumeroDePagina, *PID, NumeroDeMarco);
				LOG_solicitudLecturaConTLB(*PID, *NumeroDePagina, "MISS", entradaTLB);
					/*printf("\033[34m::\033[30m NO esta en TLB, Miro la TP y esta presente en MP ----> MP\033[37m\n");*/
				LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
				Contenido = obtenerContenido_MemoriaPrincipal(NumeroDeMarco);

				//Debo cambiarle el bit de uso a la pagina
					ColitaDeMarcos = obtenerMarcosProceso(*PID);
					ponerBitDeUso(*NumeroDePagina, ColitaDeMarcos);
			}else
			{
				/*printf("\033[34m::\033[30m NO esta en TLB, Miro la TP y NO esta presente en MP ----> SWAP\033[37m\n");*/
				((*ProcesoX).contadorPageFault)++;

				//Voy al SWAP, obtengo y cargo la pagina
				int* socketSwapLectura = obtenerUnSocket();	
				if(actionLecturaDePaginaAlSWAP_SC(*PID, *NumeroDePagina, socketSwapLectura))
				{
					Contenido = actionLecturaDePaginaRESULT_RC(socketSwapLectura);
					
					//Algoritmo de reemplazo de pagina
						algoritmo_reemplazo_pagina(*NumeroDePagina, ProcesoX, Contenido, *PID, 'L');
				}
			}
		}
	}else /* ------------- LA TLB NO ESTA HABILITADA ------------- */
	{
		int** tablaDePaginas = obtenerTPProceso(*PID);
		if( paginaPresente_TP(tablaDePaginas, *NumeroDePagina) )
		{
			NumeroDeMarco = obtenerMarcoDePagina_TP(tablaDePaginas, *NumeroDePagina);
			LOG_solicitudLectura(*PID, *NumeroDePagina);
				/*printf("\033[34m::\033[30m NO existe TLB, Miro la TP y esta presente en MP ----> MP\033[37m\n");*/
			LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
			Contenido = obtenerContenido_MemoriaPrincipal(NumeroDeMarco);

			//Debo cambiarle el bit de uso a la pagina
				ColitaDeMarcos = obtenerMarcosProceso(*PID);
				ponerBitDeUso(*NumeroDePagina, ColitaDeMarcos);

			//NO debo actualizar la TLB porque no esta :P
		}else
		{
			/*printf("\033[34m::\033[30m NO existe TLB, Miro la TP y NO esta presente en MP ----> SWAP\033[37m\n");*/
			((*ProcesoX).contadorPageFault)++;

			//Voy al SWAP, obtengo y cargo la pagina
			int* socketSwapLectura = obtenerUnSocket();	
			if(actionLecturaDePaginaAlSWAP_SC(*PID, *NumeroDePagina, socketSwapLectura))
			{
				Contenido = actionLecturaDePaginaRESULT_RC(socketSwapLectura);
				LOG_solicitudLectura(*PID, *NumeroDePagina);

				//Algoritmo de reemplazo de pagina:
					algoritmo_reemplazo_pagina(*NumeroDePagina, ProcesoX, Contenido, *PID, 'L');

				//NO debo actualizar la TLB porque no esta :P
			}
		}
	}

	/* ------------- ENVIO EL CONTENIDO DE LA PAGINA AL CPU ------------- */
	actionEnviarContenidoDePaginaCPU_SC(datosDeSolicitud->SocketSolicitante, Contenido);
			/*printf("\033[34m::\033[30m El contenido de la pagina %i, enviada al CPU es: \033[34m%s\033[37m\n", *NumeroDePagina, Contenido);*/
	
	free(PID); free(NumeroDePagina); free(CPU);
	return;
}





//
void actionEscrituraDePagina_RC(t_data * datosDeSolicitud)
{
	int* PID = umpackPID(datosDeSolicitud);
	int* NumeroDePagina = umpackNumeroDePagina(datosDeSolicitud);
	char* Datos = umpackDatos(datosDeSolicitud);
	int* CPU = umpackCPU(datosDeSolicitud);
	int NumeroDeMarco; t_proceso* ProcesoX; t_queue* ColitaDeMarcos; int entradaTLB;

	printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m ESCRIBIR del proceso: %i la pagina: %i\033[0;37m\n", *CPU, *PID, *NumeroDePagina);
	ProcesoX = obtenerProceso(*PID);
			pthread_mutex_lock(&mutexHistorica);
				Accesos_Historica++;
			pthread_mutex_unlock(&mutexHistorica);

	
	if(strcmp(TLB_Habilitada, "SI") == 0)
	{
		if( paginaCargada_TLB(*NumeroDePagina, *PID) )
		{
			pthread_mutex_lock(&mutexHistorica);
				Aciertos_Historica++;
			pthread_mutex_unlock(&mutexHistorica);

			NumeroDeMarco = obtenerMarcoDePagina_TLB(*NumeroDePagina, *PID);
			LOG_solicitudEscrituraConTLB(*PID, *NumeroDePagina, "HIT", NumeroDeMarco);
				/*printf("\033[34m::\033[30m ESTA en TLB, Obtengo su marco y Reemplazo los datos ----> MP\033[37m\n");*/
			LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
			cargarMarco_MemoriaPrincipal(NumeroDeMarco, Datos);

			//Debo cambiarle el bit de modificado a la pagina
				ColitaDeMarcos = obtenerMarcosProceso(*PID);
				ponerBitDeModificado(*NumeroDePagina, ColitaDeMarcos);
		}else
		{
			int** tablaDePaginas = obtenerTPProceso(*PID);
			if( paginaPresente_TP(tablaDePaginas, *NumeroDePagina) )
			{
				NumeroDeMarco = obtenerMarcoDePagina_TP(tablaDePaginas, *NumeroDePagina);
				//Debo actualizar la TLB
					entradaTLB = escribirEntrada_TLB(*NumeroDePagina, *PID, NumeroDeMarco);
				LOG_solicitudEscrituraConTLB(*PID, *NumeroDePagina, "MISS", entradaTLB);	
					/*printf("\033[34m::\033[30m NO esta en TLB, Miro la TP y esta presente en MP ----> MP\033[37m\n");*/
				LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
				cargarMarco_MemoriaPrincipal(NumeroDeMarco, Datos);
				
				//Debo cambiarle el bit de modificado a la pagina
					ColitaDeMarcos = obtenerMarcosProceso(*PID);
					ponerBitDeModificado(*NumeroDePagina, ColitaDeMarcos);
			}else
			{
				/*printf("\033[34m::\033[30m NO esta en TLB, Miro la TP y NO esta presente en MP ----> QUIZAS SWAP\033[37m\n");*/
				((*ProcesoX).contadorPageFault)++;

				//Algoritmo de reemplazo de pagina
					algoritmo_reemplazo_pagina(*NumeroDePagina, ProcesoX, Datos, *PID, 'E');
			}
		}
	}else /* ------------- LA TLB NO ESTA HABILITADA ------------- */
	{
		int** tablaDePaginas = obtenerTPProceso(*PID);
		if( paginaPresente_TP(tablaDePaginas, *NumeroDePagina) )
		{
			NumeroDeMarco = obtenerMarcoDePagina_TP(tablaDePaginas, *NumeroDePagina);
			LOG_solicitudEscritura(*PID, *NumeroDePagina);	
				/*printf("\033[34m::\033[30m NO existe TLB, Miro la TP y esta presente en MP ----> MP\033[37m\n");*/
			LOG_accesoMemoria(*PID, *NumeroDePagina, NumeroDeMarco);
			cargarMarco_MemoriaPrincipal(NumeroDeMarco, Datos);
				
			//Debo cambiarle el bit de modificado a la pagina
				ColitaDeMarcos = obtenerMarcosProceso(*PID);
				ponerBitDeModificado(*NumeroDePagina, ColitaDeMarcos);

			//NO debo actualizar la TLB porque no esta :P
		}else
		{
			/*printf("\033[34m::\033[30m NO existe TLB, Miro la TP y NO esta presente en MP ----> QUIZAS SWAP\033[37m\n");*/
			((*ProcesoX).contadorPageFault)++;

			LOG_solicitudEscritura(*PID, *NumeroDePagina);

			//Algoritmo de reemplazo de pagina
				algoritmo_reemplazo_pagina(*NumeroDePagina, ProcesoX, Datos, *PID, 'E');

			//NO debo actualizar la TLB porque no esta :P
		}
	}

	/* ------------- ENVIO QUE FINALIZO LA ESCRITURA DE LA PAGINA AL CPU ------------- */
	actionEnviarEscribirPaginaOK_SC(datosDeSolicitud->SocketSolicitante);
			/*printf("\033[34m::\033[30m La escritura finalizo correctamente y se informo al CPU\n");*/
	
	free(PID); free(NumeroDePagina); free(Datos); free(CPU);
	return;
}





//
void actionFinalizarProceso_RC(t_data * datosDeSolicitud)
{
	int* PID = umpackPID(datosDeSolicitud);
	int* CPU = umpackCPU(datosDeSolicitud);

		eliminarProceso(*PID);
		if(actionFinalizarProcesoAlSWAP_SC(*PID))
		{
			printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m FINALIZAR el proceso: %i\033[0;37m\n", *CPU, *PID);
			printf("\033[34m::\033[30m Se elimino correctamente el proceso %i\033[37m\n", *PID);
		}
		else
		{
			printf("\033[1;37mCPU ID%i \033[0;34m->\033[1;37m FINALIZAR el proceso: %i\033[0;37m\n", *CPU, *PID);
			printf("\033[34m::\033[30m No se pudo enviar la orden de finalizar el proceso al SWAP\033[37m\n");
		}

	free(PID); free(CPU);
	return;
}






//Recibo del SWAP las cabeceras: Ok o Fail. "El mismo socket que envio, RECIBE" !!!
int actionCrearProcesoRESULT_RC(int* socketSWAP)
{
	void* buffer = malloc(sizeof(t_header));
	int resultado;

	recv(*socketSWAP, buffer, sizeof(t_header), 0);

	t_header* cabecera = malloc(sizeof(t_header));
		umpackHeader(cabecera, buffer);
	free(buffer);

	close(*socketSWAP); free(socketSWAP);
	if(*cabecera == SWAP_NuevoProcesoOK)
	{		
		resultado = 1;
	}
	else if(*cabecera == SWAP_NuevoProcesoFAIL)
	{
		resultado = 0;
	}
	free(cabecera);
	return resultado;
}





//Recibo del SWAP el contenido de una pagina solicitada. "El mismo socket que envio, RECIBE" !!!
char* actionLecturaDePaginaRESULT_RC(int* socketSWAP)
{
	t_data* datosDeSolicitud = malloc(sizeof(t_data));
	datosDeSolicitud->SocketSolicitante = *socketSWAP;
	char* contenido = umpackDatos(datosDeSolicitud);
	
	free(datosDeSolicitud);
	close(*socketSWAP); free(socketSWAP);
	return contenido;
}



/* -------------------------------------  F Auxiliares  ------------------------------------------- */
void intentarCrearProcesoNuevo(t_data * datosDeSolicitud, int* PID, int* CantidadDePaginas)
{
	int* socketSwapProcesoN = obtenerUnSocket();
		
	actionCrearProcesoAlSWAP_SC(*PID, *CantidadDePaginas, socketSwapProcesoN);
	if(actionCrearProcesoRESULT_RC(socketSwapProcesoN))
	{
		inicializarProceso(*PID, *CantidadDePaginas);
		actionCrearProcesoOK_SC(datosDeSolicitud->SocketSolicitante);
		LOG_mProcCreado(*PID, *CantidadDePaginas);
		printf("\033[34m::\033[30m Se creo correctamente el proceso %i con: %i paginas\033[37m\n", *PID, *CantidadDePaginas);			
	}
	else
	{
		actionCrearProcesoFAIL_SC(datosDeSolicitud->SocketSolicitante);
		printf("\033[34m::\033[30m No se puede crear un nuevo mProc, el SWAP no tiene espacio\033[37m\n");
	}
	return;
}
