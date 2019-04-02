/*
 * mensaje.h
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/string.h>
#include "protocolo.h"

//Arma el texto de envio para mensajes
char *armar_mensaje(char *identificador, char *mensaje);

//Devuelve el header del mensaje
char *get_header(char *mensaje);

//Devuelve el codigo del mensaje
char *get_codigo(char *mensaje);

//Obtiene el mensaje
char * get_mensaje(char *mensaje);

//Compara un header contra el header un mensaje
int comparar_header(char *identificador, char *mensaje);

//crea el codigo y devuelve un string del mismo
char* armar_codigo(int codigo);

int strlen_null(char* frase);

#endif /* MENSAJE_H_ */
