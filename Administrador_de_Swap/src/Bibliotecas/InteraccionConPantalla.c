#include <stdio.h>
#include <stdlib.h>
#include "InteraccionConPantalla.h"
#include <commons/string.h>
// Efectos letra
#define NORMAL    "\033[0;"
#define NEGRITA   "\033[1;"
#define SUBRAYADO "\033[4;"
#define TACHADO   "\033[9;"
#define NINGUNO  "\033[0m"
//Colores

#define NEGRO    "30m"
#define ROJO     "31m"
#define VERDE    "32m"
#define AMARILLO "33m"
#define AZUL     "34m"
#define MAGENTA  "35m"
#define CYAN     "36m"
#define GRIS     "37m"


void mostrarInicioDelProceso()
{
	printf("%s%sProceso Administrador de Swap iniciado\n%s",NEGRITA,AZUL,NINGUNO);
	printf("\n");
	printf("cargando configuracion...\n");
	printf("\n");
}


void mostrarEsperaDeConexionDelAdmDeMemoria()
{
	printf("\n");
	printf("Esperando conexion del Administrador de Memoria..\n");
	printf("\n");
}


void mostrarQueEstaOperativo()
{

	refrescarPantalla();
	printf("%s%sConexion con el Administrador de Memoria establecida\n",NEGRITA,AMARILLO);
	printf("\n");
	printf("El proceso se encuentra operativo y listo para recibir conexiones...\n%s",NINGUNO);
	printf("\n");
	printf("Para mayor informacion ver el archivo de log\n");
}


void mostrarInfoCongiguracion(struct sockaddr_in direccion,t_datosConfiguracion datos)
{
	 printf("%s%s==================================================\n",NEGRITA,AZUL);
	 printf("     INFORMACION DEL ARCHIVO DE CONFIGURACION     \n");
	 printf("==================================================%s\n",NINGUNO);
	 printf("\n");
	 printf("Direccion\n");
	 printf("\n");
	 mostrarIp(direccion);
	 mostrarPuerto(direccion);
	 printf("..................................................\n");
	 printf("\n");
	 printf("Informacion de las paginas\n");
	 printf("\n");
	 printf("%s%sCANTIDAD DE PAGINAS%s: %s%s%d\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,datos.cantidadPaginas,NINGUNO);
	 printf("%s%sTAMANIO POR PAGINA%s: %s%s%d\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,datos.tamanioPagina,NINGUNO);
	 printf("..................................................\n");
	 printf("\n");
	 printf("Manejo de memoria\n");
	 printf("\n");
	 printf("%s%sNOMBRE DEL ARCHIVO DE SWAPPING%s: %s%s%s\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,datos.nombreSwap,NINGUNO);
	 printf("%s%sRETARDO DE COMPACTACION%s: %s%s%.2lf\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,datos.retardoCompactacion,NINGUNO);
	 printf("%s%sRESTARDO DE LECTURA/ESCRITURA%s: %s%s%.2lf\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,datos.retardoSwap,NINGUNO);
	 printf("..................................................\n");
}

void refrescarPantalla()
{
	system("clear");
	printf("%s%sProceso Administrador de Swap iniciado\n%s",NEGRITA,AZUL,NINGUNO);
	printf("\n");
}

void mostrarFinDemProc(int pid,int paginasEscritas,int paginasLeidas)
{
	refrescarPantalla();
	printf("%s%sMPROC FINALIZADO\n%s",NEGRITA,VERDE,NINGUNO);
	printf("\n");
	printf("%s%sPID del mPRoc%s: %s%s %d\n %s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,VERDE,pid,NINGUNO);
	printf("\n");
	printf("%s%sCantidad de paginas escritas%s: %s%s %d\n %s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,VERDE,paginasEscritas,NINGUNO);
	printf("\n");
	printf("%s%sCantidad de paginas leidas%s: %s%s %d\n %s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,VERDE,paginasLeidas,NINGUNO);
	printf("\n");
}

void mostrarIp(struct sockaddr_in direccion)
{
	printf("%s%sIP%s:  %s%s%s\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,inet_ntoa(direccion.sin_addr),NINGUNO);
}


void mostrarPuerto(struct sockaddr_in direccion)
{
	printf("%s%sPUERTO%s: %s%s%d\n%s",SUBRAYADO,GRIS,NINGUNO,NEGRITA,AZUL,ntohs(direccion.sin_port),NINGUNO);
}

void imprimirError(char* error)
{
	char * mensaje = string_new();
	string_append(&mensaje,"[ERROR] ");
	string_append(&mensaje,error);
	printf("%s%s %s\n %s",NEGRITA,ROJO,mensaje,NINGUNO);
	free(mensaje);
}
