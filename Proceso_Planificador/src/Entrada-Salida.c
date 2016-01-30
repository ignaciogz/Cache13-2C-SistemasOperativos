/*
 * Entrada-Salida.c
 *
 *  Created on: 25/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Headers/Entrada-Salida.h"
#include <semaphore.h>
#include <commons/temporal.h>

pthread_mutex_t mutexColaEntradaSalida = PTHREAD_MUTEX_INITIALIZER;
sem_t ProductorConsumidor;

void ejecutarEntradaSalida(t_Planificador* planificador)
{
	sem_init(&ProductorConsumidor, 0, 0);
	t_entradaSalida* e_s;
	int restantePorDormir;
	t_CPU* cpu;

	while(1)
	{
		sem_wait(&ProductorConsumidor);
		pthread_mutex_lock(&mutexColaEntradaSalida);
		e_s = list_get(planificador->EntradaSalida, 0);
		list_remove(planificador->EntradaSalida, 0);
		pthread_mutex_unlock(&mutexColaEntradaSalida);

		sumarAlProcesoTiempoDeEspera(e_s->proceso);

		e_s->proceso->horaComienzoEjecucion = time(NULL);

		sleep(e_s->tiempo);


		sumarAlProcesoTiempoDeEjecucion(e_s->proceso);

		e_s->proceso->horaComienzoEspera = time(NULL);

		cpu = obtenerCpuAPartirDelID(e_s->proceso->IDCpuAsignado, planificador->CPUs);

		e_s->proceso->estado = LISTO;

		list_add(cpu->colaReady, e_s->proceso);
			enviarOrdenDeEjecucionACpu(cpu, planificador);


		free(e_s);

	}

	return;
}

void EncolarEntradaSalida(t_PCB* proceso, int tiempo, t_Planificador* planificador)
{
	t_entradaSalida* e_s = malloc(sizeof(t_entradaSalida));

	e_s->proceso = proceso;
	sumarAlProcesoTiempoDeEjecucion(proceso);
	e_s->tiempo = tiempo;

	proceso->horaComienzoEspera = time(NULL);


	pthread_mutex_lock(&mutexColaEntradaSalida);
	list_add(planificador->EntradaSalida, e_s);
	pthread_mutex_unlock(&mutexColaEntradaSalida);

	sem_post(&ProductorConsumidor);
}

char* armarMensajeColaEntradaSalida(t_list* listaEntradaSalida)
{
	char* mensaje = string_new();
	int i;
	t_entradaSalida* e_s;

	pthread_mutex_lock(&mutexColaEntradaSalida);

	for(i = 0; i < list_size(listaEntradaSalida); i++)
	{
		e_s = list_get(listaEntradaSalida, i);

		string_append(&mensaje, string_itoa(e_s->proceso->PID));
		string_append(&mensaje, "  ");
	}

	string_append(&mensaje, ". \n");

	pthread_mutex_unlock(&mutexColaEntradaSalida);

	return mensaje;
}
