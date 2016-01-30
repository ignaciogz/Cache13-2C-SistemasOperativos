/*
 * Consola.c
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/Consola.h"
#include "Headers/ManejoDeMensajes.h"

void ejecutarComandoCorrer(t_Planificador* planificador, char** comando)
{
	if(comando[1] == NULL || comando[2] != NULL)
	{
		free(comando);
		errorEnComando(planificador);
	}

	if(list_size(planificador->CPUs) == 0)
	{
		printf("\nNo hay CPUs conectados para aceptar el pedido.");
		printf("\n");
		free(comando);
		consola(planificador);
	}

	char* rutaAlArchivo = string_new();

	string_append(&rutaAlArchivo, comando[1]);

	int posicionSaltoLinea = strlen(rutaAlArchivo) - 1;

	rutaAlArchivo[posicionSaltoLinea] = '\0';

	correrNuevoProceso(planificador, rutaAlArchivo);

	free(comando);
	printf("\n");
	consola(planificador);

	return;
}

void errorEnComando(t_Planificador* planificador)
{
	printf("\nError en el comando ingresado por favor ingrese un comando valido o ingrese el comando ayuda para obtener una"
			" lista de comandos validos");
	printf("\n");
	consola(planificador);

	return;
}

void ejecutarComandoFinalizar(t_Planificador* planificador, char** comando)
{
	if(comando[1] == NULL || comando[2] != NULL)
	{
		free(comando);
		errorEnComando(planificador);
	}

	int posicionSaltoLinea = strlen(comando[1]) - 1;

	comando[1][posicionSaltoLinea] = '\0';

	int pid = atoi(comando[1]);

	matarProceso(planificador, pid);

	free(comando);

	consola(planificador);
	printf("\n");
	return;
}

void ejecutarComandoPs(t_Planificador* planificador, char** comando)
{
	if(comando[1] != NULL)
	{
		free(comando);
		errorEnComando(planificador);
	}

	mostrarEstadoProcesos(planificador);

	free(comando);
	printf("\n");
	consola(planificador);

	return;
}

void ejecutarComandoCpu(t_Planificador* planificador, char** comando)
{
	if(comando[1] != NULL)
	{
		free(comando);
		errorEnComando(planificador);
	}

	mostrarPorcentajesCpu(planificador);

	free(comando);
	printf("\n");

	consola(planificador);

	return;
}

void ejecutarComandos(t_Planificador* planificador, char** comando)
{
	if(strcmp(comando[0], "correr") == 0)
		ejecutarComandoCorrer(planificador, comando);

	if(strcmp(comando[0], "finalizar") == 0)
		ejecutarComandoFinalizar(planificador, comando);

	int posicionSaltoLinea = strlen(comando[0]) - 1;

	comando[0][posicionSaltoLinea] = '\0';

	if(strcmp(comando[0], "ps") == 0)
		ejecutarComandoPs( planificador, comando);


	if(strcmp(comando[0], "cpu") == 0)
		ejecutarComandoCpu(planificador, comando);

	errorEnComando(planificador);

	free(comando);
	return;
}

void consola(t_Planificador* planificador)
{
	char* comando = malloc(100);

	size_t tamanio = 100;

	printf("Proceso planificador >>");

	getline(&comando, &tamanio, stdin);

	char** comandoSplit = string_split(comando," ");

	free(comando);

	ejecutarComandos(planificador, comandoSplit);

	return;
}



