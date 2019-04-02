#ifndef SRC_MODULOS_CONEXIONES_CONEXIONCPUDAM_H_
#define SRC_MODULOS_CONEXIONES_CONEXIONCPUDAM_H_

#include "safaConfig.h"
#include "adminColasPlanificacion.h"
#include <sharksCommons/serealizacion.h>
#include <sharksCommons/protocoloSafaCPU.h>
#include <sharksCommons/protocoloSafaDAM.h>
#include <sharksCommons/logs.h>
#include <sharksCommons/mensaje.h>

#include <sys/select.h>
#include <pthread.h>
#include <semaphore.h>
#include "cpu.h"
#include "dam.h"


void recibir_mensaje_dma(int* socket);
void handler_conexion_dam_cpu(t_safa* tSafa);
void recibir_mensaje_cpu(int* socket);


#endif /* SRC_MODULOS_CONEXIONES_CONEXIONCPUDAM_H_ */
