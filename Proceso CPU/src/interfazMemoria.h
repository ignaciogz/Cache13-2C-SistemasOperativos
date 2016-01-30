/*
 * interfazMemoria.h
 *
 *  Created on: 17/9/2015
 *      Author: utnso
 */

#ifndef PROCESO_CPU_SRC_INTERFAZMEMORIA_H_
#define PROCESO_CPU_SRC_INTERFAZMEMORIA_H_


typedef struct{
	int offset;
	void* mensaje;
}t_retorno;

int mandarIniciarMemoria(t_conexion conexion,t_datos data, t_retorno* retorno);
void verificarHeader(t_header cabecera,t_conexion conexion, int sock, t_retorno* retorno);
void mandarALeerMem(t_conexion conexion,t_datos data, t_retorno* retorno);
void mandarFinalizarMem(t_conexion conexion,t_datos data, t_retorno* retorno);
void mandarAEscribirMem(t_conexion conexion,t_datos data,char* escribir, int tamanio, t_retorno* retorno);
void avisarPlanifFalloInicio(t_conexion conexion);
void almacenarInicio(t_retorno* retorno);
void almacenarLeido(t_retorno* retorno,int tamanio,char* contenido,t_datos data);
void almacenarEscritura(t_datos data,int tamanio,char* escribir,t_retorno* retorno);
void almacenarEntradaSalida(t_datos data,t_retorno* retorno);
void finRafaga(t_conexion conexion,t_datos data,int contador,t_retorno* retorno);
void finalizado(t_conexion conexion,int contador,t_retorno* retorno);
void almacenarFinalizado(t_retorno* retorno);
void rutaArchivoEquivocada(t_conexion conexion);

#endif /* PROCESO_CPU_SRC_INTERFAZMEMORIA_H_ */
