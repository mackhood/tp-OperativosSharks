/*
 * protocoloCpuDAM.h
 *
 *  Created on: 9 sep. 2018
 *      Author: utnso
 */

#ifndef SHARKSCOMMONS_PROTOCOLOCPUDAM_H_
#define SHARKSCOMMONS_PROTOCOLOCPUDAM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/string.h>
#include <string.h>
#include "serealizacion.h"
#include "mensaje.h"
#include "protocoloSafaCPU.h"



#define SOLICITUD_CONEXION_CPU_DAM 4 //envio solicitud de conexion a DMA
#define ACEPTAR_CPU_DMA 5 //acepta la conexion DMA
#define ENVIAR_DTB_DAM 6 //enviar DTB_DAM a DAM.
#define NUEVALINEA  60

typedef struct DTB_DAM{
	char* cabecera;
	int cod;
	int id;
	char* path;
	int posMemoria;
	int lineas;
}DTB_DAM;




void destruir_DTBDAM(DTB_DAM* pcb);
char* serializarDTBDAM(DTB_DAM* object);
DTB_DAM* crearDTBDAM(char* cabecera, int cod, int idGDT, char* path, int posMemoria, int cantidad_linea);
DTB_DAM* deszerializarDTBDAM(char* buffer);

#endif /* SHARKSCOMMONS_PROTOCOLOCPUDAM_H_ */
