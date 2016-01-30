/*
 * Consola.h
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_CONSOLA_H_
#define HEADERS_CONSOLA_H_

#include "Structs.h"

void ejecutarComandoCorrer(t_Planificador* planificador, char** comando);

void errorEnComando(t_Planificador* planificador);

void ejecutarComandos(t_Planificador* planificador, char** comando);

void consola(t_Planificador* planificador);

#endif /* HEADERS_CONSOLA_H_ */
