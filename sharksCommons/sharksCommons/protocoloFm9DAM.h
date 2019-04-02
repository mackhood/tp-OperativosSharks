/*
 * protocoloFm9DAM.h
 *
 *  Created on: 14 oct. 2018
 *      Author: utnso
 */

#ifndef SHARKSCOMMONS_PROTOCOLOFM9DAM_H_
#define SHARKSCOMMONS_PROTOCOLOFM9DAM_H_

#include <stdlib.h>
#include "mensaje.h"


typedef struct{
	char* cabecera;
	int cod;
	int id;
	char* path;
	int posMemoria;
	int cant_lineas;
	char* linea;
} E_FM9;


E_FM9* deserializar_e_fm9(char* buffer);
E_FM9* crear_e_fm9(char* cabecera, int codigo, int id, char* path,int posMemoria, int cant_lineas, char* linea);
char* serializar_e_fm9(E_FM9* object);
void destroy_e_fm9(E_FM9* object);




#endif /* SHARKSCOMMONS_PROTOCOLOFM9DAM_H_ */
