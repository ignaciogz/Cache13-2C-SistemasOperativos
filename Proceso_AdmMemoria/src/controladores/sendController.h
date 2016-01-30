
#ifndef SENDCONTROLLER_H_
#define SENDCONTROLLER_H_

 	//
	int actionConexionAlSWAP_SC(int socketSWAP);

	//
	int actionCrearProcesoAlSWAP_SC(int PID, int CantidadDePaginas, int* socketDelSwapProcesoN);

	//
	int actionLecturaDePaginaAlSWAP_SC(int PID, int NumeroDePagina, int* socketDelSwapLectura);

	//
	int actionEscrituraDePaginaAlSWAP_SC(int PID, int NumeroDePagina, char* Datos);

	//
	int actionFinalizarProcesoAlSWAP_SC(int PID);

	//
	int actionCrearProcesoFAIL_SC(int unSocketDelCPU);
	
	//
	int actionCrearProcesoOK_SC(int unSocketDelCPU);

	//
	int actionEnviarContenidoDePaginaCPU_SC(int unSocketDelCPU, char* Datos);

	//
	int actionEnviarEscribirPaginaOK_SC(int unSocketDelCPU);
#endif
