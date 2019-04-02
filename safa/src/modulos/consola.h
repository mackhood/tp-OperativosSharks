/*
 * consola.h
 *
 *  Created on: 8 sep. 2018
 *      Author: utnso
 */

#ifndef SRC_MODULOS_GESTORGDT_CONSOLA_H_
#define SRC_MODULOS_GESTORGDT_CONSOLA_H_

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


/** Funciones **/
int ejecutar_linea();
void ejecutarScript(char** argumentos);
void statusColas(char** arg);
void finalizarDTB(char** arg);
void metricaDTB(char** arg);

void consola();

void handleConsolaGDT(t_safa* tSafa);
#endif /* SRC_MODULOS_GESTORGDT_CONSOLA_H_ */
