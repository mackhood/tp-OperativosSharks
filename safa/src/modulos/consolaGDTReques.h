/*
 * consolaGDTReques.h
 *
 *  Created on: 13 sep. 2018
 *      Author: utnso
 */

#ifndef SRC_MODULOS_GESTORGDT_CONSOLAGDTREQUES_H_
#define SRC_MODULOS_GESTORGDT_CONSOLAGDTREQUES_H_

#include "planificadorLP.h"
#include "adminColasPlanificacion.h"

void initNuevoGDT(char* pathScriptEscriptorio);
bool validarEstadoPrograma(DTB_SAFA* dtb);
void mostrarStatusTodo();
void mostrarStatus(int pid);
void finalizarById(int pid);
void mostrar_estado_cola(char* estado, int tam, t_list* lista);
void mostrarMetricaTodo();
void mostrarMetrica(int pid);




/* para la funcionalidad metrica */
typedef struct{
	double prom_usaron_diego; //promedio
	int cant_dtb;
	int cant_colas;
	double prom_exit;
	double porc_usaron_diego;
	double tiempo_respuesta;
}t_metrica;




#endif /* SRC_MODULOS_GESTORGDT_CONSOLAGDTREQUES_H_ */
