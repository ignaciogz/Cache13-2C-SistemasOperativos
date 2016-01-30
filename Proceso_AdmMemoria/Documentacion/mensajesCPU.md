MENSAJES RECIBIDOS DEL CPU
==================================================

## Nuevo proceso mProc

	Recibo un mensaje del estilo:

	| Header | PID | CantidadDePaginas | CPU |

## Solicitud de lectura de pagina

	Recibo un mensaje del estilo:

	| Header | PID | NumeroDePagina | CPU |

## Solicitud de escritura de pagina

	Recibo un mensaje del estilo:

	| Header | PID | NumeroDePagina | longitudDatos | Datos | CPU |

## Solicitud de eliminar mProc

	Recibo un mensaje del estilo:

	| Header | PID | CPU |



MENSAJES ENVIADOS AL CPU
==================================================

## Contenido de una pagina

	envio al CPU un mensaje del estilo:

	| TamanioDelContenido | Contenido |

## Error al buscar hueco para un mProc (falta de espacio)

	envio al CPU un mensaje del estilo:

	| Header |

## mProc aceptado (memoria y swap asignados con exito)

	envio al CPU un mensaje del estilo:

	| Header |
