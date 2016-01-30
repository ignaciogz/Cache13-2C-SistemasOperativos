#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <pthread.h>

#include "estructuras.h"
#include "seniales.h"

//Variables de LOG:
	extern t_log * archivoVarDump;
//Variables de configuracion:
	extern int Cant_Marcos;
	extern int Max_Marcos_Por_Proceso;
//Estructuras IMPORTANTES del proceso:
	extern t_dictionary* dicInfoProcesos;

pthread_mutex_t mutexVarDump = PTHREAD_MUTEX_INITIALIZER;


void flushTLB()
{
	vaciar_TLB();
	return;
}


void flushMemoria()
{	
	dictionary_iterator(dicInfoProcesos, flush);
	return;
}


void varDumpMemoria()
{
	int marcoActual;

	char * ejecucionIniciada = string_new();
		string_append(&ejecucionIniciada, "**** VAR DUMP EJECUTADO ****\n");
	
	pthread_mutex_lock(&mutexVarDump);
		log_info(archivoVarDump, ejecucionIniciada);
	pthread_mutex_unlock(&mutexVarDump);

	
	//Mostrando el contenido de los marcos:
	for (marcoActual = 0; marcoActual < Cant_Marcos; marcoActual++)
	{
		char * mensaje = string_new();
		char * contenidoDeMarco = obtenerContenido_MemoriaPrincipal(marcoActual);
		
			char * nro_marco = string_itoa(marcoActual);
			string_append(&mensaje,"Marco N");
				string_append(&mensaje, nro_marco);
			string_append(&mensaje, " -> ");
				string_append(&mensaje, contenidoDeMarco);

		pthread_mutex_lock(&mutexVarDump);
			log_info(archivoVarDump, mensaje);
		pthread_mutex_unlock(&mutexVarDump);

		free(nro_marco);
		free(mensaje);
	}
	return;
}


void flush(char* Key, void* Proceso)
{
	int i;
	t_proceso* ProcesoX = Proceso;
	t_infoDeMarco* elementoDeLaCola;

	//Obtengo la referencia a cada elemento del proceso:
	int** tablaDePaginas = (*ProcesoX).tablaDePaginas;
	t_list* marcosAsignados = (*ProcesoX).marcosAsignados;
	t_queue * marcos = (*ProcesoX).marcos;
	
	//Reseteando los elementos necesarios:
		vaciar_TLB();
		list_iterate(marcosAsignados, resetearMarco);
		for (i = 0; i < Max_Marcos_Por_Proceso; i++)
		{
			elementoDeLaCola = queue_pop(marcos);
				resetear_TP(tablaDePaginas, (*elementoDeLaCola).NumeroPagina);
			queue_push(marcos, elementoDeLaCola);
		}
		queue_clean(marcos);
	return;
}