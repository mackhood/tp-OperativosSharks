 /*============================================================================
 Name        : sentencias.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/protocoloCpuDAM.h>
#include <sharksCommons/parser.h>
#include <sharksCommons/protocolo_msj.h>
#include <commons/log.h>
#include "fm9.h"

#ifndef SRC_SENTENCIAS_H_
#define SRC_SENTENCIAS_H_

bool compararHS(char* mensajeInicial, char* cod);
void borrarFile(t_cpu_operacion claves);
void asignarArchivo(t_cpu_operacion claves);
void waitArchivo(t_cpu_operacion claves);
void cerrarArchivo(t_cpu_operacion claves);
void crearArchivo(t_cpu_operacion claves);
void signalArchivo(t_cpu_operacion claves);
void flushArchivo(t_cpu_operacion claves);
void abrirArchivo(t_cpu_operacion claves);
#endif /* SRC_SENTENCIAS_H_ */
