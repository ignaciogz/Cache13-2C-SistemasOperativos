
#ifndef RECVCONTROLLER_H_
#define RECVCONTROLLER_H_
	#include "../modelo/sockets.h"

	//
	void actionCrearProceso_RC(t_data * datosDeSolicitud);

	//
	void actionLecturaDePagina_RC(t_data * datosDeSolicitud);

	//
	void actionEscrituraDePagina_RC(t_data * datosDeSolicitud);
	
	//
	void actionFinalizarProceso_RC(t_data * datosDeSolicitud);

	//
	int actionCrearProcesoRESULT_RC(int* socketSWAP);

	//
	char* actionLecturaDePaginaRESULT_RC(int* socketSWAP);

	/* -------------------------------------  F Auxiliares  ------------------------------------------- */
	void intentarCrearProcesoNuevo(t_data * datosDeSolicitud, int* PID, int* CantidadDePaginas);

#endif