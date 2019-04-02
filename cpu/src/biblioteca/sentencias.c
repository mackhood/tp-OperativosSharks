 /*============================================================================
 Name        : sentencias.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include "sentencias.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/protocoloCpuDAM.h>
#include <sharksCommons/parser.h>
#include <sharksCommons/protocolo_msj.h>
#include "cpu_2.h"

/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/
//t_log* log_cpu;
//t_log* mostrar_log_cpu;


/* =================================================================
				    cpu
 =================================================================*/
void borrarFile(t_cpu_operacion claves) {
	char* path;

	path = claves.argumentos.borrar.path;
	verificarGDT(path);
	log_info(log_cpu, "Borrar path: %s", path);

	responderDAMCPU(OPERACION_BORRAR, path);

	log_trace(mostrar_log_cpu, " DAM: OPERACION_BORRAR %s", SUCCESS_BULLET);

	free(path);

}
void abrirArchivo(t_cpu_operacion claves) {

	char* path;

	path = claves.argumentos.abrir.path;
	if (!verificarGDT(path)) {
		log_info(mostrar_log_cpu, "operación abrir %s", path);

		responderDAMCPU(OPERACION_ABRIR, path);

		responderSafa(PEDIDO_DESALOJO_ABRIR);
		log_trace(mostrar_log_cpu, "Pedido de desalojo a SAFA %s",
				SUCCESS_BULLET);
		free(path);

		//imRunning = false;
	}

	log_trace(mostrar_log_cpu, "Archivo Abierto: %s", path);
	free(path);
}
void asignarArchivo(t_cpu_operacion claves) {
	char* path;


	path = claves.argumentos.asignar.path;
	if (verificarGDT(path)) {

		int linea = atoi(claves.argumentos.asignar.linea);
		int dato = atoi(claves.argumentos.asignar.dato);

		//	log_info(log_cpu, "Asignar recurso: %s Linea: %d en %s", dato, linea, path);

		char* mensaje; // = serializarDTBDAM(aux);
		log_trace(log_cpu, "Serializar DTB");
		mensaje = armarMensaje_Asignar(asignar_fm9, dtb->id, dtb->posMemoria,
				linea, dato);
		int flag = enviar(socket_fm9, mensaje);
		free(mensaje);
		log_trace(mostrar_log_cpu, "Enviar mensaje a FM9 %s", SUCCESS_BULLET);
		free(path);


		verificarEnvio(flag);

	} else {

		responderSafa(FILE_NO_OPEN);
		log_error(mostrar_log_cpu, "%d:El archivo no se encuentra abierto %s",
				FILE_NO_OPEN, FAILURE_BULLET);

	free(path);

		//imRunning = false;
	};

}
void waitArchivo(t_cpu_operacion claves) {
	char* path = claves.argumentos.wait.recurso;
	log_info(log_cpu, "Esperar recurso: %s\n", path);

	dtb->recurso = string_duplicate(path);
	responderSafa(PEDIDO_WAIT);

	free(path);

	char* msj = recibir(socket_safa, TAM_RECIBIR);

	dtb = deszerializarDTB(msj);


	free(msj);

}
void cerrarArchivo(t_cpu_operacion claves) {
	char* path = claves.argumentos.closes.path;
	if (verificarGDT(path)) {
	log_info(log_cpu, "operación cerrar path: %s", path);

	log_info(mostrar_log_cpu, "Cerrar %s", path);

		responderDAMCPU(OPERACION_BORRAR, path);

		char* mensaje = armarMensaje_Close(close_fm9, dtb->id, dtb->posMemoria);
		log_trace(log_cpu, "Serializar FM9");


		enviar(socket_fm9, mensaje);
	log_trace(mostrar_log_cpu, "Enviar mensaje a FM9 %s", SUCCESS_BULLET);
	free(path);
	free(mensaje);
	} else {
		log_error(mostrar_log_cpu, "%d:El archivo no se encuentra abierto %s",
		ARCH_NO_OPEN, FAILURE_BULLET);
		responderDAMCPU(ARCH_NO_OPEN, path);
		//imRunning = false;
	}
}
void crearArchivo(t_cpu_operacion claves) {
	char* path = claves.argumentos.crear.path;
	verificarGDT(path);
	int cant_linea = claves.argumentos.crear.linea;

	DTB_DAM* aux = crearDTBDAM(CABECERA_CPU, OPERACION_CREAR, dtb->id, path,
			0,
			cant_linea);

	char* mensaje = serializarDTBDAM(aux);
	log_trace(log_cpu, "Serializar DTB");

	int flag = enviar(socket_dam, mensaje);
	verificarEnvio(flag);
	log_trace(mostrar_log_cpu, "Enviar mensaje a DAM %s", SUCCESS_BULLET);
	destruir_DTBDAM(aux);
	log_info(log_cpu, "Crear path: %s Bytes: %d", path, cant_linea);

	dtb->cabecera = CABECERA_CPU;
	dtb->codigo = OPERACION_CREAR;
	dtb->path = path;
	dtb->recurso = string_itoa(cant_linea);
	char* msj = serializarDTB(dtb);
	flag = enviar(socket_safa, msj);
	verificarEnvio(flag);

	free(path);
	free(mensaje);
	free(msj);
	//imRunning = false;
}
void signalArchivo(t_cpu_operacion claves) {
	char* dato = claves.argumentos.senial.recurso;
	log_info(mostrar_log_cpu, "operación signal recuso: %s", dato);

	dtb->recurso = string_duplicate(dato);

	responderSafa(PEDIDO_WAIT);

	free(dato);

}
void flushArchivo(t_cpu_operacion claves) {
	char* path = claves.argumentos.flush.path;


	if (!verificarGDT(path)) {

		responderSafa(ARCH_NO_ABIERTO);

		log_error(mostrar_log_cpu, "%d:El archivo no se encuentra abierto %s",
		ARCH_NO_ABIERTO, SUCCESS_BULLET);
		free(path);

		//imRunning = false;
	} else {


	log_info(log_cpu, "Operación Flush path: %s", path);
		responderDAMCPU(OPERACION_FLUSH, path);
	log_info(mostrar_log_cpu, "Flush %s", path);

		responderSafa(OPERACION_FLUSH);
	free(path);
	};
}

bool compararHS(char* mensajeInicial, char* cod) {
	char* hand = string_substring(mensajeInicial, 0, 3);
	bool hs = string_equals_ignore_case(hand, cod);
	free(hand);
	return hs;
}
