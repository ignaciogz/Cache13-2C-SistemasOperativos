
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>

#include "modelo/configuracion.h"
#include "modelo/estructuras.h"
#include "modelo/sockets.h"
#include "vistas/cfgView.h"
#include "vistas/mainView.h"
#include "controladorFrontal.h"
#include "controladores/sendController.h"

//Variables de LOG:
t_log * archivoLogger;
t_log * archivoVarDump;

//Variables de configuracion:
	double Retardo;
	int Cant_Marcos;
	int Tamanio_Marco;
	int Max_Marcos_Por_Proceso;
	
	char* TLB_Habilitada;
	int Entradas_TLB;
	char* Algoritmo_De_Memoria;

//Estructura Addr del SWAP
	t_conexion conectorSWAP;

//Estructuras IMPORTANTES del proceso:
	t_structMP structMemoriaPrincipal;
	t_structTLB structTLB;
	t_dictionary* dicInfoProcesos;

//Mutex para las Estructuras IMPORTANTES del proceso
	pthread_mutex_t mutexStructMP = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexStructTLB = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexDicInfoProcesos = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexHistorica = PTHREAD_MUTEX_INITIALIZER;

//Contadores Globales
	int Aciertos_Historica = 0;
	int Accesos_Historica = 0;

int main()
{
/*-------------------- Obteniendo datos de configuracion --------------------------*/
	t_config * config;
	config = config_create("Archivos/configuracion.cfg");
	if(config== NULL)
		terminarProceso("No se pudo crear el arch de config");

	//Creo los archivos de log
	archivoLogger = log_create("Archivos/logger.log", "Proceso_AdmMemoria", false, LOG_LEVEL_INFO);
	archivoVarDump = log_create("Archivos/var_dump.log", "Proceso_AdmMemoria", false, LOG_LEVEL_INFO);


	//-------------------- Datos del PROCESO --------------------------
		char* IP_DEL_PROCESO = obtener_string(config,"IP_DEL_PROCESO");
		int Puerto_ESCUCHA = obtener_int(config,"PUERTO_ESCUCHA");
		Cant_Marcos =  obtener_int(config,"CANTIDAD_MARCOS");
		Tamanio_Marco =  obtener_int(config,"TAMANIO_MARCO");
		Max_Marcos_Por_Proceso =  obtener_int(config,"MAX_MARCOS_POR_PROCESO");
		Retardo = obtener_double(config,"RETARDO");
		Algoritmo_De_Memoria = obtener_string(config,"ALGORITMO_MEMORIA");

	//----------------------- Datos del SWAP ------------------------
		char* IP_SWAP = obtener_string(config,"IP_SWAP");
		int Puerto_SWAP = obtener_int(config,"PUERTO_SWAP");

	//-------------------- Datos Opcionales TLB --------------------------
		TLB_Habilitada = obtener_string(config,"TLB_HABILITADA");
		Entradas_TLB = obtener_int(config,"ENTRADAS_TLB");

	cfgView_DatosConfig(IP_DEL_PROCESO, Puerto_ESCUCHA, Cant_Marcos,
						Tamanio_Marco, Max_Marcos_Por_Proceso, Retardo, Algoritmo_De_Memoria);
	cfgView_DatosConfigSwap(IP_SWAP, Puerto_SWAP);
	cfgView_DatosConfigOpcionales(TLB_Habilitada, Entradas_TLB);

	free(config);


/*------------------ Creando Memoria, TLB y Diccionario con info sobre procesos -----------------------*/
	structMemoriaPrincipal.sMemoria = inicializarMemoriaPrincipal();
	structMemoriaPrincipal.sMemoriaLibre = inicializarMemoriaLibre();
	structMemoriaPrincipal.sCantMemoriaLibre = inicializarCantMemoriaLibre();
	
	structTLB.sCacheTLB = inicializarTLB();
	structTLB.sOrdenDeIngresoTLB = queue_create();

	dicInfoProcesos = dictionary_create();


/*---------------------------- Creando Sockets de los Procesos ----------------------------------*/
	int socketMemoria, socketSWAP;

	struct sockaddr_in myAddr;

	//
		if((socketMemoria = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socketMemoria");
			exit(1);
		}
		//Inicializando la estructura de direccion del PROCESO
		myAddr = inicializarDireccion(Puerto_ESCUCHA, IP_DEL_PROCESO);
			free(IP_DEL_PROCESO);

	//
		if((socketSWAP = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
				perror("socketSWAP");
				exit(1);
		}
		//Inicializando la estructura de direccion del SWAP
		conectorSWAP.SwapAddr = inicializarDireccion(Puerto_SWAP, IP_SWAP);
			free(IP_SWAP);


/*-------------------- Conectar al SWAP, en caso positvo escuchar solicitudes del CPU --------------------------*/
	mainView_ConexionConSWAP_WAIT();

	if(actionConexionAlSWAP_SC(socketSWAP))
	{
		mainView_EstadoOperativo();
		mainView_ConexionDelCPU_WAIT();
		
		//El controlador frontal decide que hacer con las solicitudes:
			actionEscucharSolicitudesDelCPU(socketMemoria, myAddr);
	}

	return EXIT_SUCCESS;
}
