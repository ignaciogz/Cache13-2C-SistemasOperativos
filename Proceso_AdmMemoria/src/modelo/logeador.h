#ifndef LOGEADOR_H_
#define LOGEADOR_H_

#include <commons/log.h>
#include <commons/collections/queue.h>

	//
	void LOG_mProcCreado(int pid, int cantidadPaginas);

	//
	void LOG_solicitudEscritura(int pid, int numeroDePagina);

	//
	void LOG_solicitudEscrituraConTLB(int pid, int numeroDePagina, char* eventoTLB, int marco);

	//
	void LOG_solicitudLectura(int pid, int numeroDePagina);

	//
	void LOG_solicitudLecturaConTLB(int pid, int numeroDePagina, char* eventoTLB, int marco);

	//
	void LOG_accesoMemoria(int pid, int numPagina, int numMarco);

	//
	void LOG_accesoSWAP(int PID, t_queue* colitaINICIAL, t_queue* colitaFINAL, int ptrColitaINICIAL, int ptrColitaFINAL);

	//
	void LOG_senialRecibidad(char* tipo, char* accion);

	//
	void LOG_senialTratada(char* tipo);

#endif