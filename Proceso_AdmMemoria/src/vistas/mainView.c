#include <stdio.h>
#include <stdlib.h>

#include "mainView.h"

//CPU
	void mainmainView_ConexionDelCPU_OK()
	{
		printf("Mensaje enviado correctamente al proceso CPU...\n");
		return;
	}

	void mainView_ConexionDelCPU_FAIL()
	{
		printf("Fallo el envio de mensaje al proceso CPU !\n");
		return;
	}

	void mainView_ConexionDelCPU_WAIT()
	{
		printf("Esperando solicitudes del proceso CPU...\n");
		return;
	}

//SWAP
	void mainView_ConexionConSWAP_OK()
	{
		printf("Conexion con SWAP establecida\n\n");
		return;
	}

	void mainView_ConexionConSWAP_FAIL()
	{
		printf("El SWAP esta Desconectadoooo !\n");
		return;
	}

	void mainView_ConexionConSWAP_WAIT()
	{
		printf("\nIntentado conectar con el SWAP...\n");
		return;
	}

//GENERAL
void mainView_EstadoOperativo()
{
	printf("\033[0;34m============================================================\033[1;37m\n");
	printf("          Administrador de Memoria:  OPERATIVO =) \n");
	printf("\033[0;34m============================================================\033[37m\n");
	return;
}
