/*
 * manejoSolicitudes.h
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#ifndef PROCESO_CPU_SRC_MANEJOSOLICITUDES_H_
#define PROCESO_CPU_SRC_MANEJOSOLICITUDES_H_

#include <arpa/inet.h>
#include "interfazMemoria.h"


void ejecutarSolicitud(struct sockaddr_in acceptAddr,t_header header,t_conexion conexion,t_datos data);
void consultarMem(int socketAccept,struct sockaddr_in  acceptAddr);
void finalizarPrograma(int socketAccept,struct sockaddr_in  acceptAddr);
void avisarFinPlanif(t_conexion conexion);
void ejecutarPrograma(t_conexion conexion,int tamanio,char* nombre,t_datos data);
char* vaciarCadena(char* cadena);
void acomodarPuntero(int puntero,FILE* fichero);


#endif /* PROCESO_CPU_SRC_MANEJOSOLICITUDES_H_ */
