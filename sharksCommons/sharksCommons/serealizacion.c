#include "serealizacion.h"


int enviar(int socket_dest, char* mensaje_a_enviar){
	int ret = 1;
	size_t sbuffer = sizeof(char)*string_length(mensaje_a_enviar);

	char *buffer = string_substring(mensaje_a_enviar,0,sbuffer);

	if ((ret = send(socket_dest, buffer, sbuffer, MSG_NOSIGNAL)) < 0)
		printf("Error al enviar mensaje %s\n", buffer);
	free(buffer);
	return ret;
}

/**
 *  si el mensaje a enviar es nulo usar "" en lugar de NULL
 */
int enviar_mensaje(int socket_dest, char* cabecera, int codigo, char* mensaje_a_enviar){
	char* identificador = malloc(4);
	int retorno_enviar;
	strcpy(identificador, cabecera);
	char* buffer; //= strdup("");
	char* codigo_with_cero = strdup("0");
	char* string_codigo = string_itoa(codigo);
	string_append(&codigo_with_cero,  string_codigo);

	if(string_length( string_codigo ) == 1){

		string_append(&identificador,  codigo_with_cero);

	} else {
		string_append(&codigo_with_cero,  string_codigo);

	}

	if( !strcmp(mensaje_a_enviar, "")==0 ){
		string_append(&buffer, armar_mensaje(identificador, mensaje_a_enviar) );
	} else {
		buffer = strdup(identificador);
		//string_append(&buffer, identificador );
	}
//	free(identificador);
	free(codigo_with_cero);
	free(string_codigo);
	free(identificador);
	retorno_enviar = enviar(socket_dest, buffer);
	free(buffer);
	return retorno_enviar;
}


char* recibir(int socket_receptor, int bytesRecibir){
	int ret;
	char* buffer = malloc(bytesRecibir);


	if ((ret = recv(socket_receptor, buffer, bytesRecibir, 0)) <= 0){
		printf("Error al recibir mensaje del coordinador");
		return strdup("X66"); //en caso de que haya un error al recibir

	}


	char* buffer_aux= strdup(buffer);
	free(buffer);
	return buffer_aux;
}







