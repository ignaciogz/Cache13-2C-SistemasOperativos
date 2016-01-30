#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/string.h>
#include <sys/socket.h>
#include "Configuracion.h"
#include "InteraccionConPantalla.h"

void terminarProceso(char* MensajeError)
{
	imprimirError(MensajeError); //Muestra un mensaje de error alertando del problema
	printf("\n");
	exit(0); //Finaliza el proceso
}


t_config * crearArchivoDeConfiguracion(char* ubicacionCarpeta,char* nombre)
{
	char * ruta = string_new();
	string_append(&ruta,ubicacionCarpeta);
	string_append(&ruta,nombre);
	t_config* archivoDeConfiguracion = config_create(ruta);
	if(archivoDeConfiguracion == NULL)
	{
		terminarProceso("La ruta del archivo de configuracion no es correcta");
	}
	return archivoDeConfiguracion;
}


int obtener_int(t_config* configuracion, char* clave)
{
	int valor;
	if (config_has_property(configuracion, clave) == true) //Comprueba que el archivo de configuracion contenga la clave solicitada.
	{
		valor = config_get_int_value(configuracion, clave); // se le asigna a la variable valor el valor correspondiente a la clave solicitada
	}
	else
		terminarProceso("Error: Archivo de configuracion incompleto.");

	return valor;
}


char* obtener_string(t_config* configuracion, char* clave)
{
	char* valor;
	if (config_has_property(configuracion, clave) == true) //Comprueba que el archivo de configuracion contenga la clave solicitada.
	{
		valor = config_get_string_value(configuracion, clave); // se le asigna a la variable valor el valor correspondiente a la clave solicitada
		if (valor == NULL)
			terminarProceso("Error: Archivo de configuracion sin valores definidos.");
	}
	else
		terminarProceso("Error: Archivo de configuracion incompleto.");

	return valor;
}

double obtener_double(t_config* configuracion, char* clave)
{
	double valor;
	if (config_has_property(configuracion, clave) == true) //Comprueba que el archivo de configuracion contenga la clave solicitada.
	{
		valor = config_get_double_value(configuracion, clave); // se le asigna a la variable valor el valor correspondiente a la clave solicitada
	}
	else
		terminarProceso("Error: Archivo de configuracion incompleto.");

	return valor;
}

void obtenerDatosConfiguracion(t_config* configuracion,t_datosConfiguracion* datos)
{
	datos->miIp = obtener_string(configuracion,"IP");
	datos->miPuerto = obtener_int(configuracion,"PUERTO_ESCUCHA");
	datos->nombreSwap = obtener_string(configuracion,"NOMBRE_SWAP");
	datos->cantidadPaginas = obtener_int(configuracion,"CANTIDAD_PAGINAS");
	datos->tamanioPagina = obtener_int(configuracion,"TAMANIO_PAGINA");
	datos->retardoSwap = obtener_double(configuracion,"RETARDO_SWAP");
	datos->retardoCompactacion = obtener_double(configuracion,"RETARDO_COMPACTACION");
}

