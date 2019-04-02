/*
 * protocoloSafaCPU.c
 *
 *  Created on: 19 sep. 2018
 *      Author: utnso
 */

#include "protocoloSafaCPU.h"

/**
 * ​El PLP deberá crear un DTB y asignar un ID al mismo.
 * La creación de este DTB siempre se hará con el flag
 * de inicialización en 1
 */
DTB* crearDTB(char* cabecera, int codigo, int id, int flag,
			int pc, char* path, int quantum, int posMemoria,
			char* recurso, t_list* archivos_abiertos) {


	char* cab = string_new();
	string_append(&cab, cabecera);
	string_append(&cab, "\0");

	DTB* pcb = calloc(1, sizeof(DTB));

	pcb->cabecera = malloc((sizeof(char) * strlen_null(cab)));
	memcpy(pcb->cabecera, cab, strlen_null(cab));

	pcb->codigo = codigo;
	pcb->id = id;
	pcb->flag = flag;
	pcb->pc = pc;
	pcb->path = malloc((sizeof(char) * strlen_null(path)) + 1);
	memcpy(pcb->path, path, strlen_null(path));

	pcb->quantum = quantum;
	pcb->posMemoria = posMemoria;

	pcb->recurso = malloc((sizeof(char) * strlen_null(recurso)) + 1);
	memcpy(pcb->recurso, recurso, strlen_null(recurso));

	pcb->archivos_abiertos =  list_create();
	pcb->archivos_abiertos = archivos_abiertos;

	return pcb;

}

char* serializarDTB(DTB* object){


	char* buffer = string_new();

	string_append(&buffer, object->cabecera);
	string_append(&buffer, "|");
	string_append(&buffer, armar_codigo(object->codigo));
	string_append(&buffer, "|");
	string_append(&buffer, string_itoa(object->id));
	string_append(&buffer, "|");
	string_append(&buffer, string_itoa(object->flag));
	string_append(&buffer, "|");
	string_append(&buffer, string_itoa(object->pc));
	string_append(&buffer, "|");
	string_append(&buffer, object->path);
	string_append(&buffer, "|");
	string_append(&buffer, string_itoa(object->quantum));
	string_append(&buffer, "|");
	string_append(&buffer, string_itoa(object->posMemoria));
	string_append(&buffer, "|");
	string_append(&buffer, object->recurso);
	string_append(&buffer, "|");
	int cant_archivo = 0;
	if( object->archivos_abiertos->elements_count )
		cant_archivo = list_size( object->archivos_abiertos );

	string_append(&buffer, string_itoa(cant_archivo));
	t_arhivo* archivo;
	if(cant_archivo > 0){

		for(int i=0; cant_archivo > i; i++){
			archivo = list_get(object->archivos_abiertos, i);
			string_append(&buffer, "|");
			string_append(&buffer, archivo->path );
			string_append(&buffer, "|");
			string_append(&buffer, string_itoa(archivo->posMemoria) );
		}


	} else {
		string_append(&buffer, "|");
		return buffer;
	}

	string_append(&buffer, "|");
	return buffer;


}

DTB* deszerializarDTB(char* buffer){

	DTB* object = malloc(sizeof(DTB));
	char** choclo = string_split(buffer, "|");

	object->archivos_abiertos = list_create();
	object->cabecera =  malloc((sizeof(char) * strlen_null(choclo[0])) + 1);
	memcpy(object->cabecera, choclo[0], string_length(choclo[0]));

	object->codigo = atoi(choclo[1]);
	object->id = atoi(choclo[2]);
	object->flag = atoi(choclo[3]);
	object->pc = atoi(choclo[4]);

	object->path =  malloc( (sizeof(char) * strlen_null(choclo[5])) );
	memcpy(object->path, choclo[5], string_length(choclo[5]));

	object->quantum = atoi(choclo[6]);
	object->posMemoria = atoi(choclo[7]);

	object->recurso =  malloc( (sizeof(char) * strlen_null(choclo[8])) );
	memcpy(object->recurso, choclo[8], string_length(choclo[8]));

	int cant_archivo = atoi(choclo[9]);

	int j = 10;
	t_arhivo* arc = malloc(sizeof(t_arhivo));;
	if(cant_archivo>0){

		for(int i = cant_archivo; i!=0; i--){

			arc->path = malloc( (sizeof(char) * strlen_null(choclo[j])) );
			memcpy(arc->path, choclo[j], string_length(choclo[j]));
			arc->posMemoria = atoi( choclo[++j] );
			list_add(object->archivos_abiertos, arc);
			j++;
		}

	} else {
		return object;
	}

	return object;


}



void destruir_DTB(DTB* pcb) {

	list_destroy(pcb->archivos_abiertos);
	free(pcb->cabecera);
	free(pcb->recurso);
	free(pcb->path);
	free(pcb);

}




