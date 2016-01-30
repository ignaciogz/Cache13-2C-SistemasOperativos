#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

#include "metricas.h"

//Contadores Globales
	extern int Aciertos_Historica;
	extern int Accesos_Historica;
	extern pthread_mutex_t mutexHistorica;

void actionHistorico()
{	
	while(1)
	{
		tasaDeAciertos();
		sleep(60);
	}
}

void tasaDeAciertos()
{
	pthread_mutex_lock(&mutexHistorica);
		int cantAccesos = Accesos_Historica;
		int cantAciertosTLB = Aciertos_Historica;		
	pthread_mutex_unlock(&mutexHistorica);

	if(cantAccesos != 0)
	{
		int tasaAciertos = ((float)(cantAciertosTLB) / (float)cantAccesos) * 100;
		printf("\033[37mCantidad Accesos: %i Cantidad Aciertos TLB: %i --> Tasa de Aciertos: %i%%\n", cantAccesos, cantAciertosTLB, tasaAciertos);
	}else{
		printf("\033[37mCantidad Accesos: 0 Cantidad Aciertos TLB: 0 --> Tasa de Aciertos: 0%%\n");
	}
}
