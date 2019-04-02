/*
 * FM9.h
 *
 *  Created on: 12/9/2018
 *      Author: utnso
 */

#ifndef FM9_H_
#define FM9_H_

//Estructura definida para el modulo de memoria

typedef struct{
	char * IP_FM9;
	int PUERTO_FM9;
	char *MODO;
	int TAMANIO;
	int MAX_LINEA;
	int TAM_PAGINA;
}t_fm9;

typedef struct{
	int pid;
	int pag;
	int estado;
} t_tablaPaginas;

typedef struct{
	int pid;
	int seg;
	int base;
	int limite;
	int estado;
} t_tablaSegmentos;



typedef struct{
	int pid;
	char* Archivos;
	int posMemoria;
} t_tablaProcesos;

typedef struct{
	int seg;
	int linea;
	int base;
	int tamanio;
} t_tablaLineas;



#include "stddef.h"
#include "stdlib.h"
#include <readline/readline.h>
#include <commons/string.h>
#include <sharksCommons/logs.h>
#include <readline/history.h>
#include <stdio.h>
#include "consolaGDTReques.h"
#include "safaConfig.h"

typedef int Funcion ();

typedef struct {
	char* nombre;
	Funcion* funcion;
} COMANDO;


COMANDO comandos[] = {
		{"Dump",dump},
		{(char *) NULL, (Function *) NULL}
};



#define SEGMENTOS 10

#endif /* FM9_H_ */
