MENSAJES RECIBIDOS DEL SWAP
==================================================

## Contenido de una pagina

	Recibo un mensaje del estilo:

	| TamanioDelContenido | Contenido |

## Error al buscar hueco para un mProc (falta de espacio)

	Recibo un mensaje del estilo:

	| Header |

## mProc aceptado (swap asignado con exito)

	Recibo un mensaje del estilo:

	| Header |



MENSAJES ENVIADOS AL SWAP
==================================================

## Nuevo proceso mProc

	envio al SWAP un mensaje del estilo:

	| Header | PID | CantidadDePaginas |

## Solicitud de lectura de pagina

	envio al SWAP un mensaje del estilo:

	| Header | PID | NumeroDePagina |

## Solicitud de escritura de pagina

	envio al SWAP un mensaje del estilo:

	| Header | PID | NumeroDePagina | longitudDatos | Datos |

## Solicitud de eliminar mProc

	envio al SWAP un mensaje del estilo:

	| Header | PID |
