/*
 * mensaje.c

 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */
#include "mensaje.h"


char* armar_mensaje(char *identificador, char *mensaje)
{
	char *resultado = strdup(identificador);
	char *payload_char = string_itoa(string_length(mensaje));
	int size_payload = string_length(payload_char);
	char *completar = string_repeat('0', 10 - size_payload);

	string_append(&resultado, completar);
	string_append(&resultado, payload_char);
	string_append(&resultado, mensaje);

	free(payload_char);
	free(completar);
	return resultado;
}

char* armar_codigo(int codigo){
	char* cod_string = string_itoa(codigo);
	int tam_cod = string_length( cod_string );

	free(cod_string);

	char* string_cero = strdup("0");
	if(tam_cod==1){
		string_append(&string_cero, cod_string);
		return string_cero;
	}

	free(string_cero);
	return string_itoa(codigo);

}

//devuelve el header del mensaje
char* get_header(char *mensaje){

	char* header = malloc(2);
	char* h =  string_substring(mensaje, 0, 1);

	memcpy(header,h, 2 );

	free(h);
	return header;
}

int comparar_header(char* identificador, char* mensaje){

	char* header = malloc(2);
	char* h =  string_substring(mensaje, 0, 1);
	memcpy(header,h, 2 );

	free(h);
	int v = !strcmp(header, identificador);
	free(header);
	return v;

}

//devuelve el codigo del mensaje
char* get_codigo(char *mensaje)
{
	return string_substring(mensaje, 1, 2);
}

//obtiene el mensaje
char* get_mensaje(char *mensaje){
	char *payload = string_substring(mensaje, 3, 10);
	int payload1 = atoi(payload);
	free(payload);
	return string_substring(mensaje, 13, payload1);
}

int strlen_null(char* frase) {
	 if (frase == NULL) {
	 		return 0;
	 	}
	 	return strlen(frase) + 1;
 }

