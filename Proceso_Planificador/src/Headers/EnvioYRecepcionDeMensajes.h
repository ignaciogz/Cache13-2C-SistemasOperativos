/*
 * EnvioYRecepcionDeMensajes.h
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_ENVIOYRECEPCIONDEMENSAJES_H_
#define HEADERS_ENVIOYRECEPCIONDEMENSAJES_H_

#include "ManejoDeMensajes.h"

//Acepta los mensajes entratrantes al socket listener y de los sockets de los cpu
void aceptarConexionesYMensajes(t_Planificador* planificador);

//Recibe el header del mensaje y ejecuta la funcion apropiada
void recibirMensaje(t_dataHilo* data);

//Obtiene el maximo file descriptor entre los cpu + 1 para usar en el select
int obtenerMaxFD(t_list* listaCPU);

//Envia al cpu la orden de ejecutar el proceso asociado a la pcb sin condicion de corte
void EnviarPedidoEjecucionSinCorte(t_CPU* cpu, t_PCB* pcb);

//Envia al cpu la orden de ejecutar el proceso asociado a la pcb con el valor del quantum a ejecutar
void EnviarPedidoEjecucionConCorte(t_CPU* cpu, t_PCB* pcb, int quantum);

//Envia el mensaje entero
void EnviarTodo(int socketAEnviar, char* mensaje, int largoMensaje);

#endif /* HEADERS_ENVIOYRECEPCIONDEMENSAJES_H_ */
