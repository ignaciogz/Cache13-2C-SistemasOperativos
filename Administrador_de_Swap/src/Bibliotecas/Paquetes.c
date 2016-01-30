#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include "Conexiones.h"
#include "Paquetes.h"


t_pagina desempaquetarPagina(t_conexion conexion)
{
	t_pagina paginaNueva;
	size_t longitudDatos;
	recv(conexion.socket,(int*)&paginaNueva.numeroDePagina,sizeof(int),0);
	recv(conexion.socket,(int*)&longitudDatos,sizeof(int),0);
	char* datos = malloc(longitudDatos);
	recv(conexion.socket,datos,longitudDatos,0);
	paginaNueva.datos = datos;
	paginaNueva.inicio = 0;
	return paginaNueva;
}

void desempaquetarmProc(t_conexion conexion,t_mProc* mProcRecibido)
{
	recv(conexion.socket,(int*)&mProcRecibido->pid,sizeof(int),0);
	recv(conexion.socket,(int*)&mProcRecibido->cantidadDePaginas,sizeof(int),0);
}

int desempaquetarPid(t_conexion conexion)
{
	int pid;
	recv(conexion.socket,(int*)&pid,sizeof(int),0);
	return pid;
}
void empaquetarYEnviarContenidoPagina(t_conexion conexion,char* contenido)
{
	int desplazamiento=0;
	int longitudContenido = strlen(contenido)+1;
	int longitudMensaje =  sizeof(int) + longitudContenido;
	void * mensaje = malloc(longitudMensaje);
	memcpy(mensaje + desplazamiento,(int*)&longitudContenido,sizeof(int));
	desplazamiento = sizeof(int);
	memcpy(mensaje + desplazamiento,(char*)contenido,longitudContenido);
	enviarTodo(conexion.socket,mensaje,longitudMensaje);
	free(mensaje);
}

void empaquetarYEnviarmProcRechazado(t_conexion conexion,int pid)
{
	t_cabecera cabecera = mProcRechazado;
	void * mensaje = malloc(sizeof(t_cabecera));
	memcpy(mensaje,(t_cabecera*)&cabecera,sizeof(t_cabecera) );
	enviarTodo(conexion.socket,mensaje,sizeof(t_cabecera));
}

void empaquetarYEnviarmProcExitoso(t_conexion conexion, int pid)
{
	t_cabecera cabecera = mProcAceptado;
	void * mensaje = malloc(sizeof(t_cabecera));
	memcpy(mensaje,(t_cabecera*)&cabecera,sizeof(t_cabecera) );
	enviarTodo(conexion.socket,mensaje,sizeof(t_cabecera));
}

void enviarFinEscritura(t_conexion conexion)
{
	t_cabecera cabecera = finEscritura;
	void * mensaje = malloc(sizeof(t_cabecera));
	memcpy(mensaje,(t_cabecera*)&cabecera,sizeof(t_cabecera) );
	enviarTodo(conexion.socket,mensaje,sizeof(t_cabecera));
}
