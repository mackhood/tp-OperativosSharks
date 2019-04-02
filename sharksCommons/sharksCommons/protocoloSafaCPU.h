
#ifndef SHARKSCOMMONS_PROTOCOLOSAFACPU_H_
#define SHARKSCOMMONS_PROTOCOLOSAFACPU_H_


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <string.h>
#include "protocolo.h"
#include "serealizacion.h"

#define SOLICITUD_CONEXION_CPU 211
#define CONEXION_ACEPTADA	   212
#define REQUEST_DTB_DUMMY	   215
#define BLOQUEAR_DTB_DUMMY	   214

#define FLAG_DTB_DUMMY 0
#define FLAG_DTB 1

#define EJECUTAR 10
#define FLAG_DTB_DUMMY 	0
#define FLAG_DTB 1
#define EJECUTO_SENTENCIA   15
#define CODIGO_SET_RAFAGA_PLANIFICACION 2



typedef struct {
	int posMemoria;
	char* path;
} t_arhivo;


typedef struct{
	char* cabecera;
	int codigo; //cod operación
	int id;	 //Id dtb
	int flag; //flag de inicializacion
	int pc;
	char* path; // ruta del escriptorio
	int quantum;
	int posMemoria;
	char* recurso;
	t_list* archivos_abiertos;
}DTB;

void destruir_DTB(DTB* pcb);

char* serializarDTB(DTB* object);

DTB* deszerializarDTB(char* buffer);

DTB* crearDTB(char* cabecera, int codigo, int id, int flag,
			int pc, char* path, int quantum, int posMemoria,
			char* recurso, t_list* archivos_abiertos);




#endif /* SHARKSCOMMONS_PROTOCOLOSAFACPU_H_ */
