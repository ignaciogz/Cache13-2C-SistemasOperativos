/*
 * Log.c
 *
 *  Created on: 16/10/2015
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h>
#include "Log.h"
#include "sockets.h"

pthread_mutex_t loggerMutex = PTHREAD_MUTEX_INITIALIZER;

void logearCreacionCpu(t_datos data)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU creado: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}



void logearConexion(t_datos data, char* proceso,char* estado)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, proceso);
	string_append(&mensaje, ":");
	string_append(&mensaje, estado);
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearContextoEjecucion(t_datos data,char* ruta)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, "Contexto de Ejecucion");
	string_append(&mensaje, "-->");
	string_append(&mensaje, "mProc:");
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, "  ruta archivo: ");
	string_append(&mensaje, ruta);
	string_append(&mensaje, "   puntero:");
	string_append(&mensaje, string_itoa(data.puntero));
	string_append(&mensaje, "  quantum:");
	string_append(&mensaje, string_itoa(data.quantum));
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearInicio(t_datos data, t_header cabecera)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, "Inicio mProc:");
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, ", paginas:");
	string_append(&mensaje, string_itoa(data.valor));
	string_append(&mensaje, ",retorno:");
	if(cabecera ==24){
		string_append(&mensaje, "Iniciado");
	}else{
		string_append(&mensaje,"Fallo");
	}

	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearLecturaEscritura(t_datos data, char* contenido,char* instruccion)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, instruccion);
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, ", paginas:");
	string_append(&mensaje, string_itoa(data.valor));
	string_append(&mensaje, ",retorno:");
	string_append(&mensaje, contenido);
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearEntradaSalida(t_datos data)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, "EntradaSalida mProc:");
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, ", tiempo:");
	string_append(&mensaje, string_itoa(data.valor));
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearFinalizomProc(t_datos data)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, "Finalizacion mProc:");
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}

void logearFinalizoRafagamProc(t_datos data)
{
	char* mensaje = string_new();

	string_append(&mensaje, "ID del CPU: ");
	string_append(&mensaje, string_itoa(data.ID));
	string_append(&mensaje, "---->");
	string_append(&mensaje, "Finalizacion mProc:");
	string_append(&mensaje, string_itoa(data.PID));
	string_append(&mensaje, ".\n");

	pthread_mutex_lock(&loggerMutex);
	log_info(data.logger, mensaje);
	pthread_mutex_unlock(&loggerMutex);

	free(mensaje);

	return;
}


