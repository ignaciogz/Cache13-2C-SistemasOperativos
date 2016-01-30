CODIGO :=  src/
BIBLIOTECAS := src/Bibliotecas/
ARCHIVOS := Archivos/
OBJETOS := Administrador_de_Swap.o Conexiones.o Configuracion.o InteraccionConPantalla.o Logueos.o ManejoSwap.o Mensajes.o Paginacion.o Paquetes.o
HEADERS := Conexiones.h  Configuracion.h  InteraccionConPantalla.h Logueos.h ManejoSwap.h Mensajes.h Paginacion.h Paquetes.h
SWAP :=all cleanObjects

#ALL

all: Administrador_de_Swap


Administrador_de_Swap: $(OBJETOS)
	gcc -o	Administrador_de_Swap $(OBJETOS)  -lcommons   -lpthread

Administrador_de_Swap.o: $(CODIGO)Administrador_de_Swap.c
	gcc -c $(CODIGO)Administrador_de_Swap.c 

Conexiones.o: $(BIBLIOTECAS)Conexiones.c $(BIBLIOTECAS)Conexiones.h
	gcc -c $(BIBLIOTECAS)Conexiones.c

Configuracion.o: $(BIBLIOTECAS)Configuracion.c $(BIBLIOTECAS)Configuracion.h
	gcc -c $(BIBLIOTECAS)Configuracion.c

InteraccionConPantalla.o: $(BIBLIOTECAS)InteraccionConPantalla.c $(BIBLIOTECAS)InteraccionConPantalla.h $(BIBLIOTECAS)Configuracion.h
	gcc -c $(BIBLIOTECAS)InteraccionConPantalla.c

Logueos.o: $(BIBLIOTECAS)Logueos.c $(BIBLIOTECAS)Logueos.h
	gcc -c $(BIBLIOTECAS)Logueos.c

ManejoSwap.o: $(BIBLIOTECAS)ManejoSwap.c $(BIBLIOTECAS)ManejoSwap.h $(BIBLIOTECAS)Paginacion.h $(BIBLIOTECAS)Logueos.h
	gcc -c $(BIBLIOTECAS)ManejoSwap.c

Mensajes.o: $(BIBLIOTECAS)Mensajes.c $(BIBLIOTECAS)Mensajes.h $(BIBLIOTECAS)InteraccionConPantalla.h $(BIBLIOTECAS)Paquetes.h $(BIBLIOTECAS)Logueos.h
	gcc -c $(BIBLIOTECAS)Mensajes.c

Paginacion.o: $(BIBLIOTECAS)Paginacion.c $(BIBLIOTECAS)Paginacion.h
	gcc -c $(BIBLIOTECAS)Paginacion.c 

Paquetes.o: $(BIBLIOTECAS)Paquetes.c $(BIBLIOTECAS)Paquetes.h $(BIBLIOTECAS)Conexiones.h
	gcc -c $(BIBLIOTECAS)Paquetes.c

# Comandos

swap:
	-make $(SWAP)

cleanObjects:
	-rm $(OBJETOS)	

clean :  
	-rm Administrador_de_Swap

cleanFiles:
	-rm -rf $(ARCHIVOS)/swap.data $(ARCHIVOS)/Logger.log