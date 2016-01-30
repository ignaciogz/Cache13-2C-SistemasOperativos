#ifndef LOGUEOS_H_
#define LOGUEOS_H_

#include <commons/log.h>

//Dadas la ubicacion de la carpeta de los archivos y el nombre del archivo crea el archivo de logger comprobando errores. En caso de error termina el proceso mostrando un mensaje por pantalla de la causa
t_log* crearArchivoDeLogger(char* ubicacionCarpeta,char* nombreArchivo);

// escribe en el archivo de logger el pid, el byte inicial donde comienza el mProc en el archivo y la cantidad de bytes asignados
void loguearmProcCreado(int pid,int byteInicial,int cantBytes);

// escribe en el archivo de logger q un mProc fue liberado indicando su pid, el byte inicial y la cantidad de bytes que tenia asignados
void loguearmProcLiberado(int pid,int byteInicial,int cantBytes,int paginasEscritas,int paginasLeidas);

// escribe en el archivo de logger el PID de un mProc rechazado por falta de espacio
void loguearmProcRechazado(int pid);

// escribe en el archivo de logger que la compactacion inicio
void loguearCompactacionIiniciada();

// escribe en el archivo de logger que la compactacion termino
void loguearCompactacionFinalizada();

// escribe en el archivo de logger que el proceso inicio su ejecucion
void loguearInicioDeEjecucion();

// escribe en el archivo de logger una solicitud de escritura de una pagina indicando: pid, n° de byte inicial, tamanio y contenido
void loguearEscritura(int pid,int byteInicial,int numeroPagina,int inicioPagina,int tamanio,char* contenido);

// escribe en el archivo de logger una solicitud de lectura de una pagina indicando: pid, n° de byte inicial, tamanio y contenido
void loguearLectura(int pid,int byteInicial,int numeroPagina,int inicioPagina,int tamanio,char* contenido);
// imprime un error por pantalla
void imprimirError(char* error);
#endif /* LOGUEOS_H_ */
