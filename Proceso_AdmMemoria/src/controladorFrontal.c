#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "modelo/metricas.h"
#include "controladores/sendController.h"
#include "controladores/recvController.h"
#include "controladores/signalController.h"
#include "controladorFrontal.h"
#include <pthread.h>

extern char* TLB_Habilitada;

void actionEscucharSolicitudesDelCPU(int socketMemoria, struct sockaddr_in myAddr)
{
	//Asigno el socket, al puerto pasado por el archivo de configuracion
	if((bind(socketMemoria, (struct sockaddr*)&myAddr, (socklen_t) sizeof(struct sockaddr)) == -1))
		{
			perror("bindSocketMemoria");
			exit(1);
		}

	//Dejo al socket escuchando nuevas conexiones
	if (listen(socketMemoria, 10) == -1)
		{
			perror("listenSocketMemoria");
			exit(1);
		}


	int socketAccept;
	int sin_size = sizeof(struct sockaddr_in);
	struct sockaddr_in acceptAddr;
	t_data * datosDeSolicitud;

	signalINI();
	
	if(strcmp(TLB_Habilitada, "SI") == 0)
	{
		pthread_t historico;
		pthread_create(&historico, NULL, (void*)&actionHistorico, NULL);	
	}

	while(1)
	{
		//Acepto las solicitudes entrantes
		socketAccept = accept(socketMemoria, (struct sockaddr*) &acceptAddr,(socklen_t*) &sin_size);

		void* buffer = malloc(sizeof(t_header));
		recv(socketAccept, buffer, sizeof(t_header), 0);

		//Desempaqueto la cabecera
			t_header* cabecera = malloc(sizeof(t_header));
			umpackHeader(cabecera, buffer);

		datosDeSolicitud = (t_data *)malloc(sizeof(t_data));
			datosDeSolicitud->SocketSolicitante = socketAccept;
			datosDeSolicitud->Cabecera = *cabecera;

		free(cabecera); free(buffer);

		pthread_t newThread;
		pthread_create(&newThread, NULL, (void*)&actionResponderSolicitud, datosDeSolicitud);
	}

	return;
}

void actionResponderSolicitud(t_data * datosDeSolicitud)
{
	t_header routing = datosDeSolicitud->Cabecera;

	switch(routing)
	{
		//Para el proyecto
		case CPU_NuevoProceso:		actionCrearProceso_RC(datosDeSolicitud);
									break;

		case CPU_LecturaDePagina:	actionLecturaDePagina_RC(datosDeSolicitud);
									break;

		case CPU_EscrituraDePagina: actionEscrituraDePagina_RC(datosDeSolicitud);
									break;

		case CPU_FinalizarProceso:	actionFinalizarProceso_RC(datosDeSolicitud);
									break;
		default: break;
	}

	close(datosDeSolicitud->SocketSolicitante);
	free(datosDeSolicitud);
	return;
}