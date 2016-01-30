/*
 * ManejoMensajes.h
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_MANEJODEMENSAJES_H_
#define HEADERS_MANEJODEMENSAJES_H_

#include "Structs.h"
#include "Logger.h"
#include "Entrada-Salida.h"

//Agrega un nuevo cpu a la lista de cpus y logea la conexion
void conectarCpu(t_Planificador* planificador, int socketCpu, struct sockaddr_in direccionCpu);

//Quita un cpu de la lista de cpus y logea la desconexion, replanifica la cola del cpu
void desconectarCpu(t_Planificador* planificador, int socketCpuDesconectado);

//Selecciona el cpu con menor cantidad de procesos asignados
t_CPU* seleccionarCpuConMenosCarga(t_list* listaCPU);

void enviarOrdenDeEjecucionACpu(t_CPU* cpu, t_Planificador* planificador);

//Crea un nuevo proceso, selecciona el cpu con menos carga para asignarlo y lo agrega a su cola de listos
void correrNuevoProceso(t_Planificador* planificador, char* pathAlArchivo);

//crea un nuevo proceso
t_PCB* crearProceso(t_Planificador* planificador, char* pathAlArchivo);

//Devuelve el menor numero disponible para usar como PID
int obtenerNuevoPid(t_list* listaPCBs);

//Envia al cpu pasado por parametro la orden de ejecutar el primer proceso que se encuentre en la cola de listos
void enviarOrdenDeEjecucionACpu(t_CPU* cpu, t_Planificador* planificador);

void matarProceso(t_Planificador* planificador, int pid);

void mostrarEstadoProcesos(t_Planificador* planificador);

void mostrarPorcentajesCpu(t_Planificador* planificador);

char* recibirYArmarMensajeDeInstruccion(int socket, t_CPU* cpu, t_Planificador* planificador, bool* finEjecucion);

t_CPU* obtenerCpuAPartirDelSocket(int socket, t_list* listaDeCpu);

t_CPU* obtenerCpuAPartirDelID(int id, t_list* listaDeCpu);

void eliminarPCBDeLaLista(int pid, t_list* listaPcb);

void recibirMensajesDeFinDeRafaga(int socket, t_Planificador* planificador);

char* calcularDiferenciaDeTiempo(char** horaInicial, char** horaFinal);

char* sumarHoras (char** hora1, char** hora2);

void sumarAlProcesoTiempoDeEjecucion(t_PCB* proceso);

void sumarAlProcesoTiempoDeEspera(t_PCB* proceso);




#endif /* HEADERS_MANEJODEMENSAJES_H_ */
