/*
 * calculoPorcentaje.c
 *
 *  Created on: 24/11/2015
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "sockets.h"


extern __thread pthread_mutex_t CPU;

void calcularPorcentaje(t_conexion* conexion ){
	int sockPlanif;
	int offset,tam;
	t_header cabecera;
	int retorno;
	void* mensaje = malloc(sizeof(t_header)+sizeof(int)+sizeof(int));
	while(1){
		pthread_mutex_lock(&CPU);
		if(conexion->cpu->tiempoInstruccion !=0){
			conexion->cpu->porcentajeUltMinuto= (((double)conexion->cpu->tiempoInstruccion*100)/60);
			conexion->cpu->tiempoInstruccion=0;
		}else{
			conexion->cpu->porcentajeUltMinuto=0;
		}
		pthread_mutex_unlock(&CPU);

		if((sockPlanif = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			exit(1);
		}

		if(connect(sockPlanif, (struct sockaddr *)&conexion->planifAddr, sizeof(struct sockaddr_in)) == -1)
			{
			perror("connect");
			exit(1);
			}
		cabecera = PorcentajeCpu;
		if(conexion->cpu->porcentajeUltMinuto > 100){
			retorno = 100;

		}else{
		retorno =conexion->cpu->porcentajeUltMinuto;
		}
		printf("ID CPU: %d porcentaje es:%d\n",conexion->ID,retorno);
		tam=sizeof(t_header)+sizeof(int)+sizeof(int);
		offset = sizeof(t_header);
		packHeader(cabecera,mensaje);
		memcpy(mensaje + offset, (int*) &conexion->ID, sizeof(int));
		offset = offset +sizeof(int);
		memcpy(mensaje + offset, (int*) &retorno, sizeof(int));
		if((sendall(sockPlanif,mensaje,&tam))==-1){
				printf("no me pudo mandar el porcentaje");
			}

		close(sockPlanif);
		sleep(60);

	}
return;
}
