#ifndef LIBENIAC_SEREALIZACION_H_
#define LIBENIAC_SEREALIZACION_H_

#include <commons/collections/list.h>
#include "sockets.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <commons/error.h>
#include <commons/log.h>
#include <commons/string.h>


#define SEND_OR_RECEIVE_FAILURE -1
#define SEND_OR_RECEIVE_SUCCESS 0




/**
 * funciones en uso
 */

#define TAM_RECV 1024
int enviar_mensaje(int socket_dest, char* cabecera, int codigo, char* mensaje_a_enviar);
int enviar(int socket_dest, char *mensaje_a_enviar);
char* recibir(int socket_receptor, int bytesRecibir);
int strlen_null(char* frase);


#endif /* LIBENIAC_SEREALIZACION_H_ */
