/*
 * configuracion.h
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#ifndef PROCESO_CPU_SRC_CONFIGURACION_H_
#define PROCESO_CPU_SRC_CONFIGURACION_H_
#include <commons/config.h>
/*Dado un mensaje de error lo muestra por pantalla y termina el proceso MaRTA*/

void terminarProceso(char* MensajeError);


/*Dado una estructura de tipo configuracion y una palabra clave, devuelve el valor (de tipo entero) relacionado a dicha clave. En caso de no encontrarse
  el valor o la clave devuelve un mensaje de error y termina el proceso*/

int obtener_int(t_config* configuracion, char* clave);


/*Dado una estructura de tipo configuracion y una palabra clave, devuelve el valor (de tipo char*) relacionado a dicha clave. En caso de no encontrarse el
 valor o la clave devuelve un mensaje de error y termina el proceso*/

char* obtener_string(t_config* configuracion, char* clave);

double obtener_double(t_config* configuracion, char* clave);



#endif /* PROCESO_CPU_SRC_CONFIGURACION_H_ */
