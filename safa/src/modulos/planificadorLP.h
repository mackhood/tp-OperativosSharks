/*
 * adminDTB.h
 *
 *  Created on: 13 sep. 2018
 *      Author: utnso
 */

#ifndef SRC_MODULOS_PLANIFICADOR_PLANIFICADORLP_H_
#define SRC_MODULOS_PLANIFICADOR_PLANIFICADORLP_H_
#include <pthread.h>
#include "safaConfig.h"
#include "cpu.h"
#include <sharksCommons/protocoloSafaCPU.h>
#include <semaphore.h>
#include "adminColasPlanificacion.h"
//#include "../planificador/adminColasPlanificacion.h"
//#include "../safa/src/safa.h"



/************ Funciones ***************/
int getIdGDT();
void initConfigPLP();
void handlePlanificadorLP(t_safa* );
void decrementarGradoMultiProgramacion(t_safa*);
int  estaEnBlockDummy(DTB_SAFA* );
int hayCpuLibres (t_safa*  ) ;
int gradoMultiprogramacionPosible(t_safa*  );
void enviarDummyCpu (DTB_SAFA* );
void setDatoToDummy(DTB_SAFA* dtb,DTB_SAFA* );
int noEstaListoDummy (  DTB_SAFA* );
int hayDTBNuevo();
void ejecutarAlgoritmo(t_safa* ,int );

/********** Definir variables globales **********/
pthread_mutex_t mutexIdGDT;

#endif /* SRC_MODULOS_PLANIFICADOR_PLANIFICADORLP_H_ */
