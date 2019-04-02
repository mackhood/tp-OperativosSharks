#include "protocoloCpuDAM.h"

DTB_DAM* crearDTBDAM(char* cabecera, int codigo, int idGDT, char* path, int posMemoria, int cantidad_linea) {
	DTB_DAM* pcb = (DTB_DAM*)calloc(1, sizeof(DTB_DAM));
	pcb->cod = codigo;
	pcb->id = idGDT;
	pcb->cabecera = malloc((sizeof(char) * strlen_null(cabecera)) + 1);
	pcb->path = malloc((sizeof(char) * strlen_null(path)) + 1);
	pcb->posMemoria = posMemoria;
	pcb->lineas = cantidad_linea;
	memcpy(pcb->path, path, strlen_null(path));
	memcpy(pcb->cabecera, cabecera, strlen_null(cabecera));
	return pcb;

}
void destruir_DTBDAM(DTB_DAM* pcb) {
	free(pcb->cabecera);
	free(pcb->path);
	free(pcb);
}
char* serializarDTBDAM(DTB_DAM* object){
	char* cod = string_new();
	char* str_cod = armar_codigo( object->cod );
	string_append(&cod, object->cabecera);
	string_append(&cod, str_cod);

	char* payId = string_itoa(object->id);
	char *buffer = armar_mensaje(cod,  payId);
	char* payPath = armar_mensaje("", object->path);
	string_append(&buffer, payPath);
	char* posMemoria = string_itoa( object->posMemoria );
	char* payPosMemoria =  armar_mensaje("", posMemoria);
	string_append(&buffer, payPosMemoria);
	char* cantLineas = string_itoa( object->lineas );
	char* payCantLineas = armar_mensaje("", cantLineas);
	string_append(&buffer, payCantLineas);

	free( cod );
	free( str_cod );
	free( payId );
	free( payPath );
	free( posMemoria );
	free( payPosMemoria );
	free( cantLineas );
	free( payCantLineas );
	return buffer;
}



DTB_DAM* deszerializarDTBDAM(char* buffer){
	DTB_DAM* object = malloc(sizeof(DTB_DAM));
	object->cabecera  = malloc(2);
	char* cab = string_substring(buffer, 0, 1);
	memcpy(object->cabecera, cab, 2);
	free( cab );

	char* codigo = string_substring(buffer, 1, 2);
	object->cod = atoi( codigo );
	free( codigo );

	char* payId = string_substring(buffer, 3, 10);
	int payload1 = atoi( payId );
	char* id = string_substring(buffer, 13, payload1);
	object->id = atoi( id );
	free( payId );
	free( id );

	char* payPath =  string_substring(buffer, 13 + payload1, 10);
	int payload2  = atoi( payPath );
	object->path = malloc(payload2 +1 );
	char* path =  string_substring(buffer, 13 + payload1 + 10, payload2);
	memcpy(object->path,  path, payload2+1);
	free( payPath );
	free( path );

	char* payPosMemoria = string_substring(buffer, 13 + payload1 + 10 + payload2, 10);
	int payload3  = atoi( payPosMemoria );
	char* posMemoria = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 , payload3);
	object->posMemoria = atoi( posMemoria );
	free( payPosMemoria );
	free( posMemoria );

	char* payLineas = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3, 10);
	int payload4  = atoi(payLineas );
	char*  lineas = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3 + 10, payload4);
	object->lineas = atoi(lineas);
	free( payLineas );
	free( lineas );

	return object;
}

