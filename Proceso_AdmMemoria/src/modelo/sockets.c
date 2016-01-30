/*
 * sockets.c
 *
 *  Created on: -
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "sockets.h"

struct sockaddr_in inicializarDireccion(int puerto, char* ip)
{
	struct sockaddr_in direccion;

	direccion.sin_family = AF_INET;
	direccion.sin_port = htons(puerto);
	inet_aton(ip, &(direccion.sin_addr));
	memset(&(direccion.sin_zero), '\0', 8);

	return direccion;
}

void packHeader(t_header header, void* mensaje)
{
	memcpy(mensaje, (t_header*) &header, sizeof(t_header));

	return;
}

void umpackHeader(t_header* header, void* package)
{
	memcpy((t_header*)header, package, sizeof(t_header));

	return;
}

int sendall(int s, char* buf, int* len)
{
	int total = 0;//cuantos bytes ha enviado
	int bytesleft = *len;//cuantos bytes quedan pendientes
	int n;
	while(total < *len){
		n = send(s, buf+total,bytesleft,0);
		if(n == -1){break;}
		total += n;
		bytesleft -= n;
	}
	*len = total;//devuelve la cant de bytes enviados realmente
	return n==-1?-1:0;//devuelve -1 si falla, 0 en otro caso
}

int* obtenerUnSocket()
{
	int* nuevoSocket = malloc(sizeof(int));
	if((*nuevoSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("obtenerSocket");
			exit(1);
		}
	return nuevoSocket;
}



/*---------------------------- Recibiendo contenido de mensajes ----------------------------------*/
	int* umpackPID(t_data * datosDeSolicitud)
	{
		int* PID = malloc(sizeof(int));
			recv( datosDeSolicitud->SocketSolicitante, (int*)PID, sizeof(int), 0 );
		return PID;
	}

	int* umpackCantidadDePaginas(t_data * datosDeSolicitud)
	{
		int* CantidadDePaginas = malloc(sizeof(int));
			recv( datosDeSolicitud->SocketSolicitante, (int*)CantidadDePaginas, sizeof(int), 0 );
		return CantidadDePaginas;	
	}

	int* umpackNumeroDePagina(t_data * datosDeSolicitud)
	{
		int* NumeroDePagina = malloc(sizeof(int));
			recv( datosDeSolicitud->SocketSolicitante, (int*)NumeroDePagina, sizeof(int), 0 );
		return NumeroDePagina;	
	}

	char* umpackDatos(t_data * datosDeSolicitud)
	{
		int* longitudDatos = malloc(sizeof(int));
			recv(datosDeSolicitud->SocketSolicitante,(int*)longitudDatos,sizeof(int),0);
			
		char* Datos = malloc(*longitudDatos * sizeof(char));
			recv(datosDeSolicitud->SocketSolicitante,(char*)Datos,*longitudDatos,0);
			
		free(longitudDatos);
		return Datos;
	}

	int* umpackCPU(t_data * datosDeSolicitud)
	{
		int* CPU = malloc(sizeof(int));
			recv( datosDeSolicitud->SocketSolicitante, (int*)CPU, sizeof(int), 0 );
		return CPU;
	}



/*---------------------------- Preparando contenido de mensajes ----------------------------------*/
	void packCrearProceso(int PID, int CantidadDePaginas, void* mensaje)
	{
		int offset=0;
			offset = offset + sizeof(t_header);
		memcpy(mensaje + offset, (int*)&PID, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (int*)&CantidadDePaginas, sizeof(int));

		return;
	}

	void packLecturaDePagina(int PID, int NumeroDePagina, void* mensaje)
	{
		int offset=0;
			offset = offset + sizeof(t_header);
		memcpy(mensaje + offset, (int*)&PID, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (int*)&NumeroDePagina, sizeof(int));

		return;
	}

	void packEscrituraDePagina(int PID, int NumeroDePagina, char* Datos, void* mensaje)
	{
		int longitudDatos = strlen(Datos) + 1;
		int offset=0;
			offset = offset + sizeof(t_header);
		memcpy(mensaje + offset, (int*)&PID, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (int*)&NumeroDePagina, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (int*)&longitudDatos, sizeof(int));
			offset = offset + sizeof(int);
		memcpy(mensaje + offset, (char*)Datos, longitudDatos);

		return;	
	}

	void packFinalizarProceso(int PID, void* mensaje)
	{
		int offset=0;
			offset = offset + sizeof(t_header);
		memcpy(mensaje + offset, (int*)&PID, sizeof(int));

		return;
	}