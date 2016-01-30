/*
 * sockets.h
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#ifndef PROCESO_CPU_SRC_SOCKETS_H_
#define PROCESO_CPU_SRC_SOCKETS_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <commons/log.h>
#include <commons/config.h>

typedef enum{
	//Con planificador
	ConexionCpuPlanif = 0,//conexion planif inicialmente
	CorrerProgramaSinCorte = 3,//planificador me envia a correr un programa
	CorrerProgramaConCorte = 4,
	Entrada_Salida = 2,//borrar
	PorcentajeCpu =50,

	//Con memoria
	ConexionCpuMem = 5,//borrar
	InicioPrograma = 20,//Le envio a memoria a q inicie sus estructuras
	InicioProgramaOK = 24,//Memoria pudo crear sus estructuras
	InicioProgramaFallo = 25,
	EjecutarInstruccion = 9,//borrar
	RespuestaInstruccion =10 ,//borrar
	Lectura=21,
	Escritura=23,
	Finalizar=1,
	FinalizarOK
}t_header;

typedef enum{
	Iniciado=0,
	Fallo=1,
	Leido=2,
	Escrito=3,
	EntradaSalida=4,
	Finalizado=5,
	RutaEquivocada=6
}t_operaciones;


typedef struct{
	int tiempoInstruccion;
	double porcentajeUltMinuto;
}t_cpu;


typedef struct{
	int i;//socket q acepte conexion
	struct sockaddr_in planifAddr;
	struct sockaddr_in memAddr;
	int ID;
	t_cpu* cpu;
}t_conexion;

typedef struct{
	double retardo;
	int ID;
	int puntero;
	int PID;
	int quantum;
	int valor;
	t_log* logger;
}t_datos;

typedef struct{
	t_config * config;
	t_log* logger;
}t_documentacion;




struct sockaddr_in inicializarDireccion(int puerto, char* ip);
void packHeader(t_header header, void* mensaje);
void umpackHeader(t_header* header, void* package);
int sendall(int s, char* buf, int* len);
void mostrarIdHilo( int ID );
void packEscritura(t_datos data,int tamanio, char* contenido, void* mensaje);


#endif /* PROCESO_CPU_SRC_SOCKETS_H_ */
