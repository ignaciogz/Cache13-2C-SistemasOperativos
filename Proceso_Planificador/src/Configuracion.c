/*
 * Configuracion.c
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/Configuracion.h"
#include <commons/string.h>

char* obtenerParametroString(t_config* configuracion, char* key)
{
	char* parametro;
	if(config_has_property(configuracion, key))
	{
		parametro = config_get_string_value(configuracion, key);

		if(parametro == NULL)
		{
			printf("ERROR: Archivo de configuracion incompleto\n");
			exit(1);
		}

		return parametro;
	}
	else{
		printf("ERROR: Archivo de configuracion incompleto\n");
		exit(1);
	}
}

int obtenerParametroInt(t_config* configuracion, char* key)
{
	int parametro;

	if(config_has_property(configuracion, key))
	{
		parametro = config_get_int_value(configuracion, key);

		return parametro;
	}
	else{
		printf("ERROR: Archivo de configuracion incompleto\n");
		exit(1);
	}
}

t_Planificador inicializarPlanificador(char* archivoConfig, char* archivoLog)
{
	t_Planificador planificador;
	algoritmosPlanificacion algoritmo;

	t_config* configuracion = config_create(archivoConfig);

	if(configuracion == NULL)
	{
		printf("ERROR: no se pudo crear el archivo de configuracion\n");
		exit(1);
	}

	char* ip = obtenerParametroString(configuracion, "IP_LISTEN");
	char* algoritmoPlanificacion = obtenerParametroString(configuracion, "ALGORITMO");
	int quantum = obtenerParametroInt(configuracion, "QUANTUM");
	short int puerto = obtenerParametroInt(configuracion, "PUERTO_LISTEN");

	if(strcmp(algoritmoPlanificacion, "FIFO") == 0)
		algoritmo = FIFO;
	else
	{
		if(strcmp(algoritmoPlanificacion, "RR") == 0)
			algoritmo = RR;
		else
		{
			printf("ERROR: Algoritmo especificado invalido\n");
			exit(1);
		}
	}

	printf("=====================================\n");
	printf("   PROCESO PLANIFICADOR INICIADO\n");
	printf("=====================================\n");
	printf("\n");
	printf("Direccion ip: ");
	printf("%s", ip);
	printf("\n");
	printf("Puerto: ");
	printf("%s", string_itoa(puerto));
	printf("\n");
	printf("Algoritmo de planificacion: ");
	printf("%s", algoritmoPlanificacion);
	if(algoritmo == RR)
	{
		printf("  Con quantum: ");
		printf("%s", string_itoa(quantum));
	}
	printf("\n");
	printf("\n");


	planificador.CPUs = list_create();
	planificador.PCBs = list_create();
	planificador.EntradaSalida = list_create();
	planificador.logger = log_create(archivoLog, "Proceso Planificador", false, LOG_LEVEL_INFO);
	planificador.quantum = quantum;
	planificador.algoritmo = algoritmo;
	planificador.direccion.sin_port = htons(puerto);
	inet_aton(ip, &planificador.direccion.sin_addr);
	planificador.direccion.sin_family = AF_INET;
	memset(planificador.direccion.sin_zero, '\0', 8);


	return planificador;
}
