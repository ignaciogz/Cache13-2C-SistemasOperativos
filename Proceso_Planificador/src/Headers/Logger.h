/*
 * Logger.h
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_LOGGER_H_
#define HEADERS_LOGGER_H_

/*
 * Logger.c
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#include <commons/log.h>
#include <commons/string.h>
#include "Structs.h"
#include "Entrada-Salida.h"

//Loggea el comienzo de ejecucion de un proceso indicando PID y nombre
void logearComienzoEjecucionProceso(t_log* logger, t_PCB* proceso);

//Loggea el fin de ejecucion de un proceso indicando PID y nombre
void logearFinEjecucionProceso(t_log* logger, t_PCB* proceso);

//Loggea la conexion de un CPU indicando su ID
void logearConexionCPU(t_log* logger, t_CPU* cpu);

//Loggea la Desconexion de un CPU indicando su ID
void logearDesconexionCPU(t_log* logger, t_CPU* cpu);

//Logea la ejecucion del algoritmo de planificacion indicando que algoritmo es, el proceso seleccionado para ejecutar
//y el orden de las colas ordenadas
void logearEjecucionAlgoritmoPlanificacion(t_Planificador* planificador, t_PCB* proceso);

//Logea la finalizacion de una rafaga de ejecucion de un proceso indicando que proceso es y los mensajes recibidos
void logearFinalizacionRafaga(t_log* logger, int pidProceso, char* MensajesEjecucion);


#endif /* HEADERS_LOGGER_H_ */
