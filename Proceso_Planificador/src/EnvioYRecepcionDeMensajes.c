/*
 * EnvioYRecepcionDeMensajes.c
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Headers/EnvioYRecepcionDeMensajes.h"
#include "Headers/Consola.h"


void aceptarConexionesYMensajes(t_Planificador* planificador)
{
	int socketListener, nuevoSocket;
		socklen_t aux;

		struct sockaddr_in nuevaConexion;

		socketListener = socket(AF_INET, SOCK_STREAM, 0);

		if(bind(socketListener, (struct sockaddr*)&planificador->direccion, sizeof(struct sockaddr_in)) == -1)
		{
			perror("Bind: ");
			exit(1);
		}

		if(listen(socketListener, 5) == -1)
		{
			perror("Listen: ");
			exit(1);
		}

		printf("Esperando conexiones de procesos CPUs\n\n");

		aux = sizeof(struct sockaddr_in);

		t_dataHilo* data;
		pthread_t nuevoHilo, hiloConsola, hiloEntradaSalida;
		tipoMensaje header;

		while(1)
		{
			nuevoSocket = accept(socketListener, (struct sockaddr*) &nuevaConexion, &aux);

			data = malloc(sizeof(t_dataHilo));

			data->direccion = nuevaConexion;
			data->socket = nuevoSocket;
			data->planificador = planificador;
			if( recv(data->socket, &data->header, sizeof(tipoMensaje), 0) > 0)
			{
				if( data->header == CONEXIONCPU)
					break;
			}

		}

	pthread_create(&nuevoHilo, NULL, (void*)&recibirMensaje, data);
	pthread_join(nuevoHilo,NULL);
	pthread_create(&hiloConsola, NULL, (void*)&consola, planificador);
	pthread_create(&hiloEntradaSalida, NULL, (void*)&ejecutarEntradaSalida, planificador);

	fd_set master;
	FD_ZERO(&master);
	fd_set setSocketsEscucha;

	FD_SET(socketListener, &master);
	FD_SET(nuevoSocket, &master);
	int maxFD;

	if(socketListener > nuevoSocket)
		maxFD = socketListener + 1;
	else
		maxFD = nuevoSocket + 1;


	int i, recivido;


	while(1)
	{
		setSocketsEscucha = master;

		select(maxFD, &setSocketsEscucha, NULL, NULL,NULL);

		for(i = 0; i < maxFD; i++)
		{
			if(FD_ISSET(i, &setSocketsEscucha))
			{

				if(i == socketListener)
				{
					nuevoSocket = accept(socketListener, (struct sockaddr*) &nuevaConexion, &aux);

					recivido = 	recv(nuevoSocket, &header, sizeof(tipoMensaje), 0);

					data = malloc(sizeof(t_dataHilo));

					data->direccion = nuevaConexion;
					data->socket = nuevoSocket;
					data->planificador = planificador;
					data->header = header;

					pthread_create(&nuevoHilo, NULL, (void*)&recibirMensaje, data);
					pthread_join(nuevoHilo, NULL);

					if(header == CONEXIONCPU)
						FD_SET(nuevoSocket, &master);
				}
				else
				{
				recivido = 	recv(i, &header, sizeof(tipoMensaje), 0);

				if(recivido == -1)
				{
					perror("recv: ");
					exit(1);
				}

				if(recivido == 0)
				{
				desconectarCpu(planificador, i);
				FD_CLR(i, &master);
				}
				else
				{
					data = malloc(sizeof(t_dataHilo));

					data->direccion = nuevaConexion;
					data->socket = i;
					data->planificador = planificador;
					data->header = header;

					pthread_create(&nuevoHilo, NULL, (void*)&recibirMensaje, data);
					pthread_join(nuevoHilo, NULL);

				}
				}
			}
		}

		maxFD = obtenerMaxFD(planificador->CPUs);

	}

	return;
}

int obtenerMaxFD(t_list* listaCPU)
{
	int maxFD = -1;

	t_CPU* aux;

	int i;

	for(i = 0; i < list_size(listaCPU); i++)
	{
		aux = list_get(listaCPU, i);

		if(aux->socketCpu >= maxFD)
			maxFD = aux->socketCpu;
	}


	return maxFD + 1;
}



void recibirMensaje(t_dataHilo* data)
{
	int cpuID;
	t_CPU* cpu;
	int porcentaje;


	switch(data->header)
	{
	case CONEXIONCPU:
		conectarCpu(data->planificador, data->socket, data->direccion);
		break;

	case FINRAFAGA:
		recibirMensajesDeFinDeRafaga(data->socket, data->planificador);
		break;

	case PORCENTAJEUSO:
		recv(data->socket, &cpuID, sizeof(int), 0);
		recv(data->socket, &porcentaje, sizeof(int), 0);

		cpu = obtenerCpuAPartirDelID(cpuID, data->planificador->CPUs);
		cpu->porcentajeUso = porcentaje;

		break;

	default:
		break;
	}

	free(data);

	return;
}

void EnviarPedidoEjecucionSinCorte(t_CPU* cpu, t_PCB* pcb)
{
	char* mensaje = malloc(sizeof(tipoMensaje) + sizeof(int) + strlen(pcb->rutaDelArchivo) + sizeof(int) + sizeof(int) + 1);

	tipoMensaje header = ENVIOPROCESOSINCORTE;

	int desplazamiento = 0;
	int largoNombreArchivo = strlen(pcb->rutaDelArchivo)+1;

	memcpy(mensaje, &header, sizeof(tipoMensaje));
	desplazamiento = sizeof(tipoMensaje);

	memcpy(mensaje + desplazamiento, &largoNombreArchivo, sizeof(int));
	desplazamiento = desplazamiento + sizeof(int);

	memcpy(mensaje + desplazamiento, pcb->rutaDelArchivo, largoNombreArchivo);
	desplazamiento = desplazamiento + largoNombreArchivo;

	memcpy(mensaje + desplazamiento, &pcb->punteroProximaInstruccion, sizeof(int));
	desplazamiento = desplazamiento + sizeof(int);

	memcpy(mensaje + desplazamiento, &pcb->PID, sizeof(int));
	desplazamiento = desplazamiento + sizeof(int);

	EnviarTodo(cpu->socketCpu, mensaje, desplazamiento);

	free(mensaje);

	return;
}

void EnviarPedidoEjecucionConCorte(t_CPU* cpu, t_PCB* pcb, int quantum)
{
	char* mensaje = malloc(sizeof(tipoMensaje) + sizeof(int) + strlen(pcb->rutaDelArchivo) + sizeof(int) + sizeof(int) + sizeof(int) +1);

		tipoMensaje header = ENVIOPROCESOCONCORTE;

		int desplazamiento = 0;
			int largoNombreArchivo = strlen(pcb->rutaDelArchivo)+1;

			memcpy(mensaje, &header, sizeof(tipoMensaje));
			desplazamiento = sizeof(tipoMensaje);

			memcpy(mensaje + desplazamiento, &largoNombreArchivo, sizeof(int));
			desplazamiento = desplazamiento + sizeof(int);

			memcpy(mensaje + desplazamiento, pcb->rutaDelArchivo, largoNombreArchivo);
			desplazamiento = desplazamiento + largoNombreArchivo;

			memcpy(mensaje + desplazamiento, &pcb->punteroProximaInstruccion, sizeof(int));
			desplazamiento = desplazamiento + sizeof(int);

			memcpy(mensaje + desplazamiento, &quantum, sizeof(int));
			desplazamiento = desplazamiento + sizeof(int);

			memcpy(mensaje + desplazamiento, &pcb->PID, sizeof(int));
			desplazamiento = desplazamiento + sizeof(int);

	EnviarTodo(cpu->socketCpu, mensaje, desplazamiento);

	//free(mensaje);

	return;
}

void EnviarTodo(int socketAEnviar, char* mensaje, int largoMensaje)
{
	int enviado = 0;
	int acumulado = 0;

	while(acumulado < largoMensaje)
	{
		enviado = send(socketAEnviar, mensaje + enviado, largoMensaje - enviado, 0);

		if(enviado == -1)
		{
			perror("Send: ");
			exit(1);
		}

		acumulado = acumulado + enviado;
	}

	return;
}
