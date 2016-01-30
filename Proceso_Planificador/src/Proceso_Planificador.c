/*
 ============================================================================
 Name        : Proceso_Planificador.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Headers/Structs.h"
#include "Headers/Logger.h"
#include "Headers/Configuracion.h"
#include "Headers/EnvioYRecepcionDeMensajes.h"

int main(int argc,char* argv[] ) {

	char* rutaArchivoConfiguracion = "../Archivos/configuracionProcesoPlanificador.cfg";
	char* rutaArchivoLogger = "../Archivos/LoggerProcesoPlanificador.txt";

	t_Planificador planificador = inicializarPlanificador(rutaArchivoConfiguracion, rutaArchivoLogger);

	planificador.rutaACods = argv[1];

	//planificador.rutaACods = "/home/utnso/tp/tp-2015-2c-404-name-not-found/mcodsChk3/cache13-scripts/";

	aceptarConexionesYMensajes(&planificador);

return 0;
}
