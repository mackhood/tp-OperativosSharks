#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <unistd.h>
/* Handshakes entre procesos */


/* Configuración S-AFA */
#define CFG_ALGORITMO  "algoritmo"
#define CFG_MULTIPROGRAMACION  "multiprogramacion"
#define CFG_QUANTUM  "quantum"
#define CFG_PORT_SAFA "puerto_safa"


/* Configuración CPU */
//#define CFG_RETARDO_CPU	"retardo"


/* CPU a DMA */


/* Configuración de DMA */

#define CFG_SIZE_DMA  "size"

/* Configuración de FM9 */
#define CFG_ALGORITMO_FM9  "modo_de_ejecución"
#define CFG_SIZE_MEMORIA	"size_memoria"
#define CFG_SIZE_LINEA		"size_linea"
#define CFG_SIZE_PAGINA		"size_pagina"

/* Configuración FIFA */
#define CFG_PATH  		"PUNTO_MONTAJE"
#define CFG_PORT_FIFA		"PUERTO_MDJ"
#define CFG_RETARDO_CPU		"RETARDO_CPU"
#define CFG_RETARDO_FIFA 	"RETARDO_FIFA"

/* Palabras Reservadas */

#define guardar_file	9010
#define obtener_datos	9011
#define validar_file	9012


#define VOLVE 305


/*  LOGGERS   */
t_log* log_consola;
t_log* log_mdj;


/* Algoritmos SAFA */
#define RR 203 //string
#define RRV 204 //string
#define PROPIO 205 //string

/* Planificador a dam */

#define OPERACION_CPU_VALIDA 306
#define OPERACION_CPU_INVALIDA 307

/* Serializacion/Desserializacion */

int strlen_null(char* str);
/*.....ClaveValor........*/
typedef struct {
	char* clave;
	char* valor;
	char* path;
}t_clavevalor;

int sizeof_clavevalor(t_clavevalor cv);
void* serializar_clavevalor(t_clavevalor clave_valor);
t_clavevalor* deserializar_clavevalor(void* buffer);
void loggear(t_log* log_app, int type, char* level_char, char* temp, ...);

//t_config* leerConfig();

/*------Mensajes -----*/
typedef struct {
	int id_dbt;
	int keyword;
	t_clavevalor clave_valor;
}t_mensaje_cpu;

typedef struct{
	char* cpu;
	char* cpu_ahora;  //Me dice en que instancia estaria actualmente
	char* valor;
} t_status_clave;


int sizeof_mensaje_cpu(t_mensaje_cpu mensaje_cpu);
void* serializar_mensaje_cpu(t_mensaje_cpu mensaje_cpu);
t_mensaje_cpu* deserializar_mensaje_cpu(void* buffer);
int strlen_null(char* frase);
void* serializar_status_clave(t_status_clave* status_clave, int* tamanio);
t_status_clave* deserializar_status_clave(void* buffer);

/*---------------CONFIGURACIONES------------------------*/



typedef struct{
	int ID_GDT;
	char* path_escriptorio;
	int programCounter;
	int flag_inicializacion;
	t_dictionary* direccionesAchivos;
	t_clavevalor* clave;
}DT_Bloque;

t_dictionary* datosDBT;

/*-----Extras---*/
#define DESCRIBE_COLOR      "\x1b[0m"
#define FAILURE_COLOR       "\x1b[38;5;1m"
#define SUCCESS_COLOR       "\x1b[38;5;2m"
#define PENDING_COLOR       "\x1b[38;5;6m"
#define NO_COLOR            "\x1b[0m"
#define UNDERLINE           "\x1b[4m"
#define BOLD                "\x1b[1m"

#define FAILURE_BULLET      "✖ "
#define SUCCESS_BULLET      "✔ "
#define PENDING_BULLET      "• "

#endif /* PROTOCOLO_H_ */
