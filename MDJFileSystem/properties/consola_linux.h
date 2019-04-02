/*
 * consola_linux.h
 *
 *  Created on: 18 oct. 2018
 *      Author: cam
 */

#ifndef CONSOLA_LINUX_H_
#define CONSOLA_LINUX_H_

/***********INCLUDES**************/
//#include "md5.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <stdarg.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/error.h>
#include <unistd.h>
#include <dirent.h>

//#include <openssl/md5.h>
//<openssl/md5.h>

/***********ESTRUCTURAS**************/

typedef int Funcion();

typedef struct {
  char *nombre;
	Function* funcion;
  char *docExtra;
} COMANDOS;


//extern char *xmalloc();
char *nombre_programa;
char *word;
//t_log* log_consola;

COMANDOS* comando;
COMANDOS comandos[9];
/***********FUNCIONES************/
void final_readline();
void eliminarEstructura(COMANDOS *com);
void memoria();
void logConsola();
int com_lista(char* argumento);
int com_ver (char* argumento);
int com_renombrar(char* argumento);
int com_estado(char* argumento);
int com_pwd(char* arg);
int com_md5(void* contenido);
int com_ayuda();
int com_cd(char* argumento);
int com_salir(char* arg);

char *stripwhite ();
COMANDOS* buscar_comando (char *nombre);
int ejecutar_linea (char* line);
int argumentos_validos(char *caller, char* arg);
char *comando_generador();
char **completar_nombre(char* text, int start, int end);
void initialize_readline();
void chauMemoria();
void logConsola();


//t_log* log_consola;
#endif /* CONSOLA_LINUX_H_ */
