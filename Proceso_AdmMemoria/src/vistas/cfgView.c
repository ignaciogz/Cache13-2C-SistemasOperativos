#include <stdio.h>
#include <stdlib.h>

#include "cfgView.h"

void cfgView_DatosConfig(char * IP_DEL_PROCESO, int Puerto_ESCUCHA, int Cant_Marcos, int Tamanio_Marco, int Max_Marcos_Por_Proceso, double Retardo, char* Algoritmo_De_Memoria)
{
	cfgView_HeaderDatosConfig();

	printf("El IP del Proceso es: \033[1;30m%s\033[0;37m\n", IP_DEL_PROCESO);
	printf("El puerto es: \033[1;30m%i\033[0;37m\n", Puerto_ESCUCHA);
	printf("La cantidad de marcos es: \033[1;30m%i\033[0;37m\n", Cant_Marcos);
	printf("El tamanio de cada marcos es: \033[1;30m%i\033[0;37m\n", Tamanio_Marco);
	printf("La cantidad de marcos asignados a cada proceso es: \033[1;30m%i\033[0;37m\n", Max_Marcos_Por_Proceso);
	printf("El retardo de acceso es: \033[1;30m%1.2lf\033[0;37m\n", Retardo);
	printf("El algoritmo de reemplazo es: \033[1;30m%s\033[0;37m\n", Algoritmo_De_Memoria);

	return;
}


void cfgView_DatosConfigSwap(char * IP_SWAP, int Puerto_SWAP)
{
	printf("\nEl IP del SWAP es: \033[1;30m%s\033[0;37m\n", IP_SWAP);
	printf("El puerto del SWAP es: \033[1;30m%i\033[0;37m\n", Puerto_SWAP);

	return;
}


void cfgView_DatosConfigOpcionales(char * TLB_Habilitada, int Entradas_TLB)
{
	printf("\nTLB Habilitada: \033[1;30m%s\033[0;37m\n", TLB_Habilitada);
	printf("La cantidad de entradas de la TLB es: \033[1;30m%i\033[0;37m\n", Entradas_TLB);

	return;
}


void cfgView_HeaderDatosConfig()
{
	system("clear");
	printf("\033[0;30m============================================================\033[1;37m\n");
	printf("                    DATOS DE CONFIGURACION\n");
	printf("\033[0;30m============================================================\033[37m\n");

	return;
}
