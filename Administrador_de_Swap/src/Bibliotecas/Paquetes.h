#ifndef BIBLIOTECAS_PAQUETES_H_
#define BIBLIOTECAS_PAQUETES_H_

#include "ManejoSwap.h"
#include "Conexiones.h"



/*************
DESEMPAQUETADO
*************/

// dado un paquete sin el header que solo contiene un numero de pagina, retorna dicho numero de pagina
t_pagina desempaquetarPagina(t_conexion conexion);

// dados una conexion con un paquete sin el header que contiene el pid y la cantidad de paginas que ocupa  y un puntero a un mProc, agrega al mProc el pid y la cantidad de paginas
void desempaquetarmProc(t_conexion conexion,t_mProc* mProcRecibido);

// dados una conexion con un paquete sin el header que contiene el pid y un puntero a int, agrega al contenido del puntero a int, el valor obtenido de la conexion
int desempaquetarPid(t_conexion conexion);


/**********
EMPAQUETADO
**********/

/*Dados una conexion, un numero de pagina y el contenido de una pagina, crea un paquete con el correspondiente header (ver el .md para mas informacion) y lo envia
Ademas modifica el puntero longitudMensaje con la longitud del mensaje creado.*/
void empaquetarYEnviarContenidoPagina(t_conexion conexion,char* contenido);

// Crea el paquete correspondiente (ver mensajes.md) para informarle al administrador de memoria que un mproc fue rechazado
void empaquetarYEnviarmProcRechazado(t_conexion conexion,int pid);

// Dadas una conexion y un pid, crea un paquete de mProc aceptado(ver .md) y lo envia al socket de esa conexion
void empaquetarYEnviarmProcExitoso(t_conexion conexion, int pid);

void enviarFinEscritura(t_conexion conexion);
#endif /* BIBLIOTECAS_PAQUETES_H_ */
