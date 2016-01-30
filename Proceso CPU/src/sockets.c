/*
 * sockets.c
 *
 *  Created on: 1/9/2015
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

void mostrarIdHilo( int ID ){
	printf("El ID del Cpu es:%i\n",ID);
}

void packEscritura(t_datos data,int tamanio, char* contenido, void* mensaje)
{
	int offset = sizeof(t_header);
	memcpy(mensaje + offset, (int*) &data.PID, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset, (int*) &data.valor, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset, (int*) &tamanio, sizeof(int));
	offset = offset + sizeof(int);
	memcpy(mensaje + offset, contenido, tamanio);
	offset = offset + tamanio;
	memcpy(mensaje + offset,(int*)&data.ID , sizeof(int));


	return;
}

