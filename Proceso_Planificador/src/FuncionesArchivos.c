/*
 * FuncionesArchivos.c
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int obtenerUltimaLinea(char* rutaAlArchivo)
{
	int ultimaLinea = 0;

	FILE* fd = fopen(rutaAlArchivo,"r");

	size_t tamanioLinea = 100;

	char* linea = malloc(tamanioLinea);


	while(getline(&linea, &tamanioLinea, fd) != EOF)
		ultimaLinea++;

	fclose(fd);

	return ultimaLinea - 1;
}
