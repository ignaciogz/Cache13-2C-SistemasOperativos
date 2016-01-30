/*
 * controladorFrontal.h
 *
 *  Created on: -
 *      Author: utnso
 */

#ifndef CONTROLADORFRONTAL_H_
#define CONTROLADORFRONTAL_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "modelo/sockets.h"
	//
	void actionEscucharSolicitudesDelCPU(int socketMemoria, struct sockaddr_in myAddr);

	//
	void actionResponderSolicitud(t_data * datosDeSolicitud);

#endif
