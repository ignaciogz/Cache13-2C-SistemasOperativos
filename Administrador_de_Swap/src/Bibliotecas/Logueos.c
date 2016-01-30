#include <stdio.h>
#include <stdlib.h>
#include "Logueos.h"
#include  <commons/string.h>
#include <commons/error.h>
#include <pthread.h>


extern t_log * archivoLogger;
pthread_mutex_t mutexLog = PTHREAD_MUTEX_INITIALIZER;

t_log* crearArchivoDeLogger(char* ubicacionCarpeta,char* nombreArchivo)
{
	char * ruta = string_new();
	string_append(&ruta,ubicacionCarpeta);
	string_append(&ruta,nombreArchivo);
	t_log * archivoLogger = log_create(ruta,"Proceso Administrador de Swap",false,LOG_LEVEL_INFO);
	if(archivoLogger == NULL)
	{
		error_show("La ruta del archivo de logger no es correcta\n");
		exit(0);
	}
	return archivoLogger;
}


void loguearmProcCreado(int pid,int byteInicial,int cantBytes)
{
	char * mensaje = string_new();
	string_append(&mensaje,"[mProc asignado] ");
	char * nro_pid = string_itoa(pid);
	char * bytesAsignados = string_itoa(cantBytes);
	char * primerByte = string_itoa(byteInicial);
	string_append(&mensaje,"PID: ");
	string_append(&mensaje,nro_pid);
	string_append(&mensaje,"   ");
	string_append(&mensaje,"Byte inicial: ");
	string_append(&mensaje,primerByte);
	string_append(&mensaje,"   ");
	string_append(&mensaje,"Cantidad de bytes: ");
	string_append(&mensaje,bytesAsignados);
	string_append(&mensaje,"\n");
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLog);
}


void loguearmProcLiberado(int pid,int byteInicial,int cantBytes,int paginasEscritas,int paginasLeidas)
{
	char * mensaje = string_new();
	string_append(&mensaje,"[mProc liberado] ");
	char * nro_pid = string_itoa(pid);
	char * bytesLiberados = string_itoa(cantBytes);
	char * primerByte = string_itoa(byteInicial);
	char * escritas = string_itoa(paginasEscritas);
	char* leidas = string_itoa(paginasLeidas);
	string_append(&mensaje,"PID: ");
	string_append(&mensaje,nro_pid);
	string_append(&mensaje,"   Byte inicial: ");
	string_append(&mensaje,primerByte);
	string_append(&mensaje,"   Tamaño en bytes: ");
	string_append(&mensaje,bytesLiberados);
	string_append(&mensaje,"   Cantidad de escrituras: ");
	string_append(&mensaje,escritas);
	string_append(&mensaje,"   Cantidad de lecturas: ");
	string_append(&mensaje,leidas);
	string_append(&mensaje,"\n");
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLog);
}


void loguearmProcRechazado(int pid)
{
	char * mensaje = string_new();
	char * identificadorProceso = string_itoa(pid);
	string_append(&mensaje,"[mProc ");
	string_append(&mensaje,identificadorProceso);
	string_append(&mensaje,"] rechazado por falta de espacio\n");
	pthread_mutex_lock(&mutexLog);
	log_warning(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLog);
}


void loguearCompactacionIiniciada()
{
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,"Compactacion iniciada\n");
	pthread_mutex_unlock(&mutexLog);
}


void loguearCompactacionFinalizada()
{
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,"Compactacion finalizada\n");
	pthread_mutex_unlock(&mutexLog);
}


void loguearInicioDeEjecucion()
{
	log_info(archivoLogger,"Inicio de ejecucion del Proceso Administrador de Swapping\n");
}


void loguearEscritura(int pid,int byteInicial,int numeroPagina,int inicioPagina,int tamanio,char* contenido)
{
	char * mensaje = string_new();
	char * inicio = string_itoa(byteInicial);
	char * PID = string_itoa(pid);
	char * tamanioEscritura = string_itoa(tamanio);
	char* pagina = string_itoa(numeroPagina);
	char* inicioPag= string_itoa(inicioPagina);
	string_append(&mensaje,"[Escritura solicitada] ");
	string_append(&mensaje,"PID: ");
	string_append(&mensaje,PID);
	string_append(&mensaje," Byte inicial mProc: ");
	string_append(&mensaje,inicio);
	string_append(&mensaje,"   Numero de pagina: ");
	string_append(&mensaje,pagina);
	string_append(&mensaje,"   Byte inicial pagina: ");
	string_append(&mensaje,inicioPag);
	string_append(&mensaje,"   Tamanio: ");
	string_append(&mensaje,tamanioEscritura);
	string_append(&mensaje,"   Contenido: ");
	string_append(&mensaje,contenido);
	string_append(&mensaje,"\n");
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLog);
}


void loguearLectura(int pid,int byteInicial,int numeroPagina,int inicioPagina,int tamanio,char* contenido)
{
	char * mensaje = string_new();
	char * inicio = string_itoa(byteInicial);
	char* PID = string_itoa(pid);
	char * tamanioEscritura = string_itoa(tamanio);
	char* pagina = string_itoa(numeroPagina);
	char* inicioPag= string_itoa(inicioPagina);
	string_append(&mensaje,"[Lectura solicitada] ");
	string_append(&mensaje,"PID: ");
	string_append(&mensaje,PID);
	string_append(&mensaje,"   Byte inicial mProc: ");
	string_append(&mensaje,inicio);
	string_append(&mensaje,"   Numero de pagina: ");
	string_append(&mensaje,pagina);
	string_append(&mensaje,"   Byte inicial pagina: ");
	string_append(&mensaje,inicioPag);
	string_append(&mensaje,"   Tamanio: ");
	string_append(&mensaje,tamanioEscritura);
	string_append(&mensaje,"   Contenido: ");
	string_append(&mensaje,contenido);
	string_append(&mensaje,"\n");
	pthread_mutex_lock(&mutexLog);
	log_info(archivoLogger,mensaje);
	pthread_mutex_unlock(&mutexLog);
}

