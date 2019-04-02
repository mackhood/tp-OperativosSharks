 /*============================================================================
 Name        : fm9.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


#ifndef SRC_BIBLIOTECA_FM9_H_
#define SRC_BIBLIOTECA_FM9_H_
#include <commons/string.h>
//Arma el texto de envio para mensajes
char* armarMensaje_Asignar(char * operacion, int pid, int refMemoria,
		int nroLinea, char * valorLinea);
char* armarMensaje_Close(char * operacion, int pid, int refMemoria);
char* armarMensaje_Solicitar(char * operacion, int pid, int refMemoria,
		int nroLinea);


#endif /* SRC_BIBLIOTECA_FM9_H_ */
