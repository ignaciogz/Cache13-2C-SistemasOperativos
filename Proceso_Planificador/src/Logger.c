/*
 * Logger.c
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/Logger.h"
#include "Headers/ManejoDeMensajes.h"
#include <commons/temporal.h>

#include <pthread.h>

pthread_mutex_t mutexLogger = PTHREAD_MUTEX_INITIALIZER;

void logearComienzoEjecucionProceso(t_log* logger, t_PCB* proceso)
{
	char* mensaje = string_new();

	string_append(&mensaje, "Comienzo de ejecucion del proceso: ");
	string_append(&mensaje, proceso->rutaDelArchivo);
	string_append(&mensaje, "con PID: ");
	string_append(&mensaje, string_itoa(proceso->PID));
	string_append(&mensaje, ". \n");

	pthread_mutex_lock(&mutexLogger);
	log_info(logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void logearFinEjecucionProceso(t_log* logger, t_PCB* proceso)
{
	char* mensaje = string_new();

	time_t horaFinEjecucionProceso = time(NULL), diferenciaHoraria;

	diferenciaHoraria = horaFinEjecucionProceso - proceso->horarioInicio;

	struct tm* tiempoEjecucion;
	struct tm* tiempoEspera;
	struct tm* tiempoRespuesta;

	tiempoEjecucion = gmtime(&proceso->tiempoEjecucion);

	tiempoEspera = gmtime(&proceso->tiempoEspera);

	tiempoRespuesta = gmtime(&diferenciaHoraria);

	string_append(&mensaje, "Fin de ejecucion del proceso: ");
	string_append(&mensaje, proceso->rutaDelArchivo);
	string_append(&mensaje, "con PID: ");
	string_append(&mensaje, string_itoa(proceso->PID));
	string_append(&mensaje, ". \n");
	string_append(&mensaje, "Tiempo de respuesta: ");
	string_append(&mensaje,string_itoa(tiempoRespuesta->tm_hour));
	string_append(&mensaje, " horas, ");
	string_append(&mensaje, string_itoa(tiempoRespuesta->tm_min));
	string_append(&mensaje, " minutos, ");
	string_append(&mensaje, string_itoa(tiempoRespuesta->tm_sec));
	string_append(&mensaje, "segundos.\n ");

	string_append(&mensaje, "Tiempo de espera: ");
	string_append(&mensaje, string_itoa(tiempoEspera->tm_hour));
	string_append(&mensaje, " horas, ");
	string_append(&mensaje, string_itoa(tiempoEspera->tm_min));
	string_append(&mensaje, " minutos, ");
	string_append(&mensaje, string_itoa(tiempoEspera->tm_sec));
	string_append(&mensaje, "segundos.\n ");

	string_append(&mensaje, "Tiempo de ejecucion: ");
	string_append(&mensaje, string_itoa(tiempoEjecucion->tm_hour));
	string_append(&mensaje, " horas, ");
	string_append(&mensaje, string_itoa(tiempoEjecucion->tm_min));
	string_append(&mensaje, " minutos, ");
	string_append(&mensaje, string_itoa(tiempoEjecucion->tm_sec));
	string_append(&mensaje, "segundos.\n ");

	pthread_mutex_lock(&mutexLogger);
	log_info(logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void logearConexionCPU(t_log* logger, t_CPU* cpu)
{
	char* mensaje = string_new();

	string_append(&mensaje, "Conexion de cpu con ID: ");
	string_append(&mensaje, string_itoa(cpu->IDCpu));
	string_append(&mensaje, ". \n");

	pthread_mutex_lock(&mutexLogger);
	log_info(logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void logearDesconexionCPU(t_log* logger, t_CPU* cpu)
{
	char* mensaje = string_new();

	string_append(&mensaje, "Desconexion de cpu con ID: ");
	string_append(&mensaje, string_itoa(cpu->IDCpu));
	string_append(&mensaje, ". \n");

	pthread_mutex_lock(&mutexLogger);
	log_info(logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void logearEjecucionAlgoritmoPlanificacion(t_Planificador* planificador, t_PCB* proceso)
{
	char* mensaje = string_new();
	char* colaBloqueados;
	int i, j;
	t_CPU* cpuAux;
	t_PCB* procesoAux;

	string_append(&mensaje, "Ejecucion del algoritmo de planificacion: ");
	if(planificador->algoritmo == FIFO)
		string_append(&mensaje, "FIFO");
	else
	{
		string_append(&mensaje, "Round Robin, con cuantum de: ");
		string_append(&mensaje, string_itoa(planificador->quantum));
	}

	string_append(&mensaje, ". \n");
	string_append(&mensaje, "Proceso seleccionado para ejecutar: ");
	string_append(&mensaje, string_itoa(proceso->PID));
	string_append(&mensaje, ". \n");
	string_append(&mensaje, "Estado de las colas:\n");


	for(i = 0; i < list_size(planificador->CPUs); i++)
	{
		cpuAux = list_get(planificador->CPUs, i);

		string_append(&mensaje, "Cpu: ");
		string_append(&mensaje, string_itoa(cpuAux->IDCpu));
		string_append(&mensaje, ". \n");
		string_append(&mensaje, "Proceso Ejecutando: ");

		if(cpuAux->procesoEnEjecucion != NULL)
			string_append(&mensaje, string_itoa(cpuAux->procesoEnEjecucion->PID));
		else
			string_append(&mensaje, "Ninguno");

		string_append(&mensaje, ". \n");
		string_append(&mensaje, "Cola de listos: ");

		for(j = 0; j < list_size(cpuAux->colaReady); j++)
		{
			procesoAux = list_get(cpuAux->colaReady, j);

			string_append(&mensaje, string_itoa(procesoAux->PID));
			string_append(&mensaje, "  ");
		}
		string_append(&mensaje, ".\n\n");

	}

	string_append(&mensaje, "Cola de bloqueados: ");

	colaBloqueados = armarMensajeColaEntradaSalida(planificador->EntradaSalida);

	string_append(&mensaje, colaBloqueados);

	free(colaBloqueados);

	pthread_mutex_lock(&mutexLogger);
	log_info(planificador->logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void logearFinalizacionRafaga(t_log* logger, int pidProceso, char* mensajesEjecucion)
{
	char* mensaje = string_new();

	string_append(&mensaje, "Fin de rafaga de ejecucion del proceso: ");
	string_append(&mensaje, string_itoa(pidProceso));
	string_append(&mensaje, ". \n");
	string_append(&mensaje, "Mensajes de ejecucion: ");
	string_append(&mensaje, mensajesEjecucion);

	pthread_mutex_lock(&mutexLogger);
	log_info(logger, mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}
