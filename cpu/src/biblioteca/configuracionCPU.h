 /*============================================================================
 Name        : configuracionCPU.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*
 */

#ifndef CONFIGURACIONCPU_H_
#define CONFIGURACIONCPU_H_

#include "sentencias.h"
#include "cpu_2.h"
#include "pruebas.h"
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


#define FAIL -1
#define TAM_RECIBIR 1024
#define LOG_LEVEL_MOSTRAR  901

typedef struct {
	int puerto_fifa;
	int retardo_cpu;
	char* ip_safa;
	int puerto_safa;
	char* ip_dam;
	int port_dma;
	char* ip_fm9;
	int port_fm9;
} t_config_cpu;




t_log* log_cpu;
t_log* mostrar_log_cpu;
t_config* config;
t_config_cpu* config_cpu;

void leer_config_cpu(int argc, char* argv[]);
void configurar(t_config* config);
int config_incorrecta(t_config* config);
void destuirConfiguracion();
void destruirDTBCPU();
void logueoErrores(char* msj, int cod, int tipo, ...);
void logCPU();
void iniciarDatos();
#endif /* CONFIGURACIONCPU_H_ */
