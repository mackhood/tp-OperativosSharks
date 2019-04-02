 /*============================================================================
 Name        : pruebas.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


#ifndef PRUEBAS_H_
#define PRUEBAS_H_
#include "cpu_2.h"
#include <stdlib.h>

void responderSafa(int clave);
void responderDAMCPU(int clave, char* path);
void borrarPruebas();
char* probarLinea(int i);
void iniciarLinea();
void iniciarLista();
void validarMsjFM9(int socket);

void iniciarDTB();
void recibirDTB();
void dummyDAM();
void dummySAFA();

DTB* crearDTBCPU(int cod);
#endif /* PRUEBAS_H_ */
