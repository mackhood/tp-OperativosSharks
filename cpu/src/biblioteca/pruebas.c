 /*============================================================================
 Name        : pruebas.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


 /*=================================================================
				Type: INCLUDE
 =================================================================*/
#include "pruebas.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu_2.h"
/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/



/* =================================================================
				    cpu
 =================================================================*/
char* solicitarLinea(int posMemoria, int linea);
void recibirDTB() {
	char* dtb_safa;

	dtb_safa = recibir(socket_safa, TAM_RECIBIR);
	printf("%s\n", dtb_safa);
	dtb = deszerializarDTB(dtb_safa);

	log_trace(mostrar_log_cpu, "Se recibió DTb");

	quantum = dtb->quantum;
	/*
	 * S|8|2|0|0|/home/fifa-examples/fifa-entrega/Archivos/scripts/simple.escriptorio|4|1|SARAZA|2|/home/fifa-examples/fifa-entrega/Archivos/scripts/simple.escriptorio|/home/fifa-examples/fifa-entrega/Archivos/scripts/complejo.escriptorio|
	 * */


	log_info(mostrar_log_cpu, "Quantum: %d", quantum);
	log_info(mostrar_log_cpu, "dtb flag %d", dtb->flag);
	log_debug(mostrar_log_cpu, "DTb path: %s", dtb->path);
	log_debug(mostrar_log_cpu, "DTb ID: %d", dtb->id);
	log_debug(mostrar_log_cpu, "DTb: %d", sizeof dtb);
	log_debug(mostrar_log_cpu, "DTb Código: %d", dtb->codigo);

	imRunning = true;

	log_info(log_cpu, "dtb configurado");
	free(dtb_safa);
	dtb_safa = NULL;

}
void dummyDAM() {
	log_trace(log_cpu, "Ejecutar DTB %d", dtb->id);

	responderDAMCPU(OPERACION_ABRIR, "\0");
	log_trace(log_cpu, "Enviar Dummy a DAM");
	log_trace(mostrar_log_cpu, "Operación Abrir para el Diego %s",
	SUCCESS_BULLET);
}
void responderDAMCPU(int clave, char* path) {
	char* msj;
	DTB_DAM* respuesta;
	log_trace(log_cpu, "Crear Info para DAM");
	respuesta = crearDTBDAM(CABECERA_CPU, clave, dtb->id, path,
			dtb->posMemoria, 0);

	log_warning(mostrar_log_cpu, "mensaje: %s", respuesta);
	msj = serializarDTBDAM(respuesta);

	int enviarM = enviar(socket_dam, msj);
	verificarEnvio(enviarM);
	free(msj);
	msj = NULL;
	destruir_DTBDAM(respuesta);

}

void dummySAFA() {
	responderSafa(PEDIDO_DESALOJO_DUMMY);
	log_trace(mostrar_log_cpu, "Enviar mensaje a Safa: BLOQUEAR_DTB_DUMMY");
}
void responderSafa(int clave) {
	char* mensaj;
	DTB* aux;
	aux = crearDTB(CABECERA_CPU, clave, dtb->id, dtb->flag, dtb->pc, dtb->path,
			dtb->quantum, dtb->posMemoria, dtb->recurso,
			dtb->archivos_abiertos);

	log_trace(log_cpu, "Ejecutar DTB %d", dtb->id);


		mensaj = serializarDTB(aux);
	log_warning(mostrar_log_cpu, "mensaje: %s", mensaj);
	log_trace(log_cpu, "Serializar Info a enviar");

	int flag = enviar(socket_safa, mensaj);

	verificarEnvio(flag);

	log_trace(log_cpu, "Bloquear DTB  %s", SUCCESS_BULLET);

	free(mensaj);
	destruirDTBCPU(aux);

}
void iniciarLista() {
	prueba = list_create();
}
void validarMsjFM9(int socket) {


	/*if ( TODO validacion    == ERROR_OPERACION) {
	 log_error(mostrar_log_cpu, "%d: archivo no abierto", ARCH_NO_ABIERTO);
	 log_error(log_cpu, "%d: archivo no abierto", ARCH_NO_ABIERTO);

	 log_trace(mostrar_log_cpu, "se envia mensaje al 10");
	 aux = crearDTBDAM(CABECERA_CPU, OPERACION_ABRIR, dtb->id, path, NULL,
	 NULL);
	 mensaje = serializarDTBDAM(aux);
	 log_trace(log_cpu, "Serializar DTB");

	 flag = enviar(socket_dam, mensaje);
	 verificarEnvio(flag);
	 destruir_DTBDAM(aux);
	 } else{

	 linea = mensaje_fm9;

	 if (scanf(linea) == -1) {
	 log_error(log_cpu, "Script vacio.");
	 //morir();
	 } else {
	 ultima_linea = realloc(ultima_linea, len);
	 strcpy(&ultima_linea, &linea);
	 };*/

}
void iniciarDTB() {
	dtb = crearDTBCPU(OPERACION);
}
void iniciarLinea() {

}
char* probarLinea(int i) {
	return (linea[i]);
}

void borrarPruebas() {

	list_destroy_and_destroy_elements(prueba, free);


}

DTB* crearDTBCPU(int flag) {

	DTB* pcb = calloc(1, sizeof(DTB));

	pcb->cabecera = string_duplicate(CABECERA_CPU);
	//malloc(sizeof(CABECERA_CPU));
	//memcpy(pcb->cabecera, CABECERA_CPU, strlen_null(CABECERA_CPU));

	pcb->codigo = flag;
	pcb->id = 0;
	pcb->flag = 0;
	pcb->pc = -1;
	pcb->path = strdup("");


	pcb->quantum = 2;
	pcb->posMemoria = 0;

	pcb->recurso = strdup("");

	//pcb->archivos_abiertos = list_create();
	pcb->archivos_abiertos = list_duplicate(prueba);

	return pcb;

}
char* solicitarLinea(int posMemoria, int linea) {
	/*	int cantSegMax= SEGMENTOS;
	 int baseLinea,tamLinea,i;
	 for(i=0;i<cantSegMax;i++){
	 if (tablaLineas[i].seg == posMemoria && tablaLineas[i].linea == linea) {
	 baseLinea = tablaLineas[i].base;
	 tamLinea = tablaLineas[i].tamanio;
	 i=cantSegMax;
	 }
	 }
	 char * frame = malloc(tamLinea);
	 memset(frame,'\0',tamLinea);
	 memcpy(frame,Memoria+tablaSegmentos[posMemoria].base+baseLinea,tamLinea);

	 S|02||1|1|/home/fifa-examples/fifa-entrega/Archivos/scripts/simple.escriptorio|1|3|"Algo"|2|/home/fifa-examples/fifa-entrega/Archivos/scripts/simple.escriptorio|
	 C|22|0|0|2|0|0|2|/home/fifa-examples/fifa-entrega/Archivos/scripts/simple.escriptorio|0|
	 */
	return (" ");
}

