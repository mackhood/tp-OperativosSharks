# tp-2018-2c-Sharks
Estructura de mensajes (Paquetes estandar de 1024 bytes)
H + YY + LLLLLLLLLL + XXXXXXXXXXXXXXXXXXXXXXXX
H: Identificador del emisor (1 bytes)
Y: Identificador del mensaje (2 bytes)
L: Longitud del mensaje a enviar (10 bytes)
X: Texto del mensaje a enviar(1011 bytes) - en caso de vacio debe ser caracter nulo
TOTAL =	1024 bytes

------- DICCIONARIO DE MENSAJES -------
# SAFA - S (S-AFA)
S00: Mensaje para pedir identificarse al recien conectado

# CPU - C
C00: me presento al mundo como una CPU
C01: Abrir Path: C010014/home/pepe.txt00040012 //C|01|TAM|PATH|TAM2|PID
C02: Flush Path: C020014/home/pepe.txt00040012 //C|02|TAM|PATH|TAM2|PID

# DMA - D (El Diego)
D00: Identificarse como DMA. | "D|00|TRANSSIZE(4bytes)| 
D01: Atiende Op Abrir -> MDJ: D010014/home/pepe.txt00040012 //D|01|TAM|PATH|TAM2|PID
D02: Atiende Op Flush -> FM9: D|02|TAM|PID|TAM|PATH
D03: Envia datos para escribir memoria. "D03|TAM|DATAAAAA"

# MDJ - M (File System)
M00: handshake
M01: Envia Data a DMA: M|01|00XX|DATDATADATA...|

# FM9 - F (Funes Memory 9)
F00: Identifica como Memoria. 
F01: Operacion realizada OK. 
F02: Envia Data a DMA: F|02|00XX|DATDATADATA...|
