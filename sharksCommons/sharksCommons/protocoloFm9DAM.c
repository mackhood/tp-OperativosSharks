#include "protocoloFm9DAM.h"

E_FM9* crear_e_fm9(char* cabecera, int codigo, int id, char* path,int posMemoria, int cant_lineas, char* linea){
	E_FM9* fm9 = calloc(1, sizeof(E_FM9));
	fm9->id =  id;
	fm9->posMemoria = posMemoria;
	fm9->cod = codigo;
	fm9->cant_lineas = cant_lineas;
	fm9->cabecera = (char*)malloc(2);
	fm9->path =  malloc(sizeof(char)*strlen_null(path));
	fm9->linea =  malloc(sizeof(char)*strlen_null(linea));

	memcpy(fm9->cabecera, cabecera, 2);
	memcpy(fm9->path, path, sizeof(char)*strlen_null(path));
	memcpy(fm9->linea, linea, sizeof(char)*strlen_null(linea));
	return fm9;
}

/**
 *
 */

char* serializar_e_fm9(E_FM9* object){

	char* identificador = strdup("");
	string_append(&identificador, object->cabecera);
	char* cod = armar_codigo( object->cod );
	string_append(&identificador, cod);

	char* posMemoria = string_itoa(object->posMemoria);
	char* payPosMemoria = armar_mensaje("", posMemoria);
	char* pid = string_itoa( object->id );
	char* payId = armar_mensaje("", pid);
	char* payPath = armar_mensaje("", object->path);
	char* cantLineas = string_itoa( object->cant_lineas );
	char* payCantLineas = armar_mensaje("", cantLineas);
	char* payLinea = armar_mensaje("", object->linea);

	char *buffer = armar_mensaje(identificador, payId);
	string_append(&buffer, payPath);
	string_append(&buffer, payPosMemoria);
	string_append(&buffer, payCantLineas);
	string_append(&buffer, payLinea);


	free(posMemoria);
	free(payPosMemoria);
	free(pid);
	free(payId);
	free(payPath);
	free(cantLineas);
	free(payCantLineas);
	free(payLinea);
	free(identificador);
	free(cod);
	return buffer;
}

E_FM9* deserializar_e_fm9(char* buffer){
	E_FM9* object = (E_FM9*)calloc(1, sizeof(E_FM9));

	object->cabecera = malloc(sizeof(char)*2);
	char* cabecera = string_substring(buffer, 0, 1);
	memcpy(object->cabecera, cabecera, sizeof(char)*2 );
	free(cabecera);

	char* cod = string_substring(buffer, 1, 2);
	object->cod = atoi(cod);
	free(cod);

	char* pay1 = string_substring(buffer, 3, 10);
	int payload1 = atoi(pay1);
	char* pid = string_substring(buffer, 3 + 10, payload1);
	object->cod = atoi(pid);
	free(pid);
	free(pay1);

	char* pay2 = string_substring(buffer, 3 + 10 + payload1, 10);
	int payload2 = atoi(pay2);
	char* path = string_substring(buffer, 3 + 10 + payload1 + 10, payload2);
	object->path = malloc(sizeof(char)*strlen_null( path ));
	memcpy( object->path, path,  sizeof(char)*strlen_null( path ));
	free(path);
	free(pay2);

	char* pay3 = string_substring(buffer, 3 + 10 + payload1 + 10 + payload2, 10);
	int payload3 = atoi(pay3);
	char* posMemoria = string_substring(buffer, 3 + 10 + payload1 + 10 + payload2 + 10, payload3);
	object->posMemoria = atoi( posMemoria );
	free(posMemoria);
	free(pay3);

	char* pay4 = string_substring(buffer, 3 + 10 + payload1 + 10 + payload2 + 10 + payload3, 10);
	int payload4 = atoi(pay4);
	char* cant_lineas = string_substring(buffer, 3 + 10 + payload1 + 10 + payload2 + 10 + payload3 + 10, payload4);
	object->cant_lineas = atoi( cant_lineas );
	free(cant_lineas);
	free(pay4);

	char* pay5 = string_substring(buffer, 3 + 10 + payload1 + 10 + payload2 + 10 +
			payload3 + 10 + payload4, 10);
	int payload5 = atoi(pay5);
	char* linea =  string_substring(buffer, 3 + 10 + payload1 + 10 + payload2 + 10 +
			payload3 + 10 + payload4 + 10, payload5);
	object->linea = malloc(sizeof(char)*strlen_null( linea ));
	memcpy( object->linea, linea,  sizeof(char)*strlen_null( linea ) );
	free(pay5);
	free(linea);


	return object;

}


void destroy_e_fm9(E_FM9* object){
	free( object->path );
	free( object->linea );
	free( object->cabecera );
	free( object );
}

