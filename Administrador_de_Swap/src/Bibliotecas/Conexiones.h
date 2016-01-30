#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include <netinet/in.h>
#include <arpa/inet.h>


typedef enum
{
		//Mensajes que recibe
		ConexionAdmMemoria= 10,
		InicioDemProc=30,
		SolicitudDeLecturaDePagina=31,
		SolicitudDeEscrituraDePagina=32,
		LiberarmProc=33,
		//Mensaje que envia
		mProcRechazado=35,
		mProcAceptado=34,
		finEscritura=36,
}t_cabecera;


typedef struct{
	int socket;
	struct sockaddr_in direccion;
}t_conexion;


// Dada una ip y un puerto genera una direccion de tipo sockaddr_in
struct sockaddr_in generarDireccion(char*ip,int puerto);

// dadas una cabecera y un mensaje, agrega la cabecera al mensaje
void empaquetarCabecera(t_cabecera header, void* message);

// dadas una cabecera y un socket, le asigna a la cabecera, cabecera del mensaje recibido de ese socke
void desempaquetarCabecera(t_cabecera* cabecera,int socketNuevo);

//Envia un mensaje asegurandose de hacerlo completo
void enviarTodo(int unSocket, void* mensaje, int longitudMensaje);

//Dado un puntero a un socket llama a la funcion socket comprobando errores, inicializando el socket
void inicializarSocket(int * socketNuevo);

//Dado un puntero a un socket y una direccion, realiza el bind comprobando errores
void bindComprobandoError(int * unSocket, struct sockaddr_in direccion);

// dado un socket, la direccion y un socklen_t, hace el accept comprobando errores devolviendo un descriptor de socket
int aceptarComprobandoError(int unSocket,struct sockaddr_in direccionEntrante,socklen_t size);

// dado un puntero a socket y un limite de la cola para aceptar, pone a escuchar el socket comprobando errores
void escucharComprobandoErrores(int* unSocket,int valor);

#endif /* CONEXIONES_H_ */
