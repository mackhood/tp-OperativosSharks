/*
 * protocolo.h
 *
 *  Created on: 29 sep. 2018
 *      Author: utnso
 */

#ifndef SHARKSCOMMONS_PROTOCOLO_H_
#define SHARKSCOMMONS_PROTOCOLO_H_

/**
 * escribamos aca todos los mensajes que nos vamos a intercambiar,
 * la idea es que ahora usemos char explicitos, en lugar de serializacion pura
 * para asi faciliar las pruebas y quede el codigo mucho mas limpio
 *
 */

/** cabeceras**/
#define CABECERA_SAFA "S"
#define CABECERA_CPU "C"
#define CABECERA_FM9 "F"
#define CABECERA_DMA "D"
#define CABECERA_MDJ "M"

/**
 * GENERIC
 */
#define VALID         99
#define GENERIC_ERROR 15
#define GENERIC_VALID VALID

/*---------------OPERACIONES GENERALES---------------*/

#define OPERACION_VALIDA	  101
#define OPERACION_INVALIDA	102

/*---------------ERRORES ESPERADOS ---------------*/
#define ERROR_OPERACION			10000	//string con fallo
#define PATH_INEXISTENTE		10001	// Abrir:  Path Inexistente
#define ESPACIO_INSUFICIENTE 	10002	// Abrir:  Espacio insuficiente en FM9
#define	FILE_NO_OPEN			20001	// Abrir:  El archivo no se encuentra abierto.
#define FALLO_SEGMENTO			20002	// Asignar:Fallo de segmento/memoria.
#define SIN_ESPACIO_FM9			20003	// Asignar:Espacio insuficiente en FM9.
#define ARCH_NO_ABIERTO			30001	// Flush:  El archivo no se encuentra abierto.
#define FALLO_MEMORIA			30002	// Flush:  Fallo de segmento/memoria.
#define SIN_ESPACIO_MDJ			30003	// Flush:  Espacio insuficiente en MDJ.
#define	SIN_ARCHIVO				30004	// Flush:  El archivo no existe en MDJ (fue borrado previamente).
#define ARCH_NO_OPEN			40001	// Close:  El archivo no se encuentra abierto.
#define FAIL_MEMORIA			40002	// Close:  Fallo de segmento/memoria.
#define ARCHIVO_EXISTENTE		50001	// Crear:  Archivo ya existente.
#define SIN_ESPACIO_CREAR		50002	// Crear:  Espacio insuficiente.​
#define	FILE_INEXISTENTE		60001	// Borrar: El archivo no existe.



/*==================protocolo MDJ - DMA ==================*/
//#define HAND_SHAKE_MDJ_DMA 1
//#define OK_ABRIR 6  //datos pedidos
//#define ERROR_ABRIR 7 //10002
/*==================CONFIGURACIONES ==================*/
/* Configuración FIFA */
#define CFG_PATH  		    "PUNTO_MONTAJE"
#define CFG_PORT_FIFA		  "PUERTO_MDJ"
#define CFG_RETARDO_CPU		"RETARDO_CPU"
#define CFG_RETARDO_FIFA 	"RETARDO_FIFA"

/* Configuración CPU */

#define CFG_IP_MDJ   	  "ip_mdj"
#define CFG_IP_FM9   	  "ip_fm9"
#define CFG_PORT_MDJ  	"port_mdj"
#define CFG_PORT_FM9  	"port_fm9"
#define CFG_PORT_DMA  	"port_dma"
#define CFG_IP_DMA  	  "ip_dam"
#define CFG_SIZE_DMA  	"size"
#define CFG_IP_SAFA	    "ip_safa"
#define CFG_INTERVAL  	"retardo"

/* Configuración DMA */

#define DFG_PUERTO_DMA 		"PUERTO_DMA"
#define DFG_PUERTO_SAFA 	"PUERTO_SAFA"
#define DFG_PUERTO_MDJ 		"PUERTO_MDJ"
#define DFG_PUERTO_FM9 		"PUERTO_FM9"
#define DFG_IP_SAFA 	    "IP_SAFA"
#define DFG_IP_MDJ 		    "IP_MDJ"
#define DFG_IP_FM9 		    "IP_FM9"
#define DFG_T_SIZE 		    "T_SIZE"

/* Handshakes entre procesos */

#define HANDSHAKE_MDJ	  "M00"
#define HANDSHAKE_FM9	  "F00"
#define HANDSHAKE_DAM	  "D00"
#define HANDSHAKE_CPU 	"C00"
#define HANDSHAKE_SAFA	"S00"
#define HANDSHAKE 	     0   //todos los modulos usan este handshake ??????

/* Sockets entre procesos */

#define SOCKET_MDJ		7000
#define SOCKET_FM9		7001
#define SOCKET_DAM		7002
#define SOCKET_CPU 		7003
#define SOCKET_SAFA		7004

/*---------------OPERACIONES DEL FILESYSTEM---------------*/

#define VALIDAR_ARCHIVO 900
#define	CREAR_ARCHIVO 901
#define OBTENER_DATOS 902
#define	GUARDAR_DATOS 903
#define	BORRAR_ARCHIVO 904
#define ABRIR_ARCHIVO 905
#define OPERACION_MDJ_VALIDA 907


/* Palabras Reservadas */
#define abrir_file			9000
#define concentrar_file		9001
#define asignar_file		9002
#define wait_file			9003
#define signal_file			9004
#define flush_file			9005
#define close_file			9006
#define crear_file			9007
#define borrar_file			9008
#define guardar_file		9010
#define obtener_datos		9011
#define validar_file		9012

/**operaciones ***/
#define abrir_dam       02
#define asignar_fm9		"C01"
#define wait_safa			"S01"
#define signal_safa		"S02"
#define flush_dam       03
#define close_fm9			"C02"
#define crear_dam			"D04"
#define borrar_dam      05
#define crear_archivo_to_safa "D01"
#define crear_archivo_to_dma 03

/*==================protocolo safa - DMA ==================*/
//#define HAND_SHAKE_SAFA_DMA 2
#define DATOS_MEMORIA_VIRTUAL 20

/*==================protocolo FM9 - DMA ==================*/
//#define HAND_SHAKE_FM9_DMA 3

/*==================protocolo CPU - DMA ==================*/
#define EJECUTAR_LINEA	402
#define FINALIZAR 		  403
#define ABORTAR 		    404
#define VOLVER			    405
#define	DUMMY_OPERACION	"D01"

/*==================protocolo CPU Operaciones ==================*/
#define OPERACION 			  201 // serializacion
#define CPU_FINALIZADO 		202 //
#define EXITO_OPERACION		204
#define BLOQUEAR_DTB		  214
#define DUMMY_EJECUTAR		215

/**
 * OPERACION CPU - DMA
 */
#define OPERACION_ABRIR     30
#define OPERACION_FLUSH     31
#define OPERACION_CREAR     32
#define OPERACION_BORRAR    33
#define OPERACION_DUMMY_DMA	34


/*==================protocolo CPU-SAFA ==================*/
#define ACCESO_INVALIDO_A_FM9 	20
#define ERROR_EN_FM9 21
#define PEDIDO_DESALOJO_DUMMY 22
#define PEDIDO_DESALOJO_ABRIR 23
#define PEDIDO_DESALOJO_FLUSH 24
#define PEDIDO_DESALOJO_CREAR 25
#define PEDIDO_DESALOJO_BORRAR 26
#define PEDIDO_DESALOJO_WAIT 27
#define PEDIDO_WAIT 28
#define PEDIDO_SIGNAL 29
#define OPERACION_DUMMY	30
#define OPERACION_WAIT_OK 31
#define OPERACION_WAIT_ERROR 32
#define OPERACION_SIGNAL_OK 33
#define FIN_ARCHIVO	34
#define FIN_QUANTUM 35

/* Algoritmos SAFA */
#define RR      203 //string
#define RRV     204 //string
#define PROPIO  205 //string

/* Planificador a dam */

#define OPERACION_CPU_VALIDA    306
#define OPERACION_CPU_INVALIDA  307

/**
 * operaciones dma - safa
 */
#define CREAR_ARCHIVO_SAFA    40
#define BORRAR_ARCHIVO_SAFA   41
#define ABRIR_ARCHIVO_SAFA    42
#define DUMMY_SAFA            43

/**
 * operaciones fm9
 */
#define GUARDAR_DATOS_MEMORIA     01
#define PEDIR_DATOS_MEMORIA       02
#define PEDIR_LINEA		       "C03"
#define GUARDAR_DATOS_OK          03  //repuesta a dma
#define GUARDAR_DATOS_ERROR GENERIC_ERROR



#endif /* SHARKSCOMMONS_PROTOCOLO_H_ */
