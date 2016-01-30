/*
 * interfazMemoria.c
 *
 *  Created on: 17/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "sockets.h"
#include "interfazMemoria.h"
#include "Log.h"

int mandarIniciarMemoria(t_conexion conexion,t_datos data, t_retorno* retorno){
	int sockMem;
	int resultado;
	if((sockMem = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				perror("socket");
				exit(1);
			}

	if(connect(sockMem, (struct sockaddr *)&conexion.memAddr, sizeof(struct sockaddr_in)) == -1)
			{
				perror("connect");
				//logear no poder conectarse con proceso y finalizar proceso Cpu
				exit(1);
			}
	t_header cabecera = InicioPrograma;
	int tam = sizeof(t_header)+sizeof(int)+sizeof(int)+ sizeof(int);
	void* mensaje = malloc(tam);
	int offset = sizeof(t_header);
	packHeader(cabecera,mensaje);
	memcpy(mensaje + offset, (int*) &data.PID, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,(int*)&data.valor , sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,(int*)&data.ID , sizeof(int));

	if((sendall(sockMem,mensaje,&tam))==-1){
		printf("Se desconecto la memoria");
	}

	free(mensaje);

	recv(sockMem,(t_header*)&cabecera,sizeof(int),0);
	resultado = cabecera;
	logearInicio(data,cabecera);
	verificarHeader(cabecera,conexion,sockMem,retorno);

return resultado;
}

void verificarHeader(t_header cabecera,t_conexion conexion, int sockMem, t_retorno* retorno){
	switch(cabecera){
	case InicioProgramaOK: 	almacenarInicio(retorno);
							break;
	case InicioProgramaFallo:	avisarPlanifFalloInicio(conexion);
								printf("Fallo el inicio de estructuras de la memoria");
								break;
	default:	break;
	}
	close(sockMem);

	return;
}

void mandarALeerMem(t_conexion conexion,t_datos data, t_retorno* retorno){
	int sockMem;
	if((sockMem = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(connect(sockMem, (struct sockaddr *)&conexion.memAddr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("connect");
		//logear no poder conectarse con proceso y finalizar proceso Cpu
		exit(1);
	}
	t_header cabecera = Lectura;
	int tam = sizeof(t_header)+sizeof(int)+sizeof(int)+ sizeof(int);
	void* mensaje = malloc(tam);
	int offset = sizeof(t_header);
	packHeader(cabecera,mensaje);
	memcpy(mensaje + offset, (int*) &data.PID, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,(int*)&data.valor , sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,(int*)&data.ID , sizeof(int));

	if((sendall(sockMem,mensaje,&tam))==-1){
		printf("Se desconecto la memoria");
	}

	//Recibo el resultado de la lectura de memoria
	int tamanio;
	char* contenido;
	recv(sockMem,(int*)&tamanio,sizeof(int),0);
	contenido = malloc(tamanio);
	recv(sockMem,contenido,tamanio,0);
	logearLecturaEscritura(data,contenido,"Lectura mProc:");
	almacenarLeido(retorno,tamanio,contenido,data);

return;
}

void mandarFinalizarMem(t_conexion conexion,t_datos data, t_retorno* retorno){
	logearFinalizomProc(data);
	logearFinalizoRafagamProc(data);
	int sockMem;
	if((sockMem = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(connect(sockMem, (struct sockaddr *)&conexion.memAddr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("connect");
		//logear no poder conectarse con proceso y finalizar proceso Cpu
		exit(1);
	}
	t_header cabecera = Finalizar;
	int tam = sizeof(t_header)+sizeof(int)+ sizeof(int);
	void* mensaje = malloc(tam);
	int offset = sizeof(t_header);
	packHeader(cabecera,mensaje);
	memcpy(mensaje + offset, (int*) &data.PID, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,(int*)&data.ID , sizeof(int));

	if((sendall(sockMem,mensaje,&tam))==-1){
		printf("Se desconecto la memoria");
	}
	close(sockMem);

	almacenarFinalizado(retorno);
	return;
}
void avisarPlanifFalloInicio(t_conexion conexion){
		t_header cabecera = Finalizar;
		t_operaciones operacion = Fallo;
		int instruccion = 1;
		int tam = sizeof(t_header)+sizeof(t_operaciones)+sizeof(int);
		void* mensaje = malloc(tam);
		packHeader(cabecera,mensaje);
		int offset = sizeof(t_header);
		memcpy(mensaje + offset, (int*) &instruccion, sizeof(int));
		offset =offset + sizeof(int);
		memcpy(mensaje + offset, (t_operaciones*) &operacion, sizeof(t_operaciones));


		if((sendall(conexion.i,mensaje,&tam))==-1){
			printf("Se desconecto Planif");
		}

	return;
}

void mandarAEscribirMem(t_conexion conexion,t_datos data,char* escribir, int tamanio, t_retorno* retorno){
	int sockMem;
	if((sockMem = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(connect(sockMem, (struct sockaddr *)&conexion.memAddr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("connect");
		//logear no poder conectarse con proceso y finalizar proceso Cpu
		exit(1);
	}
	t_header cabecera = Escritura;
	int tam = sizeof(t_header)+sizeof(int)+sizeof(int)+sizeof(int)+tamanio+sizeof(int);
	void* mensaje = malloc(tam);
	packHeader(cabecera,mensaje);
	packEscritura(data,tamanio,escribir,mensaje);


	if((sendall(sockMem,mensaje,&tam))==-1){
		printf("Se desconecto la memoria");
	}

	t_header evitarCagadas;

	logearLecturaEscritura(data,escribir,"Escritura mProc:");
	almacenarEscritura(data,tamanio,escribir,retorno);
	recv(sockMem,&evitarCagadas,sizeof(t_header),0);

	close(sockMem);
	return;
}

void almacenarInicio(t_retorno* retorno){
	int ofset = retorno->offset;

	retorno->mensaje = malloc(sizeof(t_operaciones));

	t_operaciones operacion= Iniciado;
	memcpy(retorno->mensaje,(t_operaciones*)&operacion, sizeof(t_operaciones));
	ofset = sizeof(t_operaciones);
	retorno->offset = ofset;


	return;
}

void almacenarLeido(t_retorno* retorno,int tamanio,char* contenido,t_datos data){
	int ofset = retorno->offset;
	t_operaciones operacion = Leido;
	void* message;
	message = realloc(retorno->mensaje,retorno->offset + sizeof(t_operaciones) + sizeof(int)+ sizeof(int) + tamanio);
	retorno->mensaje = message;
	memcpy(retorno->mensaje + ofset,(t_operaciones*)&operacion,sizeof(t_operaciones));
	ofset = ofset + sizeof(t_operaciones);
	memcpy(retorno->mensaje + ofset,(int*)&data.valor,sizeof(int));
	ofset = ofset + sizeof(int);
	memcpy(retorno->mensaje + ofset,(int*)&tamanio, sizeof(int));
	ofset =ofset + sizeof(int);
	memcpy(retorno->mensaje + ofset,contenido, tamanio);
	ofset =ofset + tamanio;
	retorno->offset = ofset;

	return;
}
void almacenarEscritura(t_datos data,int tamanio,char* escribir,t_retorno* retorno){
	int ofset = retorno->offset;
	t_operaciones operacion= Escrito;
	void* message;
	int tam = sizeof(t_operaciones) + sizeof(int) + sizeof(int) + tamanio;
	message = realloc(retorno->mensaje,retorno->offset + tam);
	retorno->mensaje = message;
	memcpy(retorno->mensaje + ofset,(t_operaciones*)&operacion, sizeof(t_operaciones));
	ofset =ofset + sizeof(t_operaciones);
	memcpy(retorno->mensaje + ofset,(int*)&data.valor, sizeof(int));
	ofset =ofset + sizeof(int);
	memcpy(retorno->mensaje + ofset,(int*)&tamanio, sizeof(int));
	ofset =ofset + sizeof(int);
	memcpy(retorno->mensaje + ofset,escribir, tamanio);
	ofset =ofset + tamanio;
	retorno->offset = ofset;

	return;
}

void finRafaga(t_conexion conexion,t_datos data,int contador,t_retorno* retorno){
	almacenarEntradaSalida(data,retorno);
	logearFinalizoRafagamProc(data);
	logearEntradaSalida(data);
	int tam = sizeof(t_header)+sizeof(int)+ retorno->offset;
	void* mensaje = malloc(tam);
	int offset;
	t_header cabecera = Finalizar;
	memcpy(mensaje,(t_header*)&cabecera,sizeof(t_header));
	offset = sizeof(t_header);
	memcpy(mensaje + offset,(int*)&contador,sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,retorno->mensaje,retorno->offset);

	if((sendall(conexion.i,mensaje,&tam))==-1){
		printf("Se desconecto el planif");
	}


	return;
}

void almacenarEntradaSalida(t_datos data,t_retorno* retorno){
	int ofset = retorno->offset;
	t_operaciones operacion=EntradaSalida;
	void* message;
	message = realloc(retorno->mensaje,retorno->offset + sizeof(int) +  sizeof(t_operaciones));
	retorno->mensaje = message;
	memcpy(retorno->mensaje + ofset,(t_operaciones*)&operacion, sizeof(t_operaciones));
	ofset =ofset + sizeof(t_operaciones);
	memcpy(retorno->mensaje + ofset,(int*)&data.valor, sizeof(int));
	ofset =ofset + sizeof(int);
	retorno->offset = ofset;

	return;
}

void finalizado(t_conexion conexion,int contador,t_retorno* retorno){
	int tam = sizeof(t_header)+sizeof(int)+ retorno->offset;
	void* mensaje = malloc(tam);
	int offset;
	t_header cabecera = Finalizar;
	memcpy(mensaje,(t_header*)&cabecera,sizeof(t_header));
	offset = sizeof(t_header);
	memcpy(mensaje + offset,(int*)&contador,sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset,retorno->mensaje,retorno->offset);

	if((sendall(conexion.i,mensaje,&tam))==-1){
		printf("Se desconecto el planif");
	}


	return;
}

void almacenarFinalizado(t_retorno* retorno){
	int ofset = retorno->offset;
	t_operaciones operacion= Finalizado;
	void* message;
	int tam = sizeof(t_operaciones);
	message = realloc(retorno->mensaje,retorno->offset + tam);
	retorno->mensaje = message;
	memcpy(retorno->mensaje + ofset,(t_operaciones*)&operacion, sizeof(t_operaciones));
	ofset =ofset + sizeof(t_operaciones);
	retorno->offset = ofset;

	return;
}


void rutaArchivoEquivocada(t_conexion conexion){
	int tam = sizeof(t_header)+ sizeof(t_operaciones);
	void* mensaje = malloc(tam);
	int offset;
	t_header cabecera = Finalizar;
	memcpy(mensaje,(t_header*)&cabecera,sizeof(t_header));
	offset = sizeof(t_header);
	t_operaciones operaciones = RutaEquivocada;
	memcpy(mensaje + offset,(t_operaciones*)&operaciones,sizeof(t_operaciones));

	if((sendall(conexion.i,mensaje,&tam))==-1){
			printf("Se desconecto el planif");
		}

	return;
}
