#ifndef ALGORITMOS_H_
#define ALGORITMOS_H_
	
#include "estructuras.h"

void algoritmo_reemplazo_pagina(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);

/* --------------------  Algoritmos de REEMPLAZO DE PAGINAS  -------------------------- */
void algoritmo_FIFO(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);
	
void algoritmo_LRU(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);

void algoritmo_CLOCKMod(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);


/* -----------------------  Algoritmos de la TLB  ----------------------------- */
int algoritmo_FirstFit_TLB();

	int algoritmo_SeleccionVictima_TLB();


/* ------------------------  Algoritmos de MEMORIA  --------------------------- */
int algoritmo_FirstFit_MEMORIA();


/* ------------------------  FAuxs de Algoritmos de REEMPLAZO DE PAGINAS  --------------------------- */
/*int estaEnLaCola(int numeroDePagina, t_queue* colitaDeMarcos);*/

int posicionDelPuntero(t_proceso* ProcesoX);

void ponerBitDeUso(int numeroDePagina, t_queue* colitaDeMarcos);

void ponerBitDeModificado(int numeroDePagina, t_queue* colitaDeMarcos);

void cambiarBitUso(t_infoDeMarco* elementoDeLaCola, int nuevoValor);

void cambiarBitMod(t_infoDeMarco* elementoDeLaCola, int nuevoValor);

t_queue* obtenerColaInicial(t_queue* colaOriginal);


/* ------------------------  Algoritmos de SELECCION y TRATAMIENTO de marcos victima  ------------------------ */
void algoritmo_victima_FIFO(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);

void algoritmo_victima_LRU(int numeroDePagina, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);

void algoritmo_victima_CLOCKMod(int numeroDePagina, int cantElementos, t_proceso* ProcesoX, char* contenido, int PID, char TipoOperacion);

#endif
