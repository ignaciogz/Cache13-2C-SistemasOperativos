ARCHIVOS := Archivos/
CODIGO := src/
VISTAS := src/vistas/
MODELO := src/modelo/
CONTROLADORES := src/controladores/
OBJETOS := Proceso_AdmMemoria.o controladorFrontal.o recvController.o sendController.o signalController.o algoritmos.o configuracion.o estructuras.o logeador.o metricas.o seniales.o sockets.o cfgView.o mainView.o
ADM :=all cleanObjects

#ALL:
all: Proceso_AdmMemoria

Proceso_AdmMemoria: $(OBJETOS)
	gcc -o	Proceso_AdmMemoria $(OBJETOS)  -lcommons  -lpthread


#OBJETOS:
Proceso_AdmMemoria.o: $(CODIGO)Proceso_AdmMemoria.c  $(CODIGO)controladorFrontal.h $(MODELO)configuracion.h $(MODELO)estructuras.h $(MODELO)sockets.h $(VISTAS)cfgView.h $(VISTAS)mainView.h $(CONTROLADORES)sendController.h
	gcc -c $(CODIGO)Proceso_AdmMemoria.c

controladorFrontal.o: $(CODIGO)controladorFrontal.c $(CODIGO)controladorFrontal.h  $(MODELO)sockets.h $(MODELO)metricas.h $(CONTROLADORES)sendController.h $(CONTROLADORES)recvController.h $(CONTROLADORES)signalController.h
	gcc -c $(CODIGO)controladorFrontal.c

recvController.o: $(CONTROLADORES)recvController.c $(CONTROLADORES)recvController.h  $(MODELO)logeador.h $(MODELO)estructuras.h $(MODELO)algoritmos.h $(MODELO)sockets.h $(CONTROLADORES)sendController.h
	gcc -c $(CONTROLADORES)recvController.c

sendController.o: $(CONTROLADORES)sendController.c $(CONTROLADORES)sendController.h  $(MODELO)sockets.h $(VISTAS)mainView.h
	gcc -c $(CONTROLADORES)sendController.c

signalController.o: $(CONTROLADORES)signalController.c $(CONTROLADORES)signalController.h  $(MODELO)seniales.h $(MODELO)logeador.h
	gcc -c $(CONTROLADORES)signalController.c

algoritmos.o: $(MODELO)algoritmos.c $(MODELO)algoritmos.h  $(MODELO)logeador.h $(MODELO)estructuras.h $(CONTROLADORES)sendController.h
	gcc -c $(MODELO)algoritmos.c

configuracion.o: $(MODELO)configuracion.c $(MODELO)configuracion.h
	gcc -c $(MODELO)configuracion.c

estructuras.o: $(MODELO)estructuras.c $(MODELO)estructuras.h  $(MODELO)algoritmos.h
	gcc -c $(MODELO)estructuras.c

logeador.o: $(MODELO)logeador.c $(MODELO)logeador.h  $(MODELO)estructuras.h
	gcc -c $(MODELO)logeador.c

metricas.o: $(MODELO)metricas.c $(MODELO)metricas.h
	gcc -c $(MODELO)metricas.c

seniales.o: $(MODELO)seniales.c $(MODELO)seniales.h  $(MODELO)estructuras.h
	gcc -c $(MODELO)seniales.c

sockets.o: $(MODELO)sockets.c $(MODELO)sockets.h
	gcc -c $(MODELO)sockets.c

cfgView.o: $(VISTAS)cfgView.c $(VISTAS)cfgView.h
	gcc -c $(VISTAS)cfgView.c

mainView.o: $(VISTAS)mainView.c $(VISTAS)mainView.h
	gcc -c $(VISTAS)mainView.c

Configuracion.o: $(BIBLIOTECAS)Configuracion.c $(BIBLIOTECAS)Configuracion.h
	gcc -c $(BIBLIOTECAS)Configuracion.c


#Comandos:
AdmMemoria:
	-make $(ADM)

cleanObjects:
	-rm $(OBJETOS)	

clean:
	-rm Proceso_AdmMemoria

cleanFiles:
	-rm -rf $(ARCHIVOS)/logger.log $(ARCHIVOS)/var_dump.log