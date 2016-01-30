#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <commons/error.h>
#include "Conexiones.h"


struct sockaddr_in generarDireccion(char*ip,int puerto)
{
	struct sockaddr_in direccion;
	direccion.sin_family= AF_INET;
	inet_aton(ip,&direccion.sin_addr);
	direccion.sin_port = htons(puerto);
	memset(&(direccion.sin_zero), '\0', 8);
	return direccion;
}

void empaquetarCabecera(t_cabecera header, void* message)
{
	memcpy(message, (t_cabecera*)&header, sizeof(t_cabecera));
	return;
}

void desempaquetarCabecera(t_cabecera* cabecera,int socketNuevo)
{
	recv(socketNuevo,(t_cabecera*)cabecera,sizeof(t_cabecera),0);
}

void enviarTodo(int unSocket, void* mensaje, int longitudMensaje)
{
	int enviado = 0;
    do{
	enviado = send(unSocket, mensaje + enviado, longitudMensaje, 0);
	if(enviado == -1)
	{
		perror("Send: ");
		exit(1);
	}
	longitudMensaje = longitudMensaje - enviado;
	}while(longitudMensaje > 0 );
}

void inicializarSocket(int * socketNuevo)
{
	*socketNuevo = socket(AF_INET, SOCK_STREAM, 0);
		if (*socketNuevo == -1)
		{
			perror("inicializar socket");
			exit(1);
		}
}

void bindComprobandoError(int * unSocket, struct sockaddr_in direccion)
{
	if( bind(*unSocket,(struct sockaddr*)&direccion,sizeof(struct sockaddr)) == -1 )
		{
			perror("bind");
			exit(1);
		}
}

int aceptarComprobandoError(int unSocket,struct sockaddr_in direccionEntrante,socklen_t size)
{
	int socketNuevo = accept(unSocket,(struct sockaddr*) &direccionEntrante,&size);
	if(socketNuevo == -1)
	{
		perror("accept");
		exit(1);
	}
	return socketNuevo;
}

void escucharComprobandoErrores(int* unSocket,int valor)
{
	if(listen(*unSocket,valor) == -1)
		{
			perror("listen");
			exit(1);
		}
}
