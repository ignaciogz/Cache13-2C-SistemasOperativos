
FORMATO DE MENSAJES QUE RECIBE EL PROCESO PLANIFICADOR:

CONEXION CPU:
	header + idCPU;
	

FORMATO DE MENSAJES QUE ENVIA EL PROCESO PLANIFICADOR:

PEDIDO DE EJECUCION SIN CORTE:
	header + largo ruta del archivo a ejecutar + ruta del archivo a ejecutar + puntero a proxima instruccion;
	
PEDIDO DE EJECUCION SIN CORTE:
	header + largo ruta del archivo a ejecutar + ruta del archivo a ejecutar + puntero a proxima instruccion + quantum;