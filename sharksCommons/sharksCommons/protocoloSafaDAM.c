#include "protocoloSafaDAM.h"

/**
 *crea la estructura para enviar a safa
 */
t_dmv* create_t_dmv(char* cabecera, int codigo, int pid, int posMemoria, int cant_lineas, char* lineas, char* path){

	t_dmv* mv = calloc(1, sizeof(t_dmv));
	mv->id = pid;
	mv->cod = codigo;
	mv->posMemoria = posMemoria;
	mv->cant_lineas =  cant_lineas;

	mv->cabecera = malloc(sizeof(char)*2);
	mv->lineas = malloc( strlen_null(lineas) );
	mv->path = malloc( strlen_null(path) );
	memcpy(mv->cabecera, cabecera, sizeof(char)*2 );
	memcpy(mv->lineas, lineas, strlen_null(lineas) );
	memcpy(mv->path, path, strlen_null(path) );
	return mv;

}

void destroy_t_dmv(t_dmv* dmv){
	free(dmv->cabecera);
	free(dmv->lineas);
	free(dmv->path);
	free(dmv);
}

/**
 * serializa la estructura para enviar a safa
 */
char* serializar_t_dmv(t_dmv* dmv){

	char* cod = armar_codigo( dmv->cod );
	char* identificador = string_new();
	string_append(&identificador, cod);
	string_append(&identificador, dmv->cabecera);
	char* pid = string_itoa( dmv->id );
	char* posMemoria = string_itoa(dmv->posMemoria);
	char* payPosMemoria= armar_mensaje("", posMemoria);
	char* cantLineas = string_itoa(dmv->cant_lineas);
	char* payCantLineas= armar_mensaje("", cantLineas);

	char* buffer = armar_mensaje(identificador, pid);
	string_append(&buffer, payPosMemoria);
	string_append(&buffer, payCantLineas);
	string_append(&buffer, dmv->lineas);
	string_append(&buffer, dmv->path);

	free(cod);
	free( identificador );
	free( pid );
	free( posMemoria );
	free( payPosMemoria );
	free( cantLineas );
	free( payCantLineas );

	return buffer;

}


t_dmv* deserializar_t_dmv(char* buffer){

	t_dmv* object = malloc(sizeof(t_dmv));

	char* cabecera = string_substring(buffer, 0, 1);
	object->cabecera = malloc(sizeof(char)*2);
	memcpy( object->cabecera, cabecera, sizeof(char)*2);
	free(cabecera);

	char* codigo = string_substring(buffer, 1, 2);
	object->cod = atoi( codigo );
	free(codigo);

	char* pay1 = string_substring(buffer, 3, 10);
	int payload1 = atoi(pay1);
	char* posMemoria = string_substring(buffer, 13, payload1);
	object->posMemoria = atoi( posMemoria );
	free(pay1);
	free(posMemoria);


	char* pay2 = string_substring(buffer, 13 + payload1, 10);
	int payload2 = atoi(pay2);
	char* cantLineas = string_substring(buffer, 13 + payload1 + 10, payload2);
	object->cant_lineas = atoi( cantLineas );
	free( cantLineas );
	free( pay2 );

	char* pay3 = string_substring(buffer, 13 + payload1 + 10 +  payload2, 10);
	int payload3 = atoi(pay3);
	char* lineas = string_substring(buffer, 13 + payload1 + 10 +  payload2 + 10, payload3);
	object->cabecera = malloc( sizeof(char)*strlen_null( lineas ) );
	memcpy( object->lineas, lineas, strlen_null( lineas ));
	free(lineas);
	free(pay3);

	char* pay4 = string_substring(buffer, 13 + payload1 + 10 +  payload2 + 10 + payload3, 10);
	int payload4 = atoi(pay4);
	char* path = string_substring(buffer, 13 + payload1 + 10 +  payload2 + 10 + payload3 +10, payload4);
	object->path = malloc( sizeof(char)*strlen_null( path ) );
	memcpy( object->path, path, strlen_null( path ));
	free(path);
	free(pay4);



	return object;
}

