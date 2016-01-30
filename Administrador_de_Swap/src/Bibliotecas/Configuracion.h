#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_


#include <commons/config.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct{
	char * miIp;
	int miPuerto;
	char * nombreSwap;
	int cantidadPaginas;
	int tamanioPagina;
	double retardoSwap;
	double retardoCompactacion;
}t_datosConfiguracion;

// Dada la ubicacion de la carpeta de los archivos y el nombre del archivo,crea el archivo de configuracion comprobando errores. Si hay error el proceso termina mostrando por pantalla el motivo del error
t_config * crearArchivoDeConfiguracion(char* ubicacionCarpeta,char* nombre);

//termina el proceso mostrando un mensaje con el error que causo la falla
void terminarProceso(char* MensajeError);

//obtiene un dato de tipo int del archivo de configuracion
int obtener_int(t_config* configuracion, char* clave);

//obtiene un dato de tipo string del archivo de configuracion
char* obtener_string(t_config* configuracion, char* clave);

//Dado un puntero a una variable de tipo t_datosConfiguracion, inicializa la variable con los datos del archivo de configuracion
void obtenerDatosConfiguracion(t_config* configuracion,t_datosConfiguracion* datos);

double obtener_double(t_config* configuracion, char* clave);
#endif /* CONFIGURACION_H_ */
