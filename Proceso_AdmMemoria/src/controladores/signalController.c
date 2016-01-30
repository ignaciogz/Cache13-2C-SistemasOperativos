#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "signalController.h"
#include "../modelo/seniales.h"
#include "../modelo/logeador.h"

void signalINI() {
	//Enviamos las 3 señales a nuestro propio manejador para tratarlas =)
		signal (SIGUSR1, signalHandler);
		signal (SIGUSR2, signalHandler);
		signal (SIGPOLL, signalHandler);
	return;
}

void signalHandler(int nSignal) {
	pthread_t tSignalHandler;

	switch (nSignal) {
		case SIGUSR1:
			printf("> SIGUSR1, aplicando Flush TLB\n");
			LOG_senialRecibidad("SIGUSR1", "Aplicar Flush TLB");
				pthread_create(&tSignalHandler, NULL, (void*)&actionflushTLB, NULL);
				pthread_join(tSignalHandler, NULL);
			LOG_senialTratada("SIGUSR1");
			printf("> SIGUSR1, terminado\n");
		break;
		case SIGUSR2:
			printf("> SIGUSR2, aplicando Flush Memoria\n");
			LOG_senialRecibidad("SIGUSR2", "Aplicar Flush Memoria");
				pthread_create(&tSignalHandler, NULL, (void*)&actionflushMemoria, NULL);
				pthread_join(tSignalHandler, NULL);
			LOG_senialTratada("SIGUSR2");
			printf("> SIGUSR2, terminado\n");
		break;
		case SIGPOLL:
			printf("> SIGPOLL, aplicando Var Dump\n");
			LOG_senialRecibidad("SIGPOLL", "Aplicar VarDump");
				actionVarDump();
			LOG_senialTratada("SIGPOLL");
			printf("> SIGPOLL, terminado\n");
		break;
	}

	signalINI();
	return;
}

//SIGUSR1
	void actionflushTLB()
	{
		flushTLB();
		return;
	};

//SIGUSR2
	void actionflushMemoria()
	{
		flushMemoria();
		return;
	};

//SIGPOLL
	void actionVarDump()
	{
		int pid;
		pid = fork();

			if (pid == 0)
			{// HIJO
				varDumpMemoria();
			}
			else if (pid > 0)
			{// PADRE
				wait(&pid);
			}
			else if (pid < 0){ 
				perror("Forkeando");
			}
		return;
	}
