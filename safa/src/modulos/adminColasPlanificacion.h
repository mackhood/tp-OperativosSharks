
#ifndef SRC_MODULOS_PLANIFICADOR_ADMINCOLASPLANIFICACION_H_
#define SRC_MODULOS_PLANIFICADOR_ADMINCOLASPLANIFICACION_H_

#include <sharksCommons/protocoloSafaCPU.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <sharksCommons/logs.h>
#include <pthread.h>
#include <strings.h>
#include <semaphore.h>
#include <stddef.h>
#include <time.h>

#include "planificadorLP.h"
//#include "../safa/src/safa.h"
#define ESTADO_COLA_NEW   		1
#define ESTADO_COLA_READY   	2
#define ESTADO_COLA_EXEC   		3
#define ESTADO_COLA_BLOCK  	    4
#define ESTADO_COLA_EXIT    	5
#define ESTADO_NO_ENCONTRADO   -1
/*************** Estrucuturas *****************/

/** Modelado de las distintas colas de
 * 	planificacion
 **/


typedef struct {
	t_queue* new;			  //aca van los DTB asociado al programa cundo el usuario ejecuta un script
	t_list* pendReady ;	  //Aca paso los dtb que envie a cargar en memoria y esta pendiente de pasar a ready
	t_queue* ready;
    t_list* exec;
	t_list* block;
	t_queue* exit;
	t_queue* colaPrioridad;
	t_list* ListaPrioridad;
}t_safa_estado;


DTB_SAFA* crearDTBSAFA(int gdtId, char* path, int quantum);

/**
 * estructuras administrativa de gdt
 */




/*************** Variables Globales ***********/
t_safa_estado* tSafaEstados;

pthread_mutex_t mutexListNuevo;
pthread_mutex_t mutexListListo;
pthread_mutex_t mutexListFinalizado;
pthread_mutex_t mutexListBloqueado;
pthread_mutex_t mutexListEjecucion;
pthread_mutex_t mutexListPendReady;
pthread_mutex_t mutexListcolaPrioridad;
sem_t semPCbNew ;
sem_t semPcbReady;
sem_t semPcbReadyPrioridad;



DTB_SAFA* getDTBColaPrioridad();
void initConfigAdminColas();
void liberarMemoriaDTB_SAFA(DTB_SAFA*  );
// Cola new
void enviarANew(DTB_SAFA* dtb);
void signalDTBNew();
void waitDTBNew();
DTB_SAFA* get_elem_new_by_id(int id);

//cola pen_ready
DTB_SAFA* get_elem_penready_by_id(int id);


//cola ready
void waitDTBReadyPrioridad();
void signalDTBReadyPrioridad();

//cola exec
void enviarAEjecutar(DTB_SAFA * dtb);
DTB_SAFA* getDTBReady();
void waitDTBReady();
DTB_SAFA* buscar_pcb_exec_by_id( int pid );

//cola block
void enviarABLOCK(DTB_SAFA* dtb);
DTB_SAFA* get_elem_block_by_id(int);
void moverBlockToReady(DTB_SAFA* dtb);
void desbloquear_esperando_recurso( t_recurso* rec );

//cola exit
void signalDTBReady();

DTB_SAFA* getDTB_DUMMYofBLOCK();
DTB_SAFA*  getDTBNew();

//mover dtb entre colas
void removerDeExec(DTB_SAFA* );
void enviarAPendReady(DTB_SAFA* );
int getEstadoPlanficacion(DTB_SAFA* );
void enviarAReady(DTB_SAFA* );
void enviarAEXIT(DTB_SAFA* dtb);
void removerDeReady(DTB_SAFA* dtb);
void moverExecToReady(DTB_SAFA* );
void moverExecToBlock(DTB_SAFA* );
void moverBlockToExit(DTB_SAFA* dtb);
void moverExecToExit(DTB_SAFA* );
void moverPenReadyToExit(DTB_SAFA* dtb);
void moverReadyToExit(DTB_SAFA* dtb);
void moverNewToExit(DTB_SAFA* dtb);
void moverNewToReady(DTB_SAFA* dtb);
void moverReadyToExec(DTB_SAFA* dtb);
void enviarAColaPrioridad(DTB_SAFA* dtb);
void moverPendienteReadyToReady(DTB_SAFA* dtb);


#endif /* SRC_MODULOS_PLANIFICADOR_ADMINCOLASPLANIFICACION_H_ */
