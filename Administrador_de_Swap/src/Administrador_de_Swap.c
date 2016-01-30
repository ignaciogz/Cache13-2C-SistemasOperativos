#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <string.h>
#include "Bibliotecas/Configuracion.h"
#include "Bibliotecas/Conexiones.h"
#include "Bibliotecas/Mensajes.h"
#include "Bibliotecas/Logueos.h"
#include "Bibliotecas/ManejoSwap.h"
#include "Bibliotecas/InteraccionConPantalla.h"

t_log * archivoLogger;

int main(void)
{
	mostrarInicioDelProceso();

	//se inicializan las variables
	t_list * espacioUtilizado = list_create();
	t_list * espacioLibre = list_create();
	t_config * configuracion;
	char * ubicacionArchivos = "Archivos/";
	configuracion = crearArchivoDeConfiguracion(ubicacionArchivos,"Configuracion.cfg");
	archivoLogger = crearArchivoDeLogger(ubicacionArchivos,"Logger.log");
	loguearInicioDeEjecucion(archivoLogger);
	struct sockaddr_in miDireccion,direccionEntrante;;
	int miSocket,nuevoSocket=0;
	t_conexion conexionNueva;
	t_datosConfiguracion datosArchivoDeConfiguracion;
	socklen_t size = sizeof(struct sockaddr_in);
	t_cabecera cabecera;
	char* ubicacionArchivoSwap = string_new();
	string_append(&ubicacionArchivoSwap,ubicacionArchivos);

	// Se obtienen los datos del archivo de configuracion
	obtenerDatosConfiguracion(configuracion,&datosArchivoDeConfiguracion);
	string_append(&ubicacionArchivoSwap,datosArchivoDeConfiguracion.nombreSwap);
	//se crea el archivo de swapping

	crearArchivoSwapping(ubicacionArchivos,datosArchivoDeConfiguracion.nombreSwap,datosArchivoDeConfiguracion.tamanioPagina,datosArchivoDeConfiguracion.cantidadPaginas);
/*	FILE* descriptor = abrirArchivo(ubicacionArchivoSwap,"r+");
	int guacho = datosArchivoDeConfiguracion.tamanioPagina*datosArchivoDeConfiguracion.cantidadPaginas;
	char * cocucha = malloc(guacho);
	memset(cocucha,'A',guacho);
	escribir(cocucha,guacho,1,descriptor);
	fclose(descriptor);*/
	//Se agrega que toda la particion es un hueco libre
	t_espacioLibre * huecoInicial = malloc(sizeof(t_espacioLibre));

	agregarEspacioLibre(espacioLibre,huecoInicial,0,datosArchivoDeConfiguracion.cantidadPaginas);

	//Se crea la direccion
	miDireccion = generarDireccion(datosArchivoDeConfiguracion.miIp,datosArchivoDeConfiguracion.miPuerto);
	//se inicializa el socket comprobando errores
	inicializarSocket(&miSocket);
	//se bindea al proceso a un puerto especifico comprobando errores
	bindComprobandoError(&miSocket,miDireccion);
	//Se muestra por pantalla la informacion del archivo de configuracion
	mostrarInfoCongiguracion(miDireccion,datosArchivoDeConfiguracion);

	// se espera a que el administrador de memoria este operativo para poder funcionar
	//el proceso se pone a la espera de conexiones comprobando errores
	escucharComprobandoErrores(&miSocket,5);
	mostrarEsperaDeConexionDelAdmDeMemoria();
	//mientras no sea una conexion del administrador de memoria, ignora las demas
	do
	{	 //se acepta la conexion comprobando errores y se recibe el header
		nuevoSocket = aceptarComprobandoError(miSocket,direccionEntrante,size);
		desempaquetarCabecera(&cabecera,nuevoSocket);
	}while(cabecera != ConexionAdmMemoria);
	t_parametro* parametros = malloc(sizeof(t_parametro));
	inicializarParametrosHilo(parametros,espacioUtilizado,espacioLibre,ubicacionArchivoSwap,conexionNueva,datosArchivoDeConfiguracion,cabecera);
	asignarHilo(parametros);

	while(1)
	{
		//Acepto las solicitudes entrantes
		conexionNueva.socket= aceptarComprobandoError(miSocket,direccionEntrante,size);
		//inicializo la variable con los parametros y asigno un hilo para atender la solicitud
		desempaquetarCabecera(&cabecera,conexionNueva.socket);
		t_parametro* parametros = malloc(sizeof(t_parametro));
		inicializarParametrosHilo(parametros,espacioUtilizado,espacioLibre,ubicacionArchivoSwap,conexionNueva,datosArchivoDeConfiguracion,cabecera);
		asignarHilo(parametros);
	}
	return EXIT_SUCCESS;
}
