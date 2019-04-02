/*
 * planificador.h
 *
 *  Created on: 10 sep. 2018
 *      Author: utnso
 */

#ifndef SRC_MODULOS_PLANIFICADOR_PLANIFICADORCP_H_
#define SRC_MODULOS_PLANIFICADOR_PLANIFICADORCP_H_

#include <pthread.h>
#include <stdbool.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <sharksCommons/serealizacion.h>
#include "conexionCPUDAM.h"

#include "safaConfig.h"
#include "cpu.h"
#include "adminColasPlanificacion.h"
#include "planificadorLP.h"
pthread_t thread_pcp_cpu_RR;
pthread_t thread_pcp_cpu_VRR;
pthread_t thread_pcp_cpu_SJF;

#define ROUND_ROBIN 1
#define VRROUND_ROBIN 2
#define SJF_RR_1 3

#define NAME_ROUND_ROBIN    "RR"
#define NAME_V_ROUND_ROBIN  "VRR"
#define NAME_SJF_RR_1    "SJF-RR-1"
/************** Estructuras ************/



/************* Funciones **************/
void initConfigPCP(t_safa* tSafa);
void handlePlanificadorCP(t_safa* tSafa);

void ejecutarRoundRobin(CPU* cpu,t_safa* tSafa);
void ejecutarVirtualRoundRobin(CPU* cpu,t_safa* tSafa);
int getAlgoritmoPlanificacion(char* algoritmo);
void handlSJF_RR_1(CPU* );

void handlPCP_CPURR(CPU* cpu ) ;
void handlPCP_CPUVRR(CPU* cpu ) ;
#endif /* SRC_MODULOS_PLANIFICADOR_PLANIFICADORCP_H_ */
