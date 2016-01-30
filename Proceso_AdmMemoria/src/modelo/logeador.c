#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h>

#include "logeador.h"
#include "estructuras.h"

extern t_log* archivoLogger;
pthread_mutex_t mutexLogger = PTHREAD_MUTEX_INITIALIZER;

void LOG_mProcCreado(int pid, int cantidadPaginas)
{
	char * mensaje = string_new();
	string_append(&mensaje," [MPROC NUEVO] ");
	char * nro_pid = string_itoa(pid);
	char * paginasAsignadas = string_itoa(cantidadPaginas);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de paginas asignadas: ");
			string_append(&mensaje,paginasAsignadas);
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(paginasAsignadas);

	return;
}

void LOG_solicitudEscritura(int pid, int numeroDePagina)
{
	char * mensaje = string_new();
	string_append(&mensaje," [ESCRITURA] ");
	char * nro_pid = string_itoa(pid);
	char * nro_pagina = string_itoa(numeroDePagina);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de pagina: ");
			string_append(&mensaje,nro_pagina);
		string_append(&mensaje," ");
		string_append(&mensaje,"TLB: Deshabilitada");
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(nro_pagina);

	return;
}

void LOG_solicitudEscrituraConTLB(int pid, int numeroDePagina, char* eventoTLB, int marco)
{
	char * mensaje = string_new();
	string_append(&mensaje," [ESCRITURA] ");
	char * nro_pid = string_itoa(pid);
	char * nro_pagina = string_itoa(numeroDePagina);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de pagina: ");
			string_append(&mensaje,nro_pagina);
		string_append(&mensaje," ");
		string_append(&mensaje,"Evento TLB: ");
			string_append(&mensaje,eventoTLB);
		
		if(strcmp(eventoTLB, "HIT") == 0)
		{
			char * nro_marco = string_itoa(marco);
			string_append(&mensaje," Marco resultante: ");
				string_append(&mensaje,nro_marco);
			free(nro_marco);
		}
		if(strcmp(eventoTLB, "MISS") == 0)
		{
			char * nro_marco = string_itoa(marco);
			string_append(&mensaje," Victima algoritmo: ");
				string_append(&mensaje,nro_marco);
			free(nro_marco);
		}
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(nro_pagina);

	return;
}

void LOG_solicitudLectura(int pid, int numeroDePagina)
{
	char * mensaje = string_new();
	string_append(&mensaje," [LECTURA] ");
	char * nro_pid = string_itoa(pid);
	char * nro_pagina = string_itoa(numeroDePagina);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de pagina: ");
			string_append(&mensaje,nro_pagina);
		string_append(&mensaje," ");
		string_append(&mensaje,"TLB: Deshabilitada");
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(nro_pagina);

	return;
}

void LOG_solicitudLecturaConTLB(int pid, int numeroDePagina, char* eventoTLB, int marco)
{
	char * mensaje = string_new();
	string_append(&mensaje," [LECTURA] ");
	char * nro_pid = string_itoa(pid);
	char * nro_pagina = string_itoa(numeroDePagina);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de pagina: ");
			string_append(&mensaje,nro_pagina);
		string_append(&mensaje," ");
		string_append(&mensaje,"Evento TLB: ");
			string_append(&mensaje,eventoTLB);
		
		if(strcmp(eventoTLB, "HIT") == 0)
		{
			char * nro_marco = string_itoa(marco);
			string_append(&mensaje," Marco resultante: ");
				string_append(&mensaje,nro_marco);
			free(nro_marco);
		}
		if(strcmp(eventoTLB, "MISS") == 0)
		{
			char * nro_marco = string_itoa(marco);
			string_append(&mensaje," Victima algoritmo: ");
				string_append(&mensaje,nro_marco);
			free(nro_marco);
		}
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(nro_pagina);

	return;
}

void LOG_accesoMemoria(int pid, int numPagina, int numMarco)
{
	char * mensaje = string_new();
	string_append(&mensaje," [ACCESO MEMORIA] ");
	char * nro_pid = string_itoa(pid);
	char * nro_pagina = string_itoa(numPagina);
	char * nro_marco = string_itoa(numMarco);
		string_append(&mensaje,"PID: ");
			string_append(&mensaje,nro_pid);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de paginas: ");
			string_append(&mensaje,nro_pagina);
		string_append(&mensaje," ");
		string_append(&mensaje,"N de marco: ");
			string_append(&mensaje,nro_marco);
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);
	free(nro_pid);
	free(nro_pagina);
	free(nro_marco);

	return;
}

void LOG_accesoSWAP(int PID, t_queue* colitaINICIAL, t_queue* colitaFINAL, int ptrColitaINICIAL, int ptrColitaFINAL)
{
	int i;
	int cantElementos;

	char * mensaje = string_new();
	string_append(&mensaje," [ACCESO SWAP] ");
	
	// AGREGO EL PID:
		char * nro_pid = string_itoa(PID);
			string_append(&mensaje,"PID: ");
		string_append(&mensaje, nro_pid);
		string_append(&mensaje,".\n");

	
	// AGREGO LA PRIMER COLA:
		string_append(&mensaje,"Estado Inicial -> \n");
		if(ptrColitaINICIAL != -1)
		{
			char * ptr_colitaInicial = string_itoa(ptrColitaINICIAL);
			string_append(&mensaje,"Ptr cola Inicial: ");
				string_append(&mensaje, ptr_colitaInicial);
			string_append(&mensaje,"\n");
			free(ptr_colitaInicial);
		}

		i = 0;
		cantElementos = queue_size(colitaINICIAL);
		while(i < cantElementos)
		{
			t_infoDeMarco* elementoDeLaCola = queue_pop(colitaINICIAL);
				char * NumeroPagina_Inicial = string_itoa((*elementoDeLaCola).NumeroPagina);
				char * NumeroMarco_Inicial = string_itoa((*elementoDeLaCola).NumeroMarco);
				char * BitUso_Inicial = string_itoa((*elementoDeLaCola).BitUso);
				char * BitMod_Inicial = string_itoa((*elementoDeLaCola).BitMod);

				string_append(&mensaje,"\tMarco: ");
					string_append(&mensaje, NumeroMarco_Inicial);
				string_append(&mensaje," Pagina: ");
					string_append(&mensaje, NumeroPagina_Inicial);
				if(ptrColitaINICIAL != -1)
				{
					string_append(&mensaje," BitU: ");
						string_append(&mensaje, BitUso_Inicial);	
				}
				string_append(&mensaje," BitM: ");
					string_append(&mensaje, BitMod_Inicial);
				string_append(&mensaje,"\n");
				
				free(NumeroPagina_Inicial);free(NumeroMarco_Inicial);free(BitUso_Inicial);free(BitMod_Inicial);
			queue_push(colitaINICIAL, elementoDeLaCola);
			i++;
		}

		queue_clean(colitaINICIAL);
		queue_destroy(colitaINICIAL);

	// AGREGO LA SEGUNDA COLA:
		string_append(&mensaje,"Estado Final -> \n");

		if(ptrColitaFINAL != -1)
		{
			char * ptr_colitaFinal = string_itoa(ptrColitaFINAL);
			string_append(&mensaje,"Ptr cola Final: ");
				string_append(&mensaje, ptr_colitaFinal);
			string_append(&mensaje,"\n");
			free(ptr_colitaFinal);
		}

		i = 0;
		cantElementos = queue_size(colitaFINAL);
		while(i < cantElementos)
		{
			t_infoDeMarco* elementoDeLaCola = queue_pop(colitaFINAL);
				char * NumeroPagina_Final = string_itoa((*elementoDeLaCola).NumeroPagina);
				char * NumeroMarco_Final = string_itoa((*elementoDeLaCola).NumeroMarco);
				char * BitUso_Final = string_itoa((*elementoDeLaCola).BitUso);
				char * BitMod_Final = string_itoa((*elementoDeLaCola).BitMod);

				string_append(&mensaje,"\tMarco: ");
					string_append(&mensaje, NumeroMarco_Final);
				string_append(&mensaje," Pagina: ");
					string_append(&mensaje, NumeroPagina_Final);
				if(ptrColitaFINAL != -1)
				{
					string_append(&mensaje," BitU: ");
						string_append(&mensaje, BitUso_Final);	
				}
				string_append(&mensaje," BitM: ");
					string_append(&mensaje, BitMod_Final);
				string_append(&mensaje,"\n");
				
				free(NumeroPagina_Final);free(NumeroMarco_Final);free(BitUso_Final);free(BitMod_Final);
			queue_push(colitaFINAL, elementoDeLaCola);
			i++;
		}

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void LOG_senialRecibidad(char* tipo, char* accion)
{
	char * mensaje = string_new();
	string_append(&mensaje," [SENIAL RECIBIDA] ");
		string_append(&mensaje,"Tipo: ");
			string_append(&mensaje, tipo);
		string_append(&mensaje,"Accion a ejecutar: ");
			string_append(&mensaje, accion);
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}

void LOG_senialTratada(char* tipo)
{
	char * mensaje = string_new();
	string_append(&mensaje," [SENIAL TRATADA] ");
		string_append(&mensaje,"Tipo: ");
			string_append(&mensaje, tipo);
		string_append(&mensaje,".\n");

	pthread_mutex_lock(&mutexLogger);
		log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLogger);

	free(mensaje);

	return;
}