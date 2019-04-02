 /*============================================================================
 Name        : mercadoDeJugadores.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+ */

#ifndef SRC_MERCADODEJUGADORES_H_
#define SRC_MERCADODEJUGADORES_H_

/* =================================================================
				Type: INCLUDE
 =================================================================*/

#include "../properties/configurar.h"
#include "../properties/consola_linux.h"
//#include <mdj/consola_linux.h>
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/log.h>
#include "../properties/fileSystem.h"
#include "../properties/conexiones.h"
#include "../properties/configurar.h"
#include <sharksCommons/protocolo.h>
#include <sharksCommons/protocoloMdjDAM.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#define BACKLOG 		1

/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/

t_list* tabla;
char* pathMetadata;
t_log* mdj_log;
t_log* fifa_log;
t_log* internal_log;
t_log* log_consola;
t_log* log_mdj;
//t_log* arbol_log;
int socket_mdj;
//int socket_dam;
int servidor_mdj;
int flag;
t_config* config_mdj_aux;
E_MDJ* jugador;
bool imRun;
char* lineasComandos;
pthread_t* conect;
pthread_t* consola_thread;
pthread_attr_t* at_conect;
pthread_attr_t* at_consola_thread;
/* =================================================================
				    Type: PROTOTIPOS
 =================================================================*/
void liberarMemoria();
void chauConsola();
void dest_logFS();
int conexionesMDJ();
void error_sockets(int* controlador, char* proceso);
void esperarMsj();
//t_config_mdj* fifaGate();
/* =================================================================
				    Type: FUNCIONES
 =================================================================*/
void consolaMDJ(char *argv[]);

void ejecucion();
void validar(char* path);
void* crearArchivo(char* path, int linea);
void cerrar();
void borrarArchivo(char* path);
void datos();
void guardarDatos();
void morir();
void eliminarPath(char* pathArchivo);
//* obtener_configuracion_archivo(char* path);
void finArbol(char* path);
void servidor_fifa(int argc, char **argv);
void conectarFifa();
//void empezar();
void verificar_senial(int y);
void finalizar_por_senial(int y);
void esperarDAM();
#endif /* SRC_MERCADODEJUGADORES_H_ */
