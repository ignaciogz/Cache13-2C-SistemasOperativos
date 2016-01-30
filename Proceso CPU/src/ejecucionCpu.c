/*
 * ejecucionCpu.c
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/process.h>
#include "configuracion.h"
#include "sockets.h"
#include "manejoSolicitudes.h"
#include "Log.h"
#include "ejecucionCpu.h"
#include "calculoPorcentaje.h"




void ejecucionCpu(t_documentacion* doc){
	//variables
	int Puerto_Planificador;
	int Puerto_Memoria;
	t_datos data;
	t_conexion conexion;

	conexion.cpu = malloc(sizeof(t_cpu));

	conexion.cpu->porcentajeUltMinuto=0;
	conexion.cpu->tiempoInstruccion=0;


	data.logger = doc->logger;
	data.ID =process_get_thread_id();
	conexion.ID=data.ID;
	logearCreacionCpu(data);


	//-----------------------Datos Cpu------------------------
	data.retardo =obtener_double(doc->config,"RETARDO");

	//--------------------Datos del Planificador--------------------------
	Puerto_Planificador =obtener_int(doc->config,"PUERTO_PLANIFICADOR");
	char*IP_Planificador =obtener_string(doc->config,"IP_PLANIFICADOR");

	//--------------------Datos de la Memoria--------------------------
	char*IP_Memoria =obtener_string(doc->config,"IP_MEMORIA");
	Puerto_Memoria = obtener_int(doc->config,"PUERTO_MEMORIA");

	mostrarIdHilo(data.ID);

	int socketPlanif, socketCpu, socketMem;


	if((socketCpu = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			exit(1);
		}

	if((socketPlanif = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			exit(1);
		}

	if((socketMem = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			exit(1);
		}

	//Inicializa la estructura de direccion del planificador
	conexion.planifAddr = inicializarDireccion(Puerto_Planificador, IP_Planificador);

	//Inicializa la estructura de direccion de la memoria
	conexion.memAddr = inicializarDireccion(Puerto_Memoria, IP_Memoria);

	printf("Esperando Conexion con Planificador...\n");
	if(connect(socketPlanif, (struct sockaddr *)&conexion.planifAddr, sizeof(struct sockaddr_in)) == -1)
		{
			perror("connect");
			logearConexion(data,"Proceso Planificador","Desconectado");
			exit(1);
		}

	printf("Conexion con planificador establecida\n");
	logearConexion(data,"Proceso Planificador","Conectado");

	t_header cabecera = ConexionCpuPlanif;
	int tamanio = sizeof(t_header)+sizeof(int);

	int offset = sizeof(t_header);
	void* mensaje = malloc(tamanio);
	packHeader(cabecera,mensaje);
	memcpy(mensaje + offset, (int*) &data.ID, sizeof(int));

	if((sendall(socketPlanif,mensaje,&tamanio))==-1){
		printf("Se desconecto el planificador\n");
	}

	free(mensaje);

	printf("Esperando Conexion con Memoria...\n");
	if(connect(socketMem, (struct sockaddr *)&conexion.memAddr, sizeof(struct sockaddr_in)) == -1)
		{
			perror("connect");
			logearConexion(data,"Administrador de Memoria","Desconectado");
			exit(1);
		}

	printf("Proceso CPU se encuentra operativo.\n");
	logearConexion(data,"Administrador de Memoria","Conectado");

	close(socketMem);



	pthread_t usoCpu;
	pthread_create(&usoCpu, NULL, (void*)&calcularPorcentaje,(void*)&conexion);



	struct sockaddr_in acceptAddr;

	fd_set master;
		FD_ZERO(&master);
		fd_set setSocketsEscucha;


		FD_SET(socketPlanif, &master);
		int maxFD;


			maxFD = socketPlanif + 1;


		int i, recivido;


		while(1)
		{
			setSocketsEscucha = master;

			select(maxFD, &setSocketsEscucha, NULL, NULL,NULL);

			for(i = 0; i < maxFD; i++)
			{
				if(FD_ISSET(i, &setSocketsEscucha))
				{
					void* buffer = malloc(sizeof(t_header));
					recivido = 	recv(i, buffer, sizeof(t_header), 0);

					if(recivido == -1)
					{
						perror("recv: ");
						exit(1);
					}

					if(recivido == 0)
					{
						FD_CLR(i, &master);
					}
					else
					{
						t_header header;
						umpackHeader(&header, buffer);
						conexion.i=i;
						ejecutarSolicitud(acceptAddr,header,conexion,data);

					}
				}
			}

		}
	return;
}


