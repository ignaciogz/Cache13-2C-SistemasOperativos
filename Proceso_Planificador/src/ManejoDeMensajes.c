/*
 * ManejoDeMensajes.c
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Headers/ManejoDeMensajes.h"
#include "Headers/EnvioYRecepcionDeMensajes.h"
#include "Headers/FuncionesArchivos.h"
#include <pthread.h>
#include <commons/temporal.h>
#include <commons/string.h>

pthread_mutex_t mutexColaCpu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexColaPCB = PTHREAD_MUTEX_INITIALIZER;

void conectarCpu(t_Planificador* planificador, int socketCpu, struct sockaddr_in direccionCpu)
{
	t_CPU* nuevoCPU = malloc(sizeof(t_CPU));

	nuevoCPU->direccion = direccionCpu;
	nuevoCPU->cantidadProcesosAsignados = 0;
	nuevoCPU->colaReady = list_create();
	nuevoCPU->estado = Ocioso;
	nuevoCPU->porcentajeUso = 0;
	nuevoCPU->procesoEnEjecucion = NULL;
	nuevoCPU->socketCpu = socketCpu;


	recv(socketCpu, &nuevoCPU->IDCpu, sizeof(int), 0);

	logearConexionCPU(planificador->logger, nuevoCPU);

	pthread_mutex_lock(&mutexColaCpu);
	list_add(planificador->CPUs, nuevoCPU);
	pthread_mutex_unlock(&mutexColaCpu);

	return;
}

void desconectarCpu(t_Planificador* planificador, int socketCpuDesconectado)
{
	t_CPU* aux;
	int i;

	for(i = 0; i < list_size(planificador->CPUs); i++)
	{
		aux = list_get(planificador->CPUs, i);

		if(aux->socketCpu == socketCpuDesconectado)
		{
			logearDesconexionCPU(planificador->logger, aux);
			list_remove(planificador->CPUs, i);
			free(aux);
			break;
		}
	}

	return;
}

t_CPU* seleccionarCpuConMenosCarga(t_list* listaCPU)
{
	t_CPU* cpuElegido = NULL;
	t_CPU* aux;
	int cantidadDeProcesosDelCpuElegido = -1;
	int i;

	for(i = 0; i < list_size(listaCPU); i++)
	{
		aux = list_get(listaCPU, i);

		if(aux->cantidadProcesosAsignados <= cantidadDeProcesosDelCpuElegido || cantidadDeProcesosDelCpuElegido == -1)
		{
			cantidadDeProcesosDelCpuElegido = aux->cantidadProcesosAsignados;
			cpuElegido = aux;
		}
	}

	return cpuElegido;
}

void correrNuevoProceso(t_Planificador* planificador, char* pathAlArchivo)
{
	t_CPU* cpu;

	char* rutaAlArchivo = string_new();

	string_append(&rutaAlArchivo, planificador->rutaACods);
	string_append(&rutaAlArchivo, pathAlArchivo);

	t_PCB* nuevoProceso = crearProceso(planificador, rutaAlArchivo);

	pthread_mutex_lock(&mutexColaCpu);

	cpu = seleccionarCpuConMenosCarga(planificador->CPUs);

	list_add(cpu->colaReady, nuevoProceso);

	nuevoProceso->IDCpuAsignado = cpu->IDCpu;
	nuevoProceso->estado = LISTO;
	cpu->cantidadProcesosAsignados++;

	logearComienzoEjecucionProceso(planificador->logger, nuevoProceso);

	pthread_mutex_unlock(&mutexColaCpu);
	if(cpu->procesoEnEjecucion == NULL)
		enviarOrdenDeEjecucionACpu(cpu, planificador);



	return;
}

t_PCB* crearProceso(t_Planificador* planificador, char* pathAlArchivo)
{
	t_PCB* nuevoProceso = malloc(sizeof(t_PCB));

	nuevoProceso->estado = NUEVO;
	nuevoProceso->horaComienzoEspera= time(NULL);
	nuevoProceso->punteroProximaInstruccion = 0;
	nuevoProceso->rutaDelArchivo = pathAlArchivo;
	nuevoProceso->PID = obtenerNuevoPid(planificador->PCBs);
	nuevoProceso->horarioInicio = time(NULL);
	nuevoProceso->tiempoEjecucion = 0;
	nuevoProceso->tiempoEspera = 0;

	list_add(planificador->PCBs, nuevoProceso);

	pthread_mutex_unlock(&mutexColaPCB);

	return nuevoProceso;
}

int obtenerNuevoPid(t_list* listaPCBs)
{
	int i = 1;
	int j;

	t_PCB* aux;

	pthread_mutex_lock(&mutexColaPCB);

	if(list_size(listaPCBs) == 0)
		return i;

	while(1)
	{
		for(j = 0; j < list_size(listaPCBs); j++)
		{
			aux = list_get(listaPCBs, j);

			if(aux->PID == i)
				break;

			if(j == (list_size(listaPCBs) -1))
				return i;

		}

		i++;
	}

	return i;
}

t_CPU* obtenerCpuAPartirDelSocket(int socket, t_list* listaDeCpu)
{
	t_CPU* aux;
	int i;
	pthread_mutex_lock(&mutexColaCpu);
	for(i = 0; i < list_size(listaDeCpu); i++)
	{
		aux = list_get(listaDeCpu, i);

		if(aux->socketCpu == socket)
		{
			pthread_mutex_unlock(&mutexColaCpu);
			return aux;
		}
	}
	pthread_mutex_unlock(&mutexColaCpu);

	return NULL;
}


t_CPU* obtenerCpuAPartirDelID(int id, t_list* listaDeCpu)
{
	t_CPU* aux;
	int i;
	pthread_mutex_lock(&mutexColaCpu);
	for(i = 0; i < list_size(listaDeCpu); i++)
	{
		aux = list_get(listaDeCpu, i);

		if(aux->IDCpu == id)
		{
			pthread_mutex_unlock(&mutexColaCpu);
			return aux;
		}
	}
	pthread_mutex_unlock(&mutexColaCpu);

	return NULL;
}

void enviarOrdenDeEjecucionACpu(t_CPU* cpu, t_Planificador* planificador)
{
	t_PCB* procesoAEjecutar;

	pthread_mutex_lock(&mutexColaCpu);
	if(list_size(cpu->colaReady) != 0 && cpu->procesoEnEjecucion == NULL)
	{

		procesoAEjecutar = list_get(cpu->colaReady, 0);
		cpu->procesoEnEjecucion = procesoAEjecutar;
		procesoAEjecutar->estado = EJECUTANDO;
		list_remove(cpu->colaReady, 0);


		logearEjecucionAlgoritmoPlanificacion(planificador, procesoAEjecutar);

		sumarAlProcesoTiempoDeEspera(procesoAEjecutar);

		cpu->procesoEnEjecucion->horaComienzoEjecucion = time(NULL);

		if(planificador->algoritmo == FIFO)
			EnviarPedidoEjecucionSinCorte(cpu, procesoAEjecutar);
		else
			EnviarPedidoEjecucionConCorte(cpu, procesoAEjecutar, planificador->quantum);
	}
	pthread_mutex_unlock(&mutexColaCpu);

	return;
}

void matarProceso(t_Planificador* planificador, int pid)
{
	t_PCB* aux;
	int cantidadProcesos, i, ultimaLinea;

	pthread_mutex_lock(&mutexColaPCB);
	pthread_mutex_lock(&mutexColaCpu);

	cantidadProcesos = list_size(planificador->PCBs);

	for(i = 0; i < cantidadProcesos; i++)
	{
		aux = list_get(planificador->PCBs, i);

		if(aux->PID == pid)
		{
			ultimaLinea = obtenerUltimaLinea(aux->rutaDelArchivo);
			aux->estado = FINALIZADO;
			break;
		}
	}

	aux->punteroProximaInstruccion = ultimaLinea;

	pthread_mutex_unlock(&mutexColaCpu);
	pthread_mutex_unlock(&mutexColaPCB);

	return;
}

void eliminarPCBDeLaLista(int pid, t_list* listaPcb)
{
	t_PCB* aux;
	int i;

	pthread_mutex_lock(&mutexColaPCB);
	for(i = 0; i < list_size(listaPcb); i++)
	{
		aux = list_get(listaPcb, i);
		if(aux->PID == pid)
		{
			list_remove(listaPcb, i);
			free(aux);
			pthread_mutex_unlock(&mutexColaPCB);
			return;
		}
	}
	pthread_mutex_unlock(&mutexColaPCB);

	return;
}

void mostrarEstadoProcesos(t_Planificador* planificador)
{
	t_PCB* aux;
	int cantidadProcesos, i;

	cantidadProcesos = list_size(planificador->PCBs);

	for(i = 0; i < cantidadProcesos; i++)
	{
		aux = list_get(planificador->PCBs, i);

		printf("\n");
		printf("mproc ");
		printf("%s", string_itoa(aux->PID));
		printf(": %s -> ", aux->rutaDelArchivo);

		switch(aux->estado)
		{
		case NUEVO: printf("Nuevo.");
					break;

		case EJECUTANDO: printf("Ejecutando.");
					break;

		case LISTO: printf("Listo.");
					break;

		case BLOQUEADO: printf("Bloqueado.");
					break;

		case FINALIZADO: printf("Finalizado.");
					break;

		}

	}

	return;
}

void mostrarPorcentajesCpu(t_Planificador* planificador)
{
	t_CPU* aux;
	int cantidadCpu, i;

	cantidadCpu = list_size(planificador->CPUs);

	for(i = 0; i < cantidadCpu; i++)
	{
		aux = list_get(planificador->CPUs, i);

		printf("\n");
		printf("CPU ");
		printf("%s", string_itoa(aux->IDCpu));
		printf(": %s%% ", string_itoa(aux->porcentajeUso));

	}

	return;

}

void recibirMensajesDeFinDeRafaga(int socket, t_Planificador* planificador)
{
	char* mensajes = string_new();
	char* mensajeInstruccion;
	bool finProceso = false;

	int cantidadInstruccionesEjecutadas, i, pidProcesoFinalizado;

	t_CPU* cpu = obtenerCpuAPartirDelSocket(socket, planificador->CPUs);

	pthread_mutex_lock(&mutexColaPCB);
	pidProcesoFinalizado = cpu->procesoEnEjecucion->PID;

	recv(socket, &cantidadInstruccionesEjecutadas, sizeof(int), 0);

	if(cpu->procesoEnEjecucion->estado != FINALIZADO)
		cpu->procesoEnEjecucion->punteroProximaInstruccion += cantidadInstruccionesEjecutadas;

	pthread_mutex_unlock(&mutexColaPCB);

	for(i = 0; i < cantidadInstruccionesEjecutadas; i++)
	{
		mensajeInstruccion = recibirYArmarMensajeDeInstruccion(socket, cpu, planificador, &finProceso);
		string_append(&mensajes, mensajeInstruccion);
		free(mensajeInstruccion);
	}

	logearFinalizacionRafaga(planificador->logger, pidProcesoFinalizado, mensajes);

	if(cpu->procesoEnEjecucion != NULL)
		sumarAlProcesoTiempoDeEjecucion(cpu->procesoEnEjecucion);


	if(finProceso)
	{
		logearFinEjecucionProceso(planificador->logger,cpu->procesoEnEjecucion);
		eliminarPCBDeLaLista(cpu->procesoEnEjecucion->PID, planificador->PCBs);
		cpu->procesoEnEjecucion = NULL;
	}

	free(mensajes);


	if(cpu->procesoEnEjecucion == NULL)
		enviarOrdenDeEjecucionACpu(cpu, planificador);
	else
	{
		if(cpu->procesoEnEjecucion->estado != FINALIZADO)
			cpu->procesoEnEjecucion->estado = LISTO;
		list_add(cpu->colaReady, cpu->procesoEnEjecucion);
		cpu->procesoEnEjecucion->horaComienzoEspera = time(NULL);
		cpu->procesoEnEjecucion = NULL;
		enviarOrdenDeEjecucionACpu(cpu, planificador);
	}
	return;
}

char* recibirYArmarMensajeDeInstruccion(int socket, t_CPU* cpu, t_Planificador* planificador, bool* finProceso)
{
	char* mensaje = string_new();

	mensajesCpu cabeceraMensaje;

	t_PCB* aux;

	int numeroPagina, tamanioContenido, tiempo;

	char* contenido;

	recv(socket, &cabeceraMensaje, sizeof(mensajesCpu), 0);

	switch(cabeceraMensaje)
	{
	case 	INICIOOK:	string_append(&mensaje, "mProc ");
						string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
						string_append(&mensaje, " - Iniciado.\n");
						break;

	case	INICIOFALLIDO:	string_append(&mensaje, "mProc ");
							string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
							string_append(&mensaje, " - Fallo.\n");
							eliminarPCBDeLaLista(cpu->procesoEnEjecucion->PID, planificador->PCBs);
							cpu->procesoEnEjecucion = NULL;
							break;

	case	LECTURA:	recv(socket, &numeroPagina, sizeof(int), 0);
						recv(socket, &tamanioContenido, sizeof(int), 0);
						contenido = malloc(tamanioContenido + 1);
						memset(contenido,'\0', tamanioContenido + 1);
						recv(socket, contenido, tamanioContenido, 0);

						string_append(&mensaje, "mProc ");
						string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
						string_append(&mensaje, " - Pagina ");
						string_append(&mensaje, string_itoa(numeroPagina));
						string_append(&mensaje, " leida: ");
						string_append(&mensaje, contenido);
						string_append(&mensaje, ".\n");

						free(contenido);
						break;


	case 	ESCRITURA:	recv(socket, &numeroPagina, sizeof(int), 0);
						recv(socket, &tamanioContenido, sizeof(int), 0);
						contenido = malloc(tamanioContenido + 1);
						memset(contenido,'\0', tamanioContenido + 1);
						recv(socket, contenido, tamanioContenido, 0);

						string_append(&mensaje, "mProc ");
						string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
						string_append(&mensaje, " - Pagina ");
						string_append(&mensaje, string_itoa(numeroPagina));
						string_append(&mensaje, " escrita: ");
						string_append(&mensaje, contenido);
						string_append(&mensaje, ".\n");

						free(contenido);
						break;

	case	ENTRADASALIDA:	recv(socket, &tiempo, sizeof(int), 0);
							string_append(&mensaje, "mProc ");
							string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
							string_append(&mensaje, " - En entrada-salida de tiempo ");
							string_append(&mensaje, string_itoa(tiempo));
							string_append(&mensaje, ".\n");

							cpu->procesoEnEjecucion->estado = BLOQUEADO;
							aux = cpu->procesoEnEjecucion;
							cpu->procesoEnEjecucion = NULL;
							EncolarEntradaSalida(aux, tiempo, planificador);
							break;

	case	FINALIZACIONOK:	string_append(&mensaje, "mProc ");
							string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
							string_append(&mensaje, " Finalizado correctamente.\n");
							*finProceso = true;
							break;

	case	FINALIZACIONFALLIDA:	string_append(&mensaje, "mProc ");
									string_append(&mensaje, string_itoa(cpu->procesoEnEjecucion->PID));
									string_append(&mensaje, " Finalizado incorrectamente.\n");
									*finProceso = true;
									break;

	}

	return mensaje;
}

void sumarAlProcesoTiempoDeEjecucion(t_PCB* proceso)
{
	time_t horaFinEjecucion, diferenciaDeTiempos;

	horaFinEjecucion = time(NULL);

	diferenciaDeTiempos = horaFinEjecucion - proceso->horaComienzoEjecucion;

	proceso->tiempoEspera = proceso->tiempoEspera + diferenciaDeTiempos;

	return;

}

void sumarAlProcesoTiempoDeEspera(t_PCB* proceso)
{
	time_t horaFinEspera, diferenciaDeTiempos;

	horaFinEspera = time(NULL);

	diferenciaDeTiempos = horaFinEspera - proceso->horaComienzoEspera;

	proceso->tiempoEspera = proceso->tiempoEspera + diferenciaDeTiempos;

	return;
}














