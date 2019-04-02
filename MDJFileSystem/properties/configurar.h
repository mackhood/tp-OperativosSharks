 /*============================================================================
 Name        : configurar.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


#ifndef PROPERTIES_CONFIGURAR_H_
#define PROPERTIES_CONFIGURAR_H_
//#include "fileSystem.h"
#include <commons/config.h>
#include <commons/bitarray.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/protocolo_msj.h>
#include "../src/mercadoDeJugadores.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define l_syscom 1024

typedef struct {
	int puerto_fifa;
	char* punto_de_montaje;
	int retardo_fifa;
	char* ip;
} t_config_mdj;

typedef struct {
	int size_bloques;		//	TAMANIO_BLOQUES=64
	int c_bloques;	   //	CANTIDAD_BLOQUES=5192
	char* magic;
} t_metadata;

typedef struct {
	ino_t num_inodo; /* numero de inode */
	mode_t modo_fichero; /* modo del fichero */
	nlink_t hard_link; /* numero de hard links */
	off_t tamanio_bytes; /* tamaño total en bytes */
	blksize_t bloque_tamanio; /* tamaño de bloque preferido */
	blkcnt_t bloque_numero; /* numero de bloques asignados */
	timer_t ultimo_a; /* ultima hora de acceso */
	timer_t ultima_mod;/* ultima hora de modificación */
	timer_t ultimo_inodo;/* ultima hora de cambio en inodo */
} t_estado;


typedef struct {
	char* nombre;
	char** bloques;
	int bloquesAsignados;
	t_estado info;
} t_archivo;

typedef struct {
	char* path;
	int offset;
	int size;
	char* buffer;
	struct stat myStruct;
} t_path;



t_archivo* t_archivos;
t_config_mdj* config_mdj;
t_metadata* metadata;


char* dir;
char*sentencia;
char* r;
int done;
char* lineas;
char* argumentos;
char* syscom;
//bool imRun;
t_log* mdj_log;
t_log* fifa_log;
t_log* internal_log;
t_log* log_consola;
t_log* log_mdj;



void memoria();
void comand();
void empezar();
void destruirConfiguracion();
void crearMdjConfig();
void leer_config(int argc, char* argv[]);
int configurarFIFA(t_config* conf);
void inicializar(int argc, char *argv[]);
void logMDJ();
void logFS();
void crear_MDJ_CREAR(int clave);
#endif /* PROPERTIES_CONFIGURAR_H_ */
