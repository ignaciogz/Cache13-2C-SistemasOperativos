### PLANIFICACION DEL PROCESO ADMINISTRADOR DE SWAP


##1) Ponerse operativo

Se inicializan todas las estructuras necesarias: 
Se crean las listas de los espacios, se levantan los datos del archivo de configuración y se crea el archivo de swap (cuyo nombre es el del 
archivo de configuración y su tamaño es la cantidad de paginas multiplicada por el tamaño de una página). 
Se loguea que el proceso está iniciado.
Se llena el archivo de swap con el caracter '/0' y se agrega a la lista que maneja los espacios vacíos un elemento cuyo comienzo es el byte 0 y una 
cantidad de páginas representadas igual a la cantidad de páginas totales. Esto último se hace a fin de indicar que en un principio, el archivo de 
swap es considerado como un único hueco vacío. Además se inicializan las estructuras para el manejo de sockets. Una vez hecho todo esto el proceso 
se queda a la esperar de la conexión del proceso administrador de memoria. Cualquier otra conexión recibida es ignorada. Una vez que llega la 
conexión del administrador de memoria, el proceso muestra por pantalla que está operativo y se queda a la espera de nuevas conexiones.


##2) Reservar espacio para un mProc

El proceso recibe un mensaje del administrador de memoria que le indica la creación de un nuevo mProc.
Se busca un hueco donde entre: si hay uno, se fija si es necesario compactar(se iniciada en el logger el comienzo y el fin de la misma), lo agrega a 
la lista de espacios ocupados y actualiza la lista de espacios vacíos (ver el titulo correspondiente para mas detalles). Si no hay espacio se rechaza el mProc. En cualquiera de lasdos opciones se 
informa al proceso administrador de memoria lo sucedido y se lo escribe en el logger.


##3) Liberar mProc

El proceso recibe un mensaje del administrador de memoria que le indica el fin de un nuevo mProc.
Se busca en la lista de espacios ocupados dicho mProc mediante el PID, se lo elimina de la lista y se lo guarda en una variable.
Luego se crea un elemento de espacio libre con comienzo y cantidad de páginas similares al del espacio ocupado eliminado y se lo agrega a la lista 
de espacios libres. Por último se escribe en el logger lo sucedido.


##4) Escribir página

El proceso recibe un mensaje del administrador de memoria que le indica la escritura de una página.
Se abre el archivo de swap para lectura/escritura comprobando errores (en caso de error termina el proceso) y se calcula el byte donde comienza la 
página.
Se realiza el mmap comprobando errores (en caso de error termina el proceso).
Se cierra el descriptor de archivo.
Se cierra el descriptor del archivo, se copian los datos en el puntero devuelto por el mmap y se libera la información que estaba en memoria.
Por último se busca, en la lista de espacios ocupados, el mProc que contiene la página para obtener el PID y se loguea la escritura de la página.


##5) Leer página

El proceso recibe un mensaje del administrador de memoria que le indica la lectura de una página.
Se abre el archivo de swap para solo lectura comprobando errores (en caso de error termina el proceso).
Se realiza el mmap comprobando errores (en caso de error termina el proceso), se cierra el descriptor de archivo y se copian los datos a una variable 
que luego será devuelta por la función.
Se libera la memoria y se cierra el descriptor de archivo.
Por último se loguea lo sucedido y se envían los datos de la página al proceso administrador de memoria.


##6) Actualización de los espacios vacios

Cuando se encuentra un hueco capaz de albergar a un mProc se agrega un espacio ocupado que lo representa a la lista de espacios ocupados. Luego de esto es necesario actualizar la lista de espacios vacios de la siguiente forma:
Si la cantidad de páginas que requiere un mProc es igual a la cantidad de páginas que representa el hueco encontrado, entonces se elimina el hueco de la lista de espacios vacios.
Si la cantidad de páginas que requiere un mProc es menor a la cantidad de páginas que representa el hueco encontrado, se elimina el hueco de la lista de espacios vacios pero con una diferencia: 
Además de eliminarlo, se agrega un espacio libre a la lista de espacios vacios cuya cantidad de páginas será la cantidad de páginas que el mPRoc no ocupó y su inicio será el byte siguiente al último byte asignado al mProc.


##7) Compactación

Cuando haya espacio suficiente para albergar un mProc pero este no sea contiguo, el proceso iniciará la compactación.

Momento de compactar

PID 1| Hueco 1 | PID 2 |Hueco 2 | PID 3 | Hueco 3 | PID 4
--- | --- | --- | --- | --- | ---- | --- 
          
Sean los siguientes bytes:

+ A = Byte anterior al primer byte del mProc con PID 2
+ B = Byte siguiente al último byte del mProc de PID 3
+ C = Byte anterior al primer byte del mProc de PID 4 


Pasos a seguir

Todos los espacios (tanto ocupados como vacíos) que estén entre A y C van a ser movidos. 
La idea es poner los espacios vacios contiguos a continuación de hueco 1 (que es el de byte inicial mas chico) y combinarlos en uno solo.
Luego poner los espacios ocupados correspondientes a continuación del hueco generado.

1- Actualizar las listas

Lista de espacios vacíos 
- ordenarla de menor a mayor según su byte inicial
- agrupar los espacios libres en uno solo (comenzarán del primer espacio libre de la lista o sea el que termina en A

Lista de espacios Ocupados 
- de esta solo hay que mover los que estén entre A y B
- ordenarla de mayor a menor según su byte inicial
- el último elemento debe ser actualizado de modo que su byte fnal sea C
- el resto de los elementos se actualizaran de modo que queden contiguos con el mencionado antes

2- Actualizar el archivo de Swap

Una vez que se tienen las listas actualizadas hay que mover las paginas de los mProcs correspondientes a su nueva ubicación(los libres no me interesa porque ya fueron marcados como desocupados). 
Esta tarea se realizará recorriendo la lista de espacios ocupados moviendo primero la última página del espacio ocupado y luego las otras hasta llegar a la primera de modo que no se pisen los datos (la lista de espacios ocupados está ordenada de mayor a menor según su byte inicial asique el primer elemento de la lista es el que va a terminar en C por lo que no se estarían pisando datos).

Resultado

PID 1 |Hueco compactado | PID 2 | PID 3 | PID 4
--- | --- | --- | --- | --- 
