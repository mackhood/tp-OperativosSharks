/*
 * cpuConfig.h
 *
 *  Created on: 18 nov. 2018
 *      Author: utnso
 */

#ifndef CPUCONFIG_H_
#define CPUCONFIG_H_

#include <commons/config.h>

#include <sharksCommons/protocoloSafaCPU.h>
#include <sharksCommons/protocoloCpuDAM.h>

typedef struct{
		int retardo_cpu;
		char* ip_safa;
		int port_safa;
		char* ip_dam;
		int port_dma;
		char* ip_fm9;
		int port_fm9;
}t_cpu_config;



typedef struct {


	int		fdDma;
	t_cpu_config* config;
	int 	fdSafa;
	int 	fdFm9;

}t_cpu;


t_config * config;
t_cpu_config* cpuConfig;
t_cpu* tCpu;

DTB* dtb;



#endif /* CPUCONFIG_H_ */
