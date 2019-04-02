/*
 * cpu.h
 *
 *  Created on: 25 sep. 2018
 *      Author: utnso
 */

#ifndef SRC_MODULOS_CONEXIONES_CPU_H_
#define SRC_MODULOS_CONEXIONES_CPU_H_


#include "safaConfig.h"
#include "adminColasPlanificacion.h"
#include <sharksCommons/serealizacion.h>
#include <sharksCommons/protocoloSafaCPU.h>
#include <sharksCommons/logs.h>
#include <sharksCommons/sockets.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>
//#include "../../safa.h"


//estructura para representar una CPU conectada a safa
typedef struct CPU {
	bool libreEjecucion;
	int fd;	// file Descriptor del CPU
//	DTB* dtb; // dtb del programa asociado que se esta ejecutando
	t_safa* t_safa;
}CPU;


void initCPUConfiguracion();
void handleNuevoCPU(int socketClient,t_safa* tSafa);
CPU* crearCPU(int socketClient,t_safa* safa);
void agregarCPU(CPU* cpu);
CPU* buscarCPU();
DTB* map_dtb_safa_cpu(DTB_SAFA*,int );
DTB_SAFA* map_dtb_cpu_safa(DTB* );
void operacion_signal(int socket, DTB* dtb);
void eliminarCPU(CPU* ) ;
void removerCPU(CPU* );
void setEstadoLibreCPU(CPU* cpu);
CPU* findCPUByFd(int fd) ;
void abortarDTB(int socket, DTB* c_dtb);
DTB_SAFA* map_dtb_cpu_safa(DTB* c_dtb);
void desalojar(int socket, DTB* c_dtb);
void bloquear_dummy(int socket, DTB* c_dtb);
void operacion_wait(int socket, DTB* dtb);

#endif /* SRC_MODULOS_CONEXIONES_CPU_H_ */
