CODIGO :=  ../src/
BIBLIOTECAS := ../src/Headers/
OBJETOS := Configuracion.o Consola.o Entrada-Salida.o EnvioYRecepcionDeMensajes.o FuncionesArchivos.o Logger.o ManejoDeMensajes.o Proceso_Planificador.o
HEADERS := Configuracion.h Consola.h Entrada-Salida.h EnvioYRecepcionDeMensajes.h FuncionesArchivos.h Logger.h ManejoDeMensajes.h
PLANIFICADOR :=all cleanObjects


#ALL

all: Proceso_Planificador


Proceso_Planificador: $(OBJETOS)
	gcc -o	Proceso_Planificador $(OBJETOS)  -lcommons   -lpthread

Proceso_Planificador.o: $(CODIGO)Proceso_Planificador.c
	gcc -c $(CODIGO)Proceso_Planificador.c

Configuracion.o: $(CODIGO)Configuracion.c $(BIBLIOTECAS)Configuracion.h $(BIBLIOTECAS)Structs.h
	gcc -c $(CODIGO)Configuracion.c

Consola.o: $(CODIGO)Consola.c $(BIBLIOTECAS)Consola.h $(BIBLIOTECAS)Structs.h $(BIBLIOTECAS)ManejoDeMensajes.h
	gcc -c $(CODIGO)Consola.c

Entrada-Salida.o: $(CODIGO)Entrada-Salida.c $(BIBLIOTECAS)Entrada-Salida.h $(BIBLIOTECAS)Structs.h $(BIBLIOTECAS)ManejoDeMensajes.h
	gcc -c $(CODIGO)Entrada-Salida.c

EnvioYRecepcionDeMensajes.o: $(CODIGO)EnvioYRecepcionDeMensajes.c $(BIBLIOTECAS)EnvioYRecepcionDeMensajes.h $(BIBLIOTECAS)Consola.h $(BIBLIOTECAS)ManejoDeMensajes.h
	gcc -c $(CODIGO)EnvioYRecepcionDeMensajes.c

FuncionesArchivos.o: $(CODIGO)FuncionesArchivos.c $(BIBLIOTECAS)FuncionesArchivos.h
	gcc -c $(CODIGO)FuncionesArchivos.c

Logger.o: $(CODIGO)Logger.c $(BIBLIOTECAS)Logger.h $(BIBLIOTECAS)Structs.h $(BIBLIOTECAS)Entrada-Salida.h $(BIBLIOTECAS)ManejoDeMensajes.h
	gcc -c $(CODIGO)Logger.c

ManejoDeMensajes.o: $(CODIGO)ManejoDeMensajes.c $(BIBLIOTECAS)ManejoDeMensajes.h $(BIBLIOTECAS)EnvioYRecepcionDeMensajes.h $(BIBLIOTECAS)FuncionesArchivos.h $(BIBLIOTECAS)Entrada-Salida.h $(BIBLIOTECAS)Structs.h $(BIBLIOTECAS)Logger.h
	gcc -c $(CODIGO)ManejoDeMensajes.c


# Comandos

planificador:
	-make $(Planificador)

cleanObjects:
		-rm $(OBJETOS)

clean :
		-rm Proceso_Planificador
