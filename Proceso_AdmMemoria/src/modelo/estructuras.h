/*
 * estructuras.h
 *
 *  Created on: -
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_
	#include <commons/collections/list.h>
 	#include <commons/collections/queue.h>

	typedef struct{
		char** sMemoria;
		int* sMemoriaLibre;
		int sCantMemoriaLibre;
	}t_structMP;

	typedef struct{
		int** sCacheTLB;
		t_queue* sOrdenDeIngresoTLB;
	}t_structTLB;

	typedef struct{
		int totalDePaginas;
		int ** tablaDePaginas;
		t_list* marcosAsignados;
		t_queue* marcos;
		int contadorPageFault;
		int contadorAccesos;
	}t_proceso;

	typedef struct{
		int NumeroMarco;
		int NumeroPagina;
		int BitUso;
		int BitMod;
	}t_infoDeMarco;

/* --------------------  Memoria Principal  -------------------------- */
char ** inicializarMemoriaPrincipal();
	
	/* --------------------------  FAUX para inicializar  -------------------------------- */
		char* reservarMemoriaParaMarco();
	
	/* --------------------  FAUX para los algoritmos de reemplazo  -------------------------- */
		void cargarMarco_MemoriaPrincipal(int numeroMarco, char* contenido);

		char* obtenerContenido_MemoriaPrincipal(int numeroMarco);

	/* --------------------------  FAUX para los procesos  -------------------------------- */
		void asignarMarcos_MemoriaPrincipal(t_list* marcosAsignados);
		
		int asignarUnMarco_MemoriaPrincipal(t_list* marcosAsignados);

		void eliminarMarcos_MemoriaPrincipal(t_list* marcosAsignados);

			void resetearMarco(void * numeroMarco);

/* --------------------  Gestion de Memoria Principal LIBRE   -------------------------- */
int* inicializarMemoriaLibre();

int inicializarCantMemoriaLibre();

	/* --------------------------  FAUX para Memoria Principal Libre  -------------------------------- */
		void indicarMarcoOcupado_MemoriaPrincipal(int numeroMarco);
		
		void indicarMarcoLiberado_MemoriaPrincipal(int numeroMarco);

		int hayEspacioParaSolicitudDeMarcos(int cantidadDeMarcosRequeridos);

/* --------------------  TLB  -------------------------- */
int ** inicializarTLB();

	int escribirEntrada_TLB(int numeroPagina, int PID, int numeroMarco);

	int paginaCargada_TLB(int numeroPagina, int PID);

	int obtenerMarcoDePagina_TLB(int numeroPagina, int PID);

	int entradaDePagina_TLB(int numeroPagina, int PID);

	void quitarPagina_TLB(int PID, int numeroDePagina);
	
	void eliminarDatosDelProceso_TLB(int PID);

	void actualizarOrdenDeIngreso_TLB(int entrada);
	
	void vaciar_TLB();

/* -----------------------------  PROCESOS  ----------------------------------- */
void inicializarProceso(int PID, int totalDePaginas);

t_proceso* obtenerProceso(int PID);

int** obtenerTPProceso(int PID);

t_queue* obtenerMarcosProceso(int PID);

void eliminarProceso(int PID);

/* ----------------------------  TABLA DE PAGINAS de 1 PROCESO  --------------------------------- */
int ** inicializarTablaDePaginas(int filasTablaPagina);

void actualizarEntrada_TP(int** TablaPagina, int NumeroPagina, int BitPresencia, int NumeroMarco);

int paginaPresente_TP(int** TablaPagina, int NumeroPagina);

int obtenerMarcoDePagina_TP(int** TablaPagina, int NumeroPagina);

void resetear_TP(int** TablaPagina, int numeroDePagina);

void eliminar_TP(int ** structTBPAGINA, int cantTotalPag);

/* ----------------------------   Funciones Auxiliares PRO   --------------------------------- */
int** crearTabla(int filas, int columnas);

int agregarALaCola(int entrada, t_queue* colita);
#endif
