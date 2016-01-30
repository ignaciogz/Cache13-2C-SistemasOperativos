#ifndef MENSAJES_H_
#define MENSAJES_H_

#include "Conexiones.h"
#include "Configuracion.h"
#include "ManejoSwap.h"
#include "commons/collections/list.h"

typedef struct{
	t_cabecera cabecera;
	t_conexion conexion;
	char* ubicacionSwap;
	int cantidadPaginas;
	int tamanioPagina;
	double retardoCompactacion;
	double retardoSwap;
	t_list * espaciosLibres;
	t_list * espaciosOcupados;
}t_parametro;

// dada una variable de tipo t_parametro atiende un mensaje recibido
void atenderMensaje(t_parametro* parametros);

//dados las listas que administran los espacios, los datos de la conexion, la cabecera del mensaje y los datos del archivo de configuracion, inicializa la variable "parametros"
void inicializarParametrosHilo(t_parametro* parametros,t_list* espacioUtilizado,t_list* espacioLibre,char* ubicacionArchivoSwap,t_conexion conexionNueva,t_datosConfiguracion datos,t_cabecera cabecera);

// asigna un hilo a una conexion, para luego ser atendida por la funcion atenderMensaje
void asignarHilo(t_parametro* parametros);

// Ante un pedido de lectura de pagina, le envia al Administrador de Memoria el contenido de esa pagina
void procesarLecturaDePagina(t_parametro parametros);

// dados los parametros del hilo recibe los datos del mProc y reserva memoria actualizando los espacios libres y ocupados
void reservarMemoria(t_parametro parametros);

//Dadas una conexion y un mProc, lo rechaza informandolo por logger y al administrador de memoria
void rechazarmProc(t_conexion conexion,t_mProc * mProcRecibido);

//Dados los parametros del hilo,recibe el pid del mProc a liberar, libera la memoria y loggea este evento.
void eliminarmProc(t_parametro parametros);

// dados los parametros del hilo, recibe la informacion de escritura, escribe en la pagina solicitada y loguea dicho evento
void procesarEscritura(t_parametro parametros);


#endif /* MENSAJES_H_ */
