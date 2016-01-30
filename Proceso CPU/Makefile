CODIGO :=  src/
OBJETOS := proceso_CPU.o configuracion.o interfazMemoria.o Log.o manejoSolicitudes.o sockets.o ejecucionCpu.o calculoPorcentaje.o
HEADERS := configuracion.h interfazMemoria.h Log.h manejoSolicitudes.h sockets.h ejecucionCpu.h calcularPorcentaje.h
CPU :=all cleanObjects

#ALL

all: Proceso\ CPU


Proceso\ CPU: $(OBJETOS)
	gcc -o	Proceso\ CPU $(OBJETOS)  -lcommons   -lpthread

proceso_CPU.o: $(CODIGO)proceso_CPU.c
	gcc -c $(CODIGO)proceso_CPU.c

configuracion.o: $(CODIGO)configuracion.c $(CODIGO)configuracion.h
	gcc -c $(CODIGO)configuracion.c

interfazMemoria.o: $(CODIGO)interfazMemoria.c $(CODIGO)interfazMemoria.h $(CODIGO)Log.h $(CODIGO)sockets.h
	gcc -c $(CODIGO)interfazMemoria.c

Log.o: $(CODIGO)Log.c $(CODIGO)Log.h $(CODIGO)sockets.h
	gcc -c $(CODIGO)Log.c

manejoSolicitudes.o: $(CODIGO)manejoSolicitudes.c $(CODIGO)manejoSolicitudes.h $(CODIGO)sockets.h $(CODIGO)Log.h $(CODIGO)interfazMemoria.h
	gcc -c $(CODIGO)manejoSolicitudes.c

sockets.o: $(CODIGO)sockets.c $(CODIGO)sockets.h
	gcc -c $(CODIGO)sockets.c

ejecucionCpu.o: $(CODIGO)ejecucionCpu.c $(CODIGO)ejecucionCpu.h $(CODIGO)sockets.h $(CODIGO)Log.h $(CODIGO)configuracion.h $(CODIGO)manejoSolicitudes.h $(CODIGO)calculoPorcentaje.h 
	gcc -c $(CODIGO)ejecucionCpu.c 

calculoPorcentaje.o: $(CODIGO)calculoPorcentaje.c $(CODIGO)calculoPorcentaje.h $(CODIGO)sockets.h
	gcc -c $(CODIGO)calculoPorcentaje.c


# Comandos

cpu:
	-make $(CPU)

cleanObjects:
	-rm $(OBJETOS)	

clean :  
	-rm Proceso\ CPU