#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include "Paginacion.h"


int calcularByteInicial(int numeroDePagina,int tamanioDePagina)
{
	int byteInicial = numeroDePagina * tamanioDePagina;
	return byteInicial;
}

int obtenerNumeroDePagina(int byteInicial,int tamanioPagina)
{
	div_t resultado;
	resultado = div(byteInicial,tamanioPagina);
	return resultado.quot;
}

bool estaEnLaLista(t_list* listaPaginas,int pagina)
{
	int i = 0;
	bool value;
	int longitud = list_size(listaPaginas);
	while(i<longitud)
	{
		int * numeroLista = list_get(listaPaginas,i);
		if(*numeroLista == pagina)
		{
			value = (*numeroLista == pagina);
			i = longitud;
		}
		else
		{
			i++;
		}
	}
	return value;
}

char* adaptarContenidoAPagina(char* datos,int tamanioDePagina)
{
	int longitudDatos = strlen(datos) + 1;
	char * datosModificados = malloc(tamanioDePagina);
	memcpy(datosModificados,datos,longitudDatos);
	if(longitudDatos < tamanioDePagina)
	{
		int diferencia = tamanioDePagina-longitudDatos;
		char * complemento = malloc(diferencia);
		memset(complemento,'\0',diferencia);
		//memset(complemento,'A',diferencia);
		memcpy(datosModificados+longitudDatos,complemento,diferencia);
		free(complemento);
	}
	return datosModificados;
}
