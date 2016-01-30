/*
 ============================================================================
 Name        : proceso.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/config.h>
#include <commons/log.h>
#include "configuracion.h"
#include "ejecucionCpu.h"
#include "sockets.h"

__thread t_cpu cpu;
__thread pthread_mutex_t CPU = PTHREAD_MUTEX_INITIALIZER;
int main(void) {
	int cant_Hilos,i;
	t_documentacion doc;

	doc.logger = log_create("Documentacion/LogCpu.txt", "Proceso CPU", false, LOG_LEVEL_INFO);
	doc.config = config_create("Documentacion/configuracion.cfg");
	if(doc.config== NULL)
		terminarProceso("No se pudo crear el arch de config");

	cant_Hilos =obtener_int(doc.config,"CANTIDAD_HILOS");
	pthread_t hiloId[cant_Hilos];


	for(i=0;i < cant_Hilos; i++){
		pthread_create(&hiloId[i],NULL,(void *)ejecucionCpu,(void*)&doc);

	}

	for(i=0;i < cant_Hilos; i++){
		pthread_join(hiloId[i],NULL);
	}

	return EXIT_SUCCESS;

}

