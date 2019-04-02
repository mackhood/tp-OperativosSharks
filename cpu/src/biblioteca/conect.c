 /*============================================================================
 Name        : conect.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


 /*=================================================================
				Type: INCLUDE
 =================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conect.h"
#include "cpu_2.h"
#include "fm9.h"
/*=================================================================
				Type: ESTRUCTURAS
 =================================================================*/



/*=================================================================
				    cpu
 =================================================================*/
t_paqueteCPU* recibirCPU(int socket) {

	t_paqueteCPU* paquete = malloc(sizeof(t_paqueteCPU));

	int res = recv(socket, &paquete->cod, sizeof(int),
	MSG_WAITALL);

	if (res < 0) {
		paquete->cod = -1; //Si el cod. de operacion es -1 indica que hubo un error al recibir el paquete
		paquete->tamanio = -1;
		paquete->data = NULL;
		return paquete;

	}

	recv(socket, &paquete->tamanio, sizeof(int), MSG_WAITALL);

	if (paquete->tamanio == 0) { // Si el tamanio es cero, no hay payload que recibir. Por ejemplo al recibir un handshake
		paquete->data = NULL;
	} else {
		void * info = malloc(paquete->tamanio);

		recv(socket, info, paquete->tamanio, MSG_WAITALL);

		paquete->data = info;
	}

	return paquete;
}
//Solicitar Linea C07|PID|POSMEMORIA|NROLINEA
t_paqueteCPU* crearCPU(char* cabecera, int codigo, int idGDT, int posMemoria,
		int cantidad_linea) {
	t_paqueteCPU* pcb = (t_paqueteCPU*) calloc(1, sizeof(t_paqueteCPU));
	char* path = strdup("");
	pcb->cod = atoi(armar_codigo(codigo));
	//codigo;
	pcb->id = dtb->id;
	pcb->cabecera = malloc((sizeof(char) * strlen_null(CABECERA_CPU)));
	//pcb->path = malloc((sizeof(char) * strlen_null(path)) + 1);
	pcb->posMemoria = posMemoria;
	pcb->lineas = cantidad_linea;
	memcpy(pcb->path, path, strlen_null(path));
	memcpy(pcb->cabecera, CABECERA_CPU, strlen_null(cabecera));
	return pcb;
}
char* serializarCPU(t_paqueteCPU* object) {
	char* cod = string_new();
	string_append(&cod, object->cabecera);
	string_append(&cod, armar_codigo(object->cod));

	char *buffer = armar_mensaje(cod, string_itoa(object->id));
	//string_append(&buffer, armar_mensaje("", object->path));
	string_append(&buffer, armar_mensaje("", string_itoa(object->posMemoria)));
	string_append(&buffer, armar_mensaje("", string_itoa(object->lineas)));

	free(cod);
	return buffer;
}
//C04|PID|POSMEMORIA|NROLINEA|TAMLINEA|VALORLINEA
t_paqueteCPU* deszerializarCPU(char* buffer) {
	t_paqueteCPU* object = malloc(sizeof(t_paqueteCPU));
	object->cabecera = string_new();
	string_append(&object->cabecera,
			string_substring(buffer, 0, strlen_null(CABECERA_FM9)));
	object->cod = atoi(string_substring(buffer, 1, 2));
	int bytes = 10;
	int pid = atoi(string_substring(buffer, 3, bytes));
	object->id = atoi(string_substring(buffer, (3 + bytes), pid));
	int pos = atoi(string_substring(buffer, 13 + pid, bytes));

	object->path = string_new();

	int lin = atoi(string_substring(buffer, 13 + pid + bytes + pos, bytes));
	object->posMemoria = atoi(
			string_substring(buffer, 13 + pid + bytes + pos + bytes, lin));
	int c_lin = atoi(
			string_substring(buffer, 13 + pid + bytes + pos + bytes + lin,
					bytes));
	object->lineas = atoi(
			string_substring(buffer,
					13 + pid + bytes + pos + bytes + lin + bytes, c_lin));
	string_append(&object->path,
			string_substring(buffer,
					13 + pid + bytes + pos + bytes + lin + bytes + c_lin,
					string_length(buffer)));
	return object;
}

char* solicitarSentencia(int unaLinea) {

	char* pedido;
	pedido = string_duplicate(
			armarMensaje_Solicitar(PEDIR_LINEA, dtb->id, dtb->posMemoria,
					unaLinea));

	enviar(socket_fm9, pedido);
	free(pedido);
	pedido = NULL;
	char* line = recibir(socket_fm9, TAM_RECIBIR);
	return line;
}

int conectar_fm9() {
	int puerto;

	puerto = config_cpu->port_fm9;
	log_info(log_cpu, "Puerto:%d%s", puerto, SUCCESS_BULLET);

	//char* ip = string_new();
	//string_append(&ip, config_cpu->ip_fm9);

	log_info(log_cpu, "IP:%s%s", config_cpu->ip_fm9, SUCCESS_BULLET);
	log_info(log_cpu, "Puerto:%d%s", puerto, SUCCESS_BULLET);

	socket_fm9 = conectarCliente(config_cpu->ip_fm9, puerto);

	log_debug(log_cpu, "Conectar a FunesMemory %s", SUCCESS_BULLET);

	if (socket_fm9 == FAIL) {
		log_error(log_cpu, "Conexión con FunesMemory %s", FAILURE_BULLET);
		close(socket_fm9);
		morir();
	}

	enviar_handshake_cpu(socket_fm9, SOCKET_FM9);
	//free(ip);
	return (0);
}
void conectar_DMA() {

	int puerto;

	puerto = config_cpu->port_dma;
	log_info(log_cpu, "Puerto:%d%s", puerto, SUCCESS_BULLET);


	log_info(log_cpu, "IP:%s", config_cpu->ip_dam);

	socket_dam = conectarCliente(config_cpu->ip_dam, puerto);

	if (socket_dam == FAIL) {
		log_error(log_cpu, "Conexión con elDiego %s", FAILURE_BULLET);
		close(socket_dam);
		morir();
	}
	log_debug(log_cpu, "Conectar al Diego %s", SUCCESS_BULLET);

	enviar_handshake_cpu(socket_dam, SOCKET_DAM);
	//free(ip);
}
void conectar_Safa() {

	int puerto;

	puerto = config_cpu->puerto_safa;
	log_info(log_cpu, "Puerto:%d%s", puerto, SUCCESS_BULLET);

	//char* ip = string_new();
	//string_append(&ip, config_cpu->ip_safa);
	log_info(log_cpu, "IP:%s%s", config_cpu->ip_safa, SUCCESS_BULLET);

	socket_safa = conectarCliente(config_cpu->ip_safa, puerto);

	if (socket_safa == FAIL) {
		log_error(log_cpu, "Conexión con SAFA %s", FAILURE_BULLET);
		close(socket_safa);
		//free(ip);
		morir();
	}

	//free(ip);
	enviar_handshake_cpu(socket_safa, SOCKET_SAFA);
}
void enviar_handshake_cpu(int socket_cpu, int cod) {
	char* mensajeInicial = NULL;

	switch (cod) {
	case SOCKET_SAFA:
		enviar(socket_cpu, HANDSHAKE_CPU);
		log_trace(log_cpu, "Enviar HANDSHAKE_CPU a %s", SUCCESS_BULLET);

		mensajeInicial = recibir(socket_cpu, TAM_RECIBIR);

		if (compararHS(mensajeInicial, HANDSHAKE_SAFA)) {
			log_trace(log_cpu, "HANDSHAKE_SAFA %s", SUCCESS_BULLET);
			free(mensajeInicial);
		} else {
			log_warning(log_cpu, "HANDSHAKE_SAFA %s", FAILURE_BULLET);
			log_warning(log_cpu, "No se pudo realizar handshake con safa..");
			perror("error al conectarse con safa");

			free(mensajeInicial);
		}
		break;
	case SOCKET_DAM:

		enviar(socket_cpu, HANDSHAKE_CPU);
		log_trace(log_cpu, "Enviar HANDSHAKE_CPU %s%s\n", HANDSHAKE_CPU,
		SUCCESS_BULLET);

		mensajeInicial = recibir(socket_cpu, TAM_RECIBIR);
		log_trace(mostrar_log_cpu, "Esperar HANDSHAKE de %s%s\n", HANDSHAKE_DAM,
		SUCCESS_BULLET);

		if (compararHS(mensajeInicial, HANDSHAKE_DAM)) {
			log_trace(log_cpu, "HANDSHAKE_DAM %s", SUCCESS_BULLET);
			log_debug(mostrar_log_cpu, "HANDSHAKE_DAM %s", mensajeInicial);
			free(mensajeInicial);
		} else {
			log_warning(log_cpu, "HANDSHAKE_DAM %s", FAILURE_BULLET);
			perror("error al conectarse con el diego\n");
			free(mensajeInicial);
		}
		break;
	case SOCKET_FM9:
		enviar(socket_cpu, HANDSHAKE_CPU);
		log_trace(log_cpu, "Enviar HANDSHAKE_CPU %s%s\n", HANDSHAKE_CPU,
		SUCCESS_BULLET);
		mensajeInicial = recibir(socket_cpu, TAM_RECIBIR);

		log_trace(mostrar_log_cpu, "Esperar HANDSHAKE de %s%s\n", HANDSHAKE_FM9,
		SUCCESS_BULLET);

		if (compararHS(mensajeInicial, HANDSHAKE_FM9)) {
			log_trace(log_cpu, "HANDSHAKE_FM9 %s", SUCCESS_BULLET);
			log_debug(mostrar_log_cpu, "HANDSHAKE_FM9 %s", mensajeInicial);
		} else {
			log_warning(log_cpu, "HANDSHAKE_FM9 %s", FAILURE_BULLET);
			log_warning(mostrar_log_cpu, "No se pudo realizar HANDSHAKE_FM9..");

			free(mensajeInicial);
		}

		break;
	default:
		log_warning(log_cpu, "No hay HANDSHAKE para enviar");

		free(mensajeInicial);
	}

}
