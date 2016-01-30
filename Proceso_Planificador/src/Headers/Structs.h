/*
 * Structs.h
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_STRUCTS_H_
#define HEADERS_STRUCTS_H_

#include <commons/collections/list.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

typedef enum{CONEXIONCPU = 0,
	FINRAFAGA = 1,
	FINRAFAGAPORENTRADASALIDA = 2,
	ENVIOPROCESOCONCORTE = 4,
	ENVIOPROCESOSINCORTE = 3,
	PORCENTAJEUSO = 50
} tipoMensaje;

typedef enum{INICIOOK = 0,
			INICIOFALLIDO = 1,
			LECTURA = 2,
			ESCRITURA = 3,
			ENTRADASALIDA = 4,
			FINALIZACIONOK = 5,
			FINALIZACIONFALLIDA = 6,
}mensajesCpu;

typedef enum{NUEVO, LISTO, BLOQUEADO, EJECUTANDO, FINALIZADO} estadosDeProceso;

typedef enum{Ejecutando, Ocioso} estadosCpu;

typedef enum{FIFO, RR} algoritmosPlanificacion;

typedef struct{
	int PID;
	char* rutaDelArchivo;
	time_t horarioInicio;
	time_t horaComienzoEjecucion;
	time_t horaComienzoEspera;
	time_t tiempoEjecucion;
	time_t tiempoEspera;
	int punteroProximaInstruccion;
	estadosDeProceso estado;
	int IDCpuAsignado;
}t_PCB;

typedef struct{
	int IDCpu;
	t_list* colaReady;
	estadosCpu estado;
	struct sockaddr_in direccion;
	int porcentajeUso;
	int cantidadProcesosAsignados;
	t_PCB* procesoEnEjecucion;
	int socketCpu;
}t_CPU;

typedef struct{
	algoritmosPlanificacion algoritmo;
	int quantum;
	struct sockaddr_in direccion;
	t_list* PCBs;
	t_list* CPUs;
	t_list* EntradaSalida;
	t_log* logger;
	char* rutaACods;
}t_Planificador;

typedef struct{
	t_Planificador* planificador;
	int socket;
	struct sockaddr_in direccion;
	tipoMensaje header;
}t_dataHilo;

typedef struct{
	t_PCB* proceso;
	int tiempo;
}t_entradaSalida;

#endif /* HEADERS_STRUCTS_H_ */
