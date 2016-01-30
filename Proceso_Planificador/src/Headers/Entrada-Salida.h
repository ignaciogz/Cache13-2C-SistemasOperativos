/*
 * Entrada-Salida.h
 *
 *  Created on: 25/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_ENTRADA_SALIDA_H_
#define HEADERS_ENTRADA_SALIDA_H_

#include <commons/collections/list.h>
#include "Structs.h"
#include "ManejoDeMensajes.h"


void ejecutarEntradaSalida(t_Planificador* planificador);

void EncolarEntradaSalida(t_PCB* proceso, int tiempo, t_Planificador* planificador);

char* armarMensajeColaEntradaSalida(t_list* listaEntradaSalida);

#endif /* HEADERS_ENTRADA_SALIDA_H_ */
