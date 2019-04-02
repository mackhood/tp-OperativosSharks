/*
 * serealizacion_fm9.h
 *
 *  Created on: 2/12/2018
 *      Author: utnso
 */

#ifndef SERIALIZACION_FM9_H_
#define SERIALIZACION_FM9_H_

//Arma el texto de envio para mensajes
char * armarMensaje_Asignar( char * operacion, int pid, int refMemoria, int nroLinea,  char * valorLinea);
char * armarMensaje_Close( char * operacion, int pid, int refMemoria);
char * armarMensaje_Solicitar( char * operacion, int pid, int refMemoria, int nroLinea);


#endif /* SERIALIZACION_FM9_H_ */
