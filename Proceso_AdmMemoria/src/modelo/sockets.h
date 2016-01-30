/*
 * sockets.h
 *
 *  Created on: -
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

		typedef enum{
			//Solicitudes del CPU:
				CPU_NuevoProceso = 20,
				CPU_LecturaDePagina = 21,
				CPU_EscrituraDePagina = 23,
				CPU_FinalizarProceso = 1,
			//RESPUESTAS al CPU:
				CPU_NuevoProcesoOK = 24,
				CPU_NuevoProcesoFAIL = 25,
				CPU_EscrituraOK = 99,

			
			//Aviso al SWAP que estoy activo:
				ConexionMemoriaSWAP = 10,
			//Peticiones al SWAP:
				SWAP_NuevoProceso = 30,
				SWAP_LecturaDePagina = 31,
				SWAP_EscrituraDePagina = 32,
				SWAP_FinalizarProceso = 33,
			//RESPUESTAS del SWAP:
				SWAP_NuevoProcesoOK = 34,
				SWAP_NuevoProcesoFAIL = 35
		}t_header;

		typedef struct{
			struct sockaddr_in SwapAddr;
		}t_conexion;

		typedef struct{
			int SocketSolicitante;
			t_header Cabecera;
		}t_data;

	//
	struct sockaddr_in inicializarDireccion(int puerto, char* ip);

	//
	void packHeader(t_header header, void* mensaje);

	//
	void umpackHeader(t_header* header, void* package);

	//
	int sendall(int s, char* buf, int* len);

	//
	int* obtenerUnSocket();

	/*---------------------- Recibiendo contenido de mensajes ----------------------------*/
	int* umpackPID(t_data * datosDeSolicitud);
	int* umpackCantidadDePaginas(t_data * datosDeSolicitud);
	int* umpackNumeroDePagina(t_data * datosDeSolicitud);
	char* umpackDatos(t_data * datosDeSolicitud);
	int* umpackCPU(t_data * datosDeSolicitud);
	
	/*---------------------------- Preparando contenido de mensajes ----------------------------------*/
	void packCrearProceso(int PID, int CantidadDePaginas, void* mensaje);
	void packLecturaDePagina(int PID, int NumeroDePagina, void* mensaje);
	void packEscrituraDePagina(int PID, int NumeroDePagina, char* Datos, void* mensaje);
	void packFinalizarProceso(int PID, void* mensaje);
	
#endif
