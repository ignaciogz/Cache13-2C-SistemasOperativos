/*
 * Paginacion.h
 *
 *  Created on: 13/9/2015
 *      Author: utnso
 */

#ifndef BIBLIOTECAS_PAGINACION_H_
#define BIBLIOTECAS_PAGINACION_H_

#include <commons/collections/list.h>


typedef struct{
	int numeroDePagina;
	int inicio;
	char* datos;
}t_pagina;


// dados el numero de una pagina y el tamanio por pagina, devuelve el byte inicial de dicha pagina
int calcularByteInicial(int numeroDePagina,int tamanioDePagina);

// Dados un byte inicial y el tamanio por pagina, devuelve el numero de pagina al que corresponde
int obtenerNumeroDePagina(int byteInicial,int tamanioPagina);

// dadas una lista con numeros de pagina y un numero de pagina devuelve si la pagina esta o no en la lista
bool estaEnLaLista(t_list* listaPaginas,int pagina);

// dado un char* devuelve un char* del tamanio de una pagina que posee los datos y el resto completado con '/0' hasta llega al tamanio de la pagina
char* adaptarContenidoAPagina(char* datos,int tamanioDePagina);

#endif /* BIBLIOTECAS_PAGINACION_H_ */
