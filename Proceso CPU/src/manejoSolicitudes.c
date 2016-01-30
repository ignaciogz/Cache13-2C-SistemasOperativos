/*
 * manejoSolicitudes.c
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "sockets.h"
#include "manejoSolicitudes.h"
#include "interfazMemoria.h"
#include "Log.h"
#define TAM 200


extern __thread pthread_mutex_t CPU;

void ejecutarSolicitud(struct sockaddr_in  acceptAddr,t_header header,t_conexion conexion,t_datos data){
	int tamanio;
	char*nombre;

	switch(header){

	case CorrerProgramaSinCorte:	recv(conexion.i, &tamanio, sizeof(int), 0);
									nombre= malloc(tamanio);
									memset(nombre, '\0', tamanio);
									recv(conexion.i, nombre, tamanio, 0);
									recv(conexion.i, &(data.puntero), sizeof(int), 0);
									recv(conexion.i, &(data.PID), sizeof(int), 0);
									data.quantum = -1;
									logearContextoEjecucion(data,nombre);
									ejecutarPrograma(conexion,tamanio,nombre,data);
									break;

	case CorrerProgramaConCorte:	recv(conexion.i, &tamanio, sizeof(int), 0);
									nombre= malloc(tamanio);
									memset(nombre, '\0', tamanio);
									recv(conexion.i, nombre, tamanio, 0);
									recv(conexion.i, &(data.puntero), sizeof(int), 0);
									recv(conexion.i,&(data.quantum), sizeof(int), 0);
									recv(conexion.i, &(data.PID), sizeof(int), 0);
									logearContextoEjecucion(data,nombre);
									ejecutarPrograma(conexion,tamanio,nombre,data);
									break;

	default: break;

	}

return;
}


void consultarMem(int socketAccept,struct sockaddr_in  acceptAddr){
	t_header cabecera = EjecutarInstruccion;
	int tamanio = sizeof(t_header);
	void* mensaje = malloc(sizeof(t_header));
	packHeader(cabecera,mensaje);
	if((sendall(socketAccept,mensaje,&tamanio))==-1){
		printf("Se desconecto la memoria");
	}
free(mensaje);
return;

}

void finalizarPrograma(int socketAccept,struct sockaddr_in  acceptAddr){
	t_header cabecera = Finalizar;
	int tamanio = sizeof(t_header);
	void* mensaje = malloc(sizeof(t_header));
	packHeader(cabecera,mensaje);
	if((sendall(socketAccept,mensaje,&tamanio))==-1){
		printf("Se desconecto la memoria");
	}
free(mensaje);
return;

}

void avisarFinPlanif(t_conexion conexion){
	int sockPlanif;
	if((sockPlanif = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(connect(sockPlanif, (struct sockaddr *)&conexion.planifAddr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("connect");
		//logear no poder conectarse con proceso y finalizar proceso Cpu
		exit(1);
	}


	t_header cabecera = Finalizar;
	int tamanio = sizeof(t_header);
	void* mensaje = malloc(sizeof(t_header));
	packHeader(cabecera,mensaje);
	if((sendall(sockPlanif,mensaje,&tamanio))==-1){
		printf("Se desconecto Planificador");
	}

free(mensaje);
return;

}

void ejecutarPrograma(t_conexion conexion,int tamanio,char* nombre,t_datos data){
	FILE* fichero;
	char letra;
	t_retorno retornoDatos;
	int resultadoInicio;
	bool Es = false;
	/*inicializo datos para controlar el tiempo de instrucciones*/
	time_t *tiempo1 = malloc(sizeof(time_t));
	time_t *tiempo2 = malloc(sizeof(time_t));
	double tiempoInicioInstruccion = 0;
	double tiempoFinInstruccion = 0;
	int tiempoTranscurridoInst=0;

	retornoDatos.offset = 0;
	char palabra[TAM];
	memset(&palabra,'\0',TAM);
	char* escribir = malloc(TAM);
	int i,contador,tamanioContenido;

	fichero = fopen(nombre,"rt");
	if(fichero != NULL){

	acomodarPuntero(data.puntero,fichero);
	if(data.puntero >0)
		retornoDatos.mensaje = malloc(sizeof(t_operaciones));

	tiempoInicioInstruccion=time(tiempo1);
	letra = fgetc(fichero);
	vaciarCadena(palabra);
	i=0;
	while((letra !=' ') && (letra !=';')){
		palabra[i] = letra;
		i++;
		letra = fgetc(fichero);

	}

	contador = 0;
	while((strcmp(palabra,"finalizar")!=0) && data.quantum != contador){


		if(strcmp(palabra,"iniciar")==0){
			//tiempoInicioInstruccion=time(tiempo1);
			letra = fgetc(fichero);
			vaciarCadena(palabra);
			i=0;
			while(letra !=';'){
				palabra[i] = letra;
				i++;
				letra = fgetc(fichero);
			}

			data.valor = atoi(palabra);
			usleep(data.retardo*1000000);
			resultadoInicio = mandarIniciarMemoria(conexion,data,&retornoDatos);
			if(resultadoInicio == 25){
				break;
			}
			contador++;
			tiempoFinInstruccion=time(tiempo2);
			tiempoTranscurridoInst=tiempoFinInstruccion - tiempoInicioInstruccion;
			pthread_mutex_lock(&CPU);
			conexion.cpu->tiempoInstruccion=conexion.cpu->tiempoInstruccion + tiempoTranscurridoInst;
			pthread_mutex_unlock(&CPU);
		}

		if(strcmp(palabra,"leer")==0){
			//tiempoInicioInstruccion=time(tiempo1);
			letra = fgetc(fichero);
			vaciarCadena(palabra);
			i=0;
			while(letra !=';'){
			palabra[i] = letra;
			i++;
			letra = fgetc(fichero);
			}

			data.valor = atoi(palabra);
			usleep(data.retardo*1000000);
			mandarALeerMem(conexion,data,&retornoDatos);
			contador++;
			tiempoFinInstruccion=time(tiempo2);
			tiempoTranscurridoInst=tiempoFinInstruccion - tiempoInicioInstruccion;
			pthread_mutex_lock(&CPU);
			conexion.cpu->tiempoInstruccion=conexion.cpu->tiempoInstruccion + tiempoTranscurridoInst;
			pthread_mutex_unlock(&CPU);
		}

		if(strcmp(palabra,"escribir")==0){
			//tiempoInicioInstruccion=time(tiempo1);
			letra = fgetc(fichero);
			vaciarCadena(palabra);
			i=0;
			while(letra !=' '){
				palabra[i] = letra;
				i++;
				letra = fgetc(fichero);
			}
			data.valor = atoi(palabra);

			fgetc(fichero);
			letra = fgetc(fichero);
			vaciarCadena(palabra);
			i=0;
			while(letra !='"'){
				palabra[i] = letra;
				i++;
				letra = fgetc(fichero);

			}

			tamanioContenido = strlen(palabra) + 1 ;
			memset(escribir,'\0',TAM);
			memcpy(escribir,(char*)&palabra,tamanioContenido);
			usleep(data.retardo*1000000);
			mandarAEscribirMem(conexion,data,escribir,tamanioContenido, &retornoDatos);
			fgetc(fichero);
			contador++;
			tiempoFinInstruccion=time(tiempo2);
			tiempoTranscurridoInst=tiempoFinInstruccion - tiempoInicioInstruccion;
			pthread_mutex_lock(&CPU);
			conexion.cpu->tiempoInstruccion=conexion.cpu->tiempoInstruccion + tiempoTranscurridoInst;
			pthread_mutex_unlock(&CPU);

		}

		if(strcmp(palabra,"entrada-salida")==0){
			//tiempoInicioInstruccion=time(tiempo1);
			letra = fgetc(fichero);
			vaciarCadena(palabra);
			Es = true;
			i=0;
			while(letra !=';'){
				palabra[i] = letra;
				i++;
				letra = fgetc(fichero);
			}
			usleep(data.retardo*1000000);
			data.valor = atoi(palabra);
			contador++;
			tiempoFinInstruccion=time(tiempo2);
			tiempoTranscurridoInst=tiempoFinInstruccion - tiempoInicioInstruccion;
			pthread_mutex_lock(&CPU);
			conexion.cpu->tiempoInstruccion=conexion.cpu->tiempoInstruccion + tiempoTranscurridoInst;
			pthread_mutex_unlock(&CPU);
			finRafaga(conexion,data,contador,&retornoDatos);
			break;

		}

		if(contador == data.quantum){
			break;
		}

		tiempoInicioInstruccion=time(tiempo1);
		vaciarCadena(palabra);
		fgetc(fichero);
		letra = fgetc(fichero);
		i=0;
		while((letra !=' ') && (letra !=';')){
			palabra[i] = letra;
			i++;
			letra = fgetc(fichero);
		}

}
	if(data.quantum == contador && !Es){
		finalizado(conexion,contador,&retornoDatos);
	}
	else
	{
		if(strcmp(palabra,"finalizar")==0){
			//tiempoInicioInstruccion=time(tiempo1);
			mandarFinalizarMem(conexion,data,&retornoDatos);
			contador++;
			tiempoFinInstruccion=time(tiempo2);
			tiempoTranscurridoInst=tiempoFinInstruccion - tiempoInicioInstruccion;
			pthread_mutex_lock(&CPU);
			conexion.cpu->tiempoInstruccion=conexion.cpu->tiempoInstruccion + tiempoTranscurridoInst;
			pthread_mutex_unlock(&CPU);
			finalizado(conexion,contador,&retornoDatos);
	}
	}

	}else{
		printf("Amigo esa ruta no existe");
		rutaArchivoEquivocada(conexion);
	}
	close(tiempo1);
	close(tiempo2);

	return;
}

char* vaciarCadena(char* cadena){
	//int tamanio = strlen(cadena);
	int i;
	for(i=0; i < TAM; i++){
		*(cadena+i)='\0';
	}
return cadena;
}

void acomodarPuntero(int puntero,FILE* fichero){
	int i;
	char *line = NULL;
	size_t linecap = 0;
	if(puntero != 0){
		for(i=0 ;i < puntero; i++){
			getline(&line,&linecap,fichero);
		}
	}
}






