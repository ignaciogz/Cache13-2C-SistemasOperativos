#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sendController.h"
#include "../modelo/sockets.h"
#include "../vistas/mainView.h"

extern t_conexion conectorSWAP;


/* -------------------------------------  FUNCIONES CON EL SWAP  ------------------------------------------- */
int actionConexionAlSWAP_SC(int socketSWAP)
{
	//Conecto el socket, al SWAP para notificar que estoy activo
	if(connect(socketSWAP, (struct sockaddr *)&conectorSWAP.SwapAddr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect");
			exit(1);
		}

	t_header cabecera = ConexionMemoriaSWAP;

	int tamanio = sizeof(t_header);
	void* mensaje = malloc(sizeof(t_header));
	packHeader(cabecera,mensaje);

	if((sendall(socketSWAP,mensaje,&tamanio))==-1){
		mainView_ConexionConSWAP_FAIL();

		close(socketSWAP);
		free(mensaje);
		return 0;
	}

	mainView_ConexionConSWAP_OK();

	close(socketSWAP);
	free(mensaje);

	return 1;
}






int actionCrearProcesoAlSWAP_SC(int PID, int CantidadDePaginas, int* socketDelSwapProcesoN)
{
	if(connect(*socketDelSwapProcesoN, (struct sockaddr *)&conectorSWAP.SwapAddr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect");
			exit(1);
		}

	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = SWAP_NuevoProceso;

		void* mensaje = malloc( sizeof(t_header) + sizeof(int)*2 );
			packHeader(*cabecera,mensaje);
			packCrearProceso(PID, CantidadDePaginas, mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header) + sizeof(int)*2;

	if((sendall(*socketDelSwapProcesoN,mensaje,&tamanioMensaje))==-1){
		mainView_ConexionConSWAP_FAIL();

		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}





int actionLecturaDePaginaAlSWAP_SC(int PID, int NumeroDePagina, int* socketDelSwapLectura)
{
	if(connect(*socketDelSwapLectura, (struct sockaddr *)&conectorSWAP.SwapAddr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect");
			exit(1);
		}

	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = SWAP_LecturaDePagina;

		void* mensaje = malloc( sizeof(t_header) + sizeof(int)*2 );
			packHeader(*cabecera,mensaje);
			packLecturaDePagina(PID, NumeroDePagina, mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header) + sizeof(int)*2;

	if((sendall(*socketDelSwapLectura,mensaje,&tamanioMensaje))==-1){
		mainView_ConexionConSWAP_FAIL();

		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}





int actionEscrituraDePaginaAlSWAP_SC(int PID, int NumeroDePagina, char* Datos)
{
	int* socketDelSwapEscritura = obtenerUnSocket();

	if(connect(*socketDelSwapEscritura, (struct sockaddr *)&conectorSWAP.SwapAddr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect");
			exit(1);
		}

	//Armando el mensaje a enviar:
	int tamanioMensaje = sizeof(t_header) + sizeof(int)*3  + strlen(Datos) + 1;
		
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = SWAP_EscrituraDePagina;
	
		void* mensaje = malloc( tamanioMensaje );
			packHeader(*cabecera,mensaje);
			packEscrituraDePagina(PID, NumeroDePagina, Datos, mensaje);
		free(cabecera);

	if((sendall(*socketDelSwapEscritura,mensaje,&tamanioMensaje))==-1){
		mainView_ConexionConSWAP_FAIL();

		close(*socketDelSwapEscritura);
		free(mensaje);
		return 0;
	}

	void* buffer = malloc(sizeof(t_header));

	recv(*socketDelSwapEscritura, buffer, sizeof(t_header), 0);

	t_header* cabecera2 = malloc(sizeof(t_header));
		umpackHeader(cabecera2, buffer);
	free(buffer);
	free(cabecera2);

	close(*socketDelSwapEscritura);
	free(socketDelSwapEscritura); free(mensaje);
	return 1;
}





int actionFinalizarProcesoAlSWAP_SC(int PID)
{
	int* socketDelSwapFinalizar = obtenerUnSocket();

	if(connect(*socketDelSwapFinalizar, (struct sockaddr *)&conectorSWAP.SwapAddr, sizeof(struct sockaddr)) == -1)
		{
			perror("connect");
			exit(1);
		}

	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = SWAP_FinalizarProceso;

		void* mensaje = malloc( sizeof(t_header) + sizeof(int) );
			packHeader(*cabecera,mensaje);
			packFinalizarProceso(PID,mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header) + sizeof(int);

	if((sendall(*socketDelSwapFinalizar,mensaje,&tamanioMensaje))==-1){
		close(*socketDelSwapFinalizar);
		free(socketDelSwapFinalizar); free(mensaje);
		return 0;
	}

	close(*socketDelSwapFinalizar);
	free(socketDelSwapFinalizar); free(mensaje);
	return 1;
}





/* -------------------------------------  FUNCIONES CON EL CPU  ------------------------------------------- */
int actionCrearProcesoOK_SC(int unSocketDelCPU)
{
	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = CPU_NuevoProcesoOK;

		void* mensaje = malloc(sizeof(t_header));
			packHeader(*cabecera,mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header);

	if((sendall(unSocketDelCPU,mensaje,&tamanioMensaje))==-1){
		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}





int actionCrearProcesoFAIL_SC(int unSocketDelCPU)
{
	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = CPU_NuevoProcesoFAIL;

		void* mensaje = malloc(sizeof(t_header));
			packHeader(*cabecera,mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header);

	if((sendall(unSocketDelCPU,mensaje,&tamanioMensaje))==-1){
		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}





int actionEnviarEscribirPaginaOK_SC(int unSocketDelCPU)
{
	//Armando el mensaje a enviar:
		t_header* cabecera = malloc(sizeof(t_header));
		*cabecera = CPU_EscrituraOK;

		void* mensaje = malloc(sizeof(t_header));
			packHeader(*cabecera,mensaje);
		free(cabecera);

	int tamanioMensaje = sizeof(t_header);

	if((sendall(unSocketDelCPU,mensaje,&tamanioMensaje))==-1){
		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}





int actionEnviarContenidoDePaginaCPU_SC(int unSocketDelCPU, char* Datos)
{
	int Tamanio = strlen(Datos) + 1;
	void* mensaje = malloc( sizeof(int) + strlen(Datos) + 1 );
	int offset=0;
		memcpy(mensaje + offset, (int*)&Tamanio, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (char*)Datos, strlen(Datos) + 1);

	int tamanioMensaje = sizeof(int)  + strlen(Datos) + 1;

	if((sendall(unSocketDelCPU,mensaje,&tamanioMensaje))==-1){
		free(mensaje);
		return 0;
	}

	free(mensaje);
	return 1;
}