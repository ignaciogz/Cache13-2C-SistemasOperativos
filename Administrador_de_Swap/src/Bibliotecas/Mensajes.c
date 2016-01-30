#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/error.h>
#include "InteraccionConPantalla.h"
#include "Paquetes.h"
#include "Logueos.h"
#include "Mensajes.h"

extern pthread_mutex_t mutexEspacioLibre;
extern pthread_mutex_t mutexEspacioOcupado;

void atenderMensaje(t_parametro* parametros)
{
	switch(parametros->cabecera)
	{

	case ConexionAdmMemoria:			mostrarQueEstaOperativo();
										close(parametros->conexion.socket);
										break;
	case InicioDemProc:					reservarMemoria(*parametros);
										close(parametros->conexion.socket);
										break;

	case SolicitudDeEscrituraDePagina:	procesarEscritura(*parametros);
										close(parametros->conexion.socket);
										break;

	case SolicitudDeLecturaDePagina:	procesarLecturaDePagina(*parametros);
										close(parametros->conexion.socket);
										break;

	case LiberarmProc:					eliminarmProc(*parametros);
										close(parametros->conexion.socket);
										break;

	default:				            break;
	}
	free(parametros);
}

void inicializarParametrosHilo(t_parametro* parametros,t_list* espacioUtilizado,t_list* espacioLibre,char* ubicacionArchivoSwap,t_conexion conexionNueva,t_datosConfiguracion datos,t_cabecera cabecera)
{
	parametros->cabecera = cabecera;
	parametros->conexion = conexionNueva;
	parametros->tamanioPagina = datos.tamanioPagina;
	parametros->cantidadPaginas = datos.cantidadPaginas;
	parametros->ubicacionSwap = ubicacionArchivoSwap;
	parametros->retardoCompactacion = datos.retardoCompactacion;
	parametros->retardoSwap = datos.retardoSwap;
	parametros->espaciosLibres = espacioLibre;
	parametros->espaciosOcupados = espacioUtilizado;
}

void asignarHilo(t_parametro* parametros)
{
	pthread_t hilo;
	pthread_create( &hilo, NULL,(void*)&atenderMensaje,parametros);
}

void procesarLecturaDePagina(t_parametro parametros)
{
	int pid = desempaquetarPid(parametros.conexion);
	int nroPagina;
	recv(parametros.conexion.socket,(int*)&nroPagina,sizeof(int),0);
	t_pagina pagina;
	pagina.numeroDePagina = nroPagina;
	pthread_mutex_lock(&mutexEspacioOcupado);
	int posicion = buscarEspacio(parametros.espaciosOcupados,pid,&tienePidIgual);
	t_mProc* mProc = list_get(parametros.espaciosOcupados,posicion);
	mProc->paginasLeidas = mProc->paginasLeidas + 1;
	pthread_mutex_unlock(&mutexEspacioOcupado);
	pagina.inicio = mProc->comienzoDelHueco + (parametros.tamanioPagina * nroPagina);
	char* contenido = consultarPagina(pagina,parametros.ubicacionSwap,parametros.tamanioPagina);
	int tamanioContenido= strlen(contenido)+1;
	usleep(parametros.retardoSwap*1000000);
	loguearLectura(mProc->pid,mProc->comienzoDelHueco,pagina.numeroDePagina,pagina.inicio,tamanioContenido,contenido);
	empaquetarYEnviarContenidoPagina(parametros.conexion,contenido);
}


void reservarMemoria(t_parametro parametros)
{
	t_espacioLibre * hueco = malloc(sizeof(t_espacioLibre));
	t_mProc* mProcRecibido = malloc (sizeof(t_mProc));
	desempaquetarmProc(parametros.conexion,mProcRecibido);
	int espacioRequerido = mProcRecibido->cantidadDePaginas;
	int espacioDisponible = cantidadDeEspacioLibre(parametros.espaciosLibres,parametros.tamanioPagina);
	if(espacioRequerido > espacioDisponible)
		{  //se esceribe en el logger q se rechazo el mProc y se lo informa al administrador de memoria
			rechazarmProc(parametros.conexion,mProcRecibido);
			free(mProcRecibido);
		}
	else
		{
		pthread_mutex_lock(&mutexEspacioLibre);
		int posicion = buscarEspacio(parametros.espaciosLibres,mProcRecibido->cantidadDePaginas,&tieneEspacioHueco);
		pthread_mutex_unlock(&mutexEspacioLibre);
		if(posicion == -1)
			{
				loguearCompactacionIiniciada();
				compactar(parametros.espaciosLibres,parametros.espaciosOcupados,parametros.ubicacionSwap,parametros.tamanioPagina);
				usleep(parametros.retardoCompactacion*1000000);
				loguearCompactacionFinalizada();
				posicion = buscarEspacio(parametros.espaciosLibres,mProcRecibido->cantidadDePaginas,&tieneEspacioHueco);
			}
		hueco = list_get(parametros.espaciosLibres,posicion);
		agregarmProc(parametros.espaciosOcupados,mProcRecibido,hueco->comienzoDelHueco);
		loguearmProcCreado(mProcRecibido->pid,mProcRecibido->comienzoDelHueco,mProcRecibido->cantidadDePaginas * parametros.tamanioPagina);
		actualizarEspacioLibre(parametros.espaciosLibres,mProcRecibido,hueco,parametros.tamanioPagina,espacioDisponible);
		empaquetarYEnviarmProcExitoso(parametros.conexion,mProcRecibido->pid);
		}
}

void rechazarmProc(t_conexion conexion,t_mProc * mProcRecibido)
{
	loguearmProcRechazado(mProcRecibido->pid);
	empaquetarYEnviarmProcRechazado(conexion,mProcRecibido->pid);
}

void eliminarmProc(t_parametro parametros)
{
	int pid = desempaquetarPid(parametros.conexion);
	liberarmProc(pid,parametros.espaciosLibres,parametros.espaciosOcupados,parametros.tamanioPagina);
}

void procesarEscritura(t_parametro parametros)
{
	t_pagina paginaNueva;
	int pid = desempaquetarPid(parametros.conexion);
	paginaNueva = desempaquetarPagina(parametros.conexion);
	pthread_mutex_lock(&mutexEspacioOcupado);
	int posicion = buscarEspacio(parametros.espaciosOcupados,pid,&tienePidIgual);
	t_mProc* mProc = list_get(parametros.espaciosOcupados,posicion);
	mProc->paginasEscritas = mProc->paginasEscritas + 1;
	pthread_mutex_unlock(&mutexEspacioOcupado);
	paginaNueva.inicio = mProc->comienzoDelHueco + (parametros.tamanioPagina * paginaNueva.numeroDePagina);
	escribirEnPagina(paginaNueva,parametros.ubicacionSwap,parametros.tamanioPagina);
	int tamanio = strlen(paginaNueva.datos) +1;
	usleep(parametros.retardoSwap*1000000);
	enviarFinEscritura(parametros.conexion);
	loguearEscritura(mProc->pid,mProc->comienzoDelHueco,paginaNueva.numeroDePagina,paginaNueva.inicio,tamanio,paginaNueva.datos);
}

