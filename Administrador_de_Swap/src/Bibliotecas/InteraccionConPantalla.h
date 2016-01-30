#ifndef BIBLIOTECAS_INTERACCIONCONPANTALLA_H_
#define BIBLIOTECAS_INTERACCIONCONPANTALLA_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include "Configuracion.h"


//muestra por pantalla que inicio el proceso
void mostrarInicioDelProceso();

//muestra por pantalla que el proceso se encuentra a la espera de una conexion del Administrador de Memoria
void mostrarEsperaDeConexionDelAdmDeMemoria();

//muestra por pantalla que el proceso esta operativo y listo para recibir conexiones
void mostrarQueEstaOperativo();

//Dadas la direccion y una variable con los datos del archivo de configuracion, los muestra por pantalla
void mostrarInfoCongiguracion(struct sockaddr_in direccion,t_datosConfiguracion datos);

//Refescar la pantalla
void refrescarPantalla();

//Muestra por pantalla la informacion de un mProc finalizado
void mostrarFinDemProc(int pid,int paginasEscritas,int paginasLeidas);

//Dada una direccion muestra por pantalla su ip
void mostrarIp(struct sockaddr_in direccion);

//Dada una direccion muestra por pantalla su puerto
void mostrarPuerto(struct sockaddr_in direccion);

void imprimirError(char* error);
#endif /* BIBLIOTECAS_INTERACCIONCONPANTALLA_H_ */
