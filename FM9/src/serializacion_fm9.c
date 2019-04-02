/*
 * serializacion_fm9.c
 *
 *  Created on: 2/12/2018
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

char * armarMensaje_Asignar( char * operacion, int pid, int refMemoria, int nroLinea,  char * valorLinea)
{
	// Ej: C040000000089000000000000000000010000000033abrir /scripts/simple.escriptorio
		char *serializa_Asignar = strdup(operacion);
		char * str_pid=string_itoa(pid);
		int size_payload_pid = string_length(str_pid);
		char *completar_pid = string_repeat('0', 10 - size_payload_pid);
		char * str_refMemoria=string_itoa(refMemoria);
		int size_payload_refMemoria = string_length(str_refMemoria);
		char *completar_refMemoria = string_repeat('0', 10 - size_payload_refMemoria);
		char * str_nroLinea=string_itoa(nroLinea);
		int size_payload_nroLinea = string_length(str_nroLinea);
		char *completar_nroLinea = string_repeat('0', 10 - size_payload_nroLinea);
		char *str_largoLinea = string_itoa(string_length(valorLinea));
		int size_payload_largoLinea = string_length(str_largoLinea);
		char *completar_1argoLinea = string_repeat('0', 10 - size_payload_largoLinea);

		string_append(&serializa_Asignar, completar_pid);
		string_append(&serializa_Asignar, str_pid);
		string_append(&serializa_Asignar, completar_refMemoria);
		string_append(&serializa_Asignar, str_refMemoria);
		string_append(&serializa_Asignar, completar_nroLinea);
		string_append(&serializa_Asignar, str_nroLinea);
		string_append(&serializa_Asignar, completar_1argoLinea);
		string_append(&serializa_Asignar, str_largoLinea);
		string_append(&serializa_Asignar, valorLinea);

		free(completar_pid);
		free(str_pid);
		free(completar_refMemoria);
		free(str_refMemoria);
		free(completar_nroLinea);
		free(str_nroLinea);
		free(completar_1argoLinea);
		free(str_largoLinea);

		return serializa_Asignar;
}

char * armarMensaje_Close( char * operacion, int pid, int refMemoria)
{
	// C0500000001250000000000
			char *serializa_Close = strdup(operacion);
			char * str_pid=string_itoa(pid);
			int size_payload_pid = string_length(str_pid);
			char *completar_pid = string_repeat('0', 10 - size_payload_pid);
			char * str_refMemoria=string_itoa(refMemoria);
			int size_payload_refMemoria = string_length(str_refMemoria);
			char *completar_refMemoria = string_repeat('0', 10 - size_payload_refMemoria);

			string_append(&serializa_Close, completar_pid);
			string_append(&serializa_Close, str_pid);
			string_append(&serializa_Close, completar_refMemoria);
			string_append(&serializa_Close, str_refMemoria);

			free(completar_pid);
			free(str_pid);
			free(completar_refMemoria);
			free(str_refMemoria);

			return serializa_Close;
}

char * armarMensaje_Solicitar( char * operacion, int pid, int refMemoria, int nroLinea){

	char *serializa_Solicitar = strdup(operacion);
	char * str_pid=string_itoa(pid);
	int size_payload_pid = string_length(str_pid);
	char *completar_pid = string_repeat('0', 10 - size_payload_pid);
	char * str_refMemoria=string_itoa(refMemoria);
	int size_payload_refMemoria = string_length(str_refMemoria);
	char *completar_refMemoria = string_repeat('0', 10 - size_payload_refMemoria);
	char * str_nroLinea=string_itoa(nroLinea);
	int size_payload_nroLinea = string_length(str_nroLinea);
	char *completar_nroLinea = string_repeat('0', 10 - size_payload_nroLinea);

	string_append(&serializa_Solicitar, completar_pid);
	string_append(&serializa_Solicitar, str_pid);
	string_append(&serializa_Solicitar, completar_refMemoria);
	string_append(&serializa_Solicitar, str_refMemoria);
	string_append(&serializa_Solicitar, completar_nroLinea);
	string_append(&serializa_Solicitar, str_nroLinea);

	free(completar_pid);
	free(str_pid);
	free(completar_refMemoria);
	free(str_refMemoria);
	free(completar_nroLinea);
	free(str_nroLinea);

	return serializa_Solicitar;

}
