/*
 * Configuracion.h
 *
 *  Created on: 3/9/2015
 *      Author: utnso
 */

#ifndef HEADERS_CONFIGURACION_H_
#define HEADERS_CONFIGURACION_H_

#include <commons/config.h>
#include "Structs.h"

//Funcion que obtiene un string desde el archivo de configuracion
//En caso de no encontrar la key muestra un error por pantalla y termina la ejecucion del proceso
char* obtenerParametroString(t_config* configuracion, char* key);

//Funcion que obtiene un int desde el archivo de configuracion
//En caso de no encontrar la key muestra un error por pantalla y termina la ejecucion del proceso
int obtenerParametroInt(t_config* configuracion, char* key);

//Inicializa la estructura del planificador a traves del archivo de configuracion y ademas crea el log
t_Planificador inicializarPlanificador(char* archivoConfig, char* archivoLog);

#endif /* HEADERS_CONFIGURACION_H_ */
