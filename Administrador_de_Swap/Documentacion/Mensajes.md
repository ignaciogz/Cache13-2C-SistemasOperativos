###MENSAJES RECIBIDOS


## Nuevo proceso mProc

Recibe un mensaje del estilo:

| Header | PID | CantidadDePaginas |

## Solicitud de lectura de pagina

Recibe un mensaje del estilo:

| Header | PID | NumeroDePagina |

## Solicitud de escritura de pagina

Recibe un mensaje del estilo:

| Header | PID | NumeroDePagina | longitudDatos | Datos |

## Solicitud de eliminar mProc

Recibe un mensaje del estilo:

| Header | PID |


###MENSAJES ENVIADOS

## Contenido de una pagina

enviar al Administrador de memoria un mensaje del estilo:

| TamanioDelContenido | Contenido |

## Error al buscar hueco para un mProc (falta de espacio)

envia al Administrador de memoria un mensaje del estilo:

| Header |

## mProc aceptado (memoria asignada con exito)

envia al Administrador de memoria un mensaje del estilo:

| Header |