/*
 * Log.h
 *
 *  Created on: 16/10/2015
 *      Author: utnso
 */

#ifndef LOG_H_
#define LOG_H_

void logearCreacionCpu(t_datos data);
void logearConexion(t_datos data, char* proceso,char* estado);
void logearContextoEjecucion(t_datos data,char* ruta);
void logearInicio(t_datos data, t_header cabecera);
void logearLecturaEscritura(t_datos data, char* contenido,char* instruccion);
void logearEntradaSalida(t_datos data);
void logearFinalizomProc(t_datos data);
void logearFinalizoRafagamProc(t_datos data);

#endif /* LOG_H_ */
