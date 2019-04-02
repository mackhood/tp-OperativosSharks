/*
 * protocoloSafaDAM.h
 *
 *  Created on: 28 nov. 2018
 *      Author: utnso
 */

#ifndef SHARKSCOMMONS_PROTOCOLOSAFADAM_H_
#define SHARKSCOMMONS_PROTOCOLOSAFADAM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/string.h>
#include <string.h>
#include "mensaje.h"

typedef struct{
	char* cabecera;
	int cod;
	int id;
	int posMemoria;
	int cant_lineas;
	char* lineas;
	char* path;
}t_dmv;

void destroy_t_dmv(t_dmv* dmv);
t_dmv* create_t_dmv(char* cabecera, int codigo, int pid, int posMemoria, int cant_lineas, char* lineas, char* path);
char* serializar_t_dmv(t_dmv* dmv);
t_dmv* deserializar_t_dmv(char* buffer);


#endif /* SHARKSCOMMONS_PROTOCOLOSAFADAM_H_ */
