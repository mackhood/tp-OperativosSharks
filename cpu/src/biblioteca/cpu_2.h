/*
 * SAFA.h
 *
 *  Created on: 2 sept. 2018
 *      Author: cam
 */

#ifndef CPU_H_
#define CPU_H_

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include <sharksCommons/protocolo_msj.h>
#include <sharksCommons/sockets.h>
#include <sharksCommons/protocoloCpuDAM.h>
#include <sharksCommons/protocoloSafaCPU.h>
#include <sharksCommons/parser.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/serealizacion.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include "conect.h"
#include "configuracionCPU.h"
#include "pruebas.h"
#include "sentencias.h"

//#include <sisop/conexiones.h>
//#include <sisop/sockets.h>


/*
#define CFG_ALGORITMO  "algoritmo"
#define CFG_PORT_SAFA  8080		//"puerto_safa"
#define CFG_MULTIPROGRAMACION  "multiprogramacion"
#define CFG_QUANTUM  "quantum"
#define CFG_INTERVAL  "retardo"
 */


// estructuras
char* linea[9];

bool imRunning;
//int socket_dam;
int socket_safa;
int socket_dam;
int socket_fm9;
int ID;
int pc;

int quantum;
char* ultima_linea;

t_list* prueba;
DTB* dtb;




void inicializar();
void levantarConfig(t_config* config);
void morir();


//conexion
int inicializarConexion_cpu();
void atenderConexion(int servidor);

void notificar_cpu(int socket_safa, char* respuesta);

void morir();
void finalizar_por_senial(int sig);
void verificar_senial (int num);
void esperarSenial(int seg);


void verificarEnvio(int num);
bool escriptorioVacio(char* line);
bool codigoBueno();
int verificarFlag(DTB* dbt);

bool verificarGDT(char* arch);

//Funciones

int extraer_mensaje_cpu(char* linea);


void ejecutarSentencias(int tiempo);
void ejecutar();
int ejecutarMensaje(t_cpu_operacion operacion, char* linea);

void dummy();

#endif /* CPU_H_ */
