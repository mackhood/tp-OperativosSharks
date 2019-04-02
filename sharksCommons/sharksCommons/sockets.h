
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <sys/types.h>
#include <sys/socket.h>
#include <commons/string.h>
#include "mensaje.h"
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include <fcntl.h> //fcntl
#include <pthread.h>

#define ERROR -1
#define NULO 0
#define OK 1
#define SOCKET_FAILURE -1
#define FAIL -1

int newSocket();
struct sockaddr_in asociarSocket(int sockfd, int puerto);
void escucharSocket(int sockfd, int conexionesEntrantesPermitidas);
int initSocketNConexiones(int puerto, int cant_max_conexiones);
int conectarCliente(char * ipDestino, int puerto);
int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio) ;
int recibirPorSocket(int fdCliente, void * buffer, int tamanio) ;
void cerrarSocket(int sockfd);
int aceptarConexionSocket(int sockfd);
void selectSocket(int cantSocket, fd_set* listaSocket, int intentos);

