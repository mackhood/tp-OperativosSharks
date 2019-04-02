
#include "consolaGDTReques.h"


/**
 * Se crea un nuevo DTB  flag = 1
 * Flujo DTB_DUMMY ->Inicializacion :
 * 1.Se crea DTB
 * 2.Se envia a la cola NEW
 * 3.Se avisa al PLP que hay alguien en la cola de NEW
 */

//DTB_SAFA* dtb = malloc( sizeof(DTB_SAFA) );
void initNuevoGDT(char* pathScriptEscriptorio){

	int idGdt = getIdGDT();
	agregarGDT( crearGdt(idGdt) );
	enviarANew(crearDTBSAFA(idGdt, pathScriptEscriptorio, tSafa->config->quantum));

}

void mostrar_estado_cola(char* estado, int tam, t_list* lista){
	DTB_SAFA* dtb;

	for(int i=0; tam > i; i++ ){
		dtb =(DTB_SAFA*)list_get(lista , i);
		printf("ESTADO: %s  PID: %d  FLAG: %d  PATH: %s \n", estado,dtb->idGDT, dtb->flag, dtb->path);
	}
}

double prom_usaron_diego(int tam, t_list* lista){
	DTB_SAFA* dtb;
	//double ejecutadas = 0;
	double usaron_diego = 0;

	for(int i=0; tam > i; i++ ){
		dtb =(DTB_SAFA*)list_get(lista , i);
		//ejecutadas += dtb->pc;
		usaron_diego+= dtb->cant_usaron_diego;
	}
	return usaron_diego/tam;

}

double porc_usaron_diego(int tam, t_list* lista){
	DTB_SAFA* dtb;
	double ejecutadas = 0;
	double usaron_diego = 0;

	for(int i=0; tam > i; i++ ){
		dtb =(DTB_SAFA*)list_get(lista , i);
		ejecutadas += dtb->pc;
		usaron_diego+= dtb->cant_usaron_diego;
	}
	return (usaron_diego/ejecutadas)*100;

}


double prom_para_exit(int tam, t_list* lista){
	DTB_SAFA* dtb;
	double ejecutadas = 0;
	double total = 0;

	for(int i=0; tam > i; i++ ){
		dtb =(DTB_SAFA*)list_get(lista , i);
		ejecutadas += dtb->pc;
		total += dtb->total_sentencias;
	}
	return ejecutadas/tam;
}

double tiempo_repuesta(int tam, t_list* lista){
	DTB_SAFA* dtb;
	double tiempo = 0;

	for(int i=0; tam > i; i++ ){
		dtb =(DTB_SAFA*)list_get(lista , i);
		if(dtb->se_ejecuto){
			tiempo += dtb->tiempo_repuesta;
		}
	}
	return tiempo/tam;
}



void mostrarStatusTodo(){


	int cant_new =  list_size(tSafaEstados->new->elements);
	int cant_ready =  list_size(tSafaEstados->ready->elements);
	int cant_block =  list_size(tSafaEstados->block);
	int cant_exit =  list_size(tSafaEstados->exit->elements);
	int cant_exec =  list_size(tSafaEstados->exec);
	int cant_pendReady =  list_size(tSafaEstados->pendReady);


	puts("=======COLA NEW=======");
	pthread_mutex_lock(&mutexListNuevo);
	mostrar_estado_cola("NEW", cant_new, tSafaEstados->new->elements);
	pthread_mutex_unlock(&mutexListNuevo);

	puts("=======COLA EXIT=======");
	pthread_mutex_lock(&mutexListFinalizado);
	mostrar_estado_cola("EXIT", cant_exit, tSafaEstados->exit->elements);
	pthread_mutex_unlock(&mutexListFinalizado);

	puts("=======COLA READY=======");
	pthread_mutex_lock(&mutexListListo);
	mostrar_estado_cola("READY", cant_ready, tSafaEstados->ready->elements);
	pthread_mutex_unlock(&mutexListListo);

	puts("=======COLA EXEC=======");
	pthread_mutex_lock(&mutexListEjecucion);
	mostrar_estado_cola("EXEC", cant_exec, tSafaEstados->exec);
	pthread_mutex_unlock(&mutexListEjecucion);

	puts("=======COLA BLOCK=======");
	pthread_mutex_lock(&mutexListBloqueado);
	mostrar_estado_cola("BLOCK", cant_block, tSafaEstados->block);
	pthread_mutex_unlock(&mutexListBloqueado);

	puts("=======COLA PEND-READY=======");
	pthread_mutex_lock(&mutexListPendReady);
	mostrar_estado_cola("PEND", cant_pendReady, tSafaEstados->pendReady);
	pthread_mutex_unlock(&mutexListPendReady);


}

void liberarDTB(DTB_SAFA* dtb){
	free(dtb->path);
	free(dtb->tablaDeArchivosAbiertos);
	free(dtb->tablaDeArchivosAbiertos);
}
void mostrarStatus(int pid){

	bool existe(DTB_SAFA* element){
		return element->idGDT == pid;
	}

	bool estaAqui;
	DTB_SAFA* dtb;

	pthread_mutex_lock(&mutexListNuevo);
	estaAqui = list_any_satisfy( tSafaEstados->new->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find( tSafaEstados->new->elements, (void*)existe);
		printf("ESTADO: NEW  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListNuevo);
		return;
	}
	pthread_mutex_unlock(&mutexListNuevo);

	pthread_mutex_lock(&mutexListFinalizado);
	estaAqui = list_any_satisfy( tSafaEstados->exit->elements, (void*)existe);
	 if(estaAqui){
		dtb = list_find( tSafaEstados->exit->elements, (void*)existe);
		printf("ESTADO: EXIT  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListFinalizado);
		return;
	 }
	pthread_mutex_unlock(&mutexListFinalizado);


	pthread_mutex_lock(&mutexListListo);
	estaAqui = list_any_satisfy(tSafaEstados->ready->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->ready->elements, (void*)existe);
		printf("ESTADO: READY  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListListo);
		return;
	}
	pthread_mutex_unlock(&mutexListListo);


	pthread_mutex_lock(&mutexListEjecucion);
	estaAqui = list_any_satisfy(tSafaEstados->exec, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->exec, (void*)existe);
		printf("ESTADO: EXEC  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListEjecucion);
		return;
	}
	pthread_mutex_unlock(&mutexListEjecucion);


	pthread_mutex_lock(&mutexListBloqueado);
	estaAqui = list_any_satisfy(tSafaEstados->block, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->block, (void*)existe);
		printf("ESTADO: BLOCK  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListBloqueado);
		return;
	}
	pthread_mutex_unlock(&mutexListBloqueado);


	pthread_mutex_lock(&mutexListPendReady);
	estaAqui = list_any_satisfy(tSafaEstados->pendReady, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->pendReady, (void*)existe);
		printf("ESTADO: PEN-READY  PID: %d  FLAG: %d  PATH: %s \n", dtb->idGDT, dtb->flag, dtb->path);
		pthread_mutex_unlock(&mutexListPendReady);
		return;
	}
	pthread_mutex_unlock(&mutexListPendReady);

}

void finalizarById(int pid){

	bool existe(DTB_SAFA* element){
		return element->idGDT == pid;
	}

	bool estaAqui;
	DTB_SAFA* dtb;

	pthread_mutex_lock(&mutexListNuevo);
	estaAqui = list_any_satisfy( tSafaEstados->new->elements, (void*)existe);
	if(estaAqui){
		dtb = (DTB_SAFA*)list_find( tSafaEstados->new->elements, (void*)existe);
		moverNewToExit(dtb);
		pthread_mutex_unlock(&mutexListNuevo);
		return;
	}
	pthread_mutex_unlock(&mutexListNuevo);

	pthread_mutex_lock(&mutexListFinalizado);
	estaAqui = list_any_satisfy( tSafaEstados->exit->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->ready->elements, (void*)existe);
		printf("El DTB con pid = %d ya está finalizado \n", dtb->idGDT);
		pthread_mutex_unlock(&mutexListFinalizado);
		return;
	}

	pthread_mutex_unlock(&mutexListFinalizado);


	pthread_mutex_lock(&mutexListListo);
	estaAqui = list_any_satisfy(tSafaEstados->ready->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->ready->elements, (void*)existe);
		moverReadyToExit(dtb);
		pthread_mutex_unlock(&mutexListListo);
		return;
	}
	pthread_mutex_unlock(&mutexListListo);


	pthread_mutex_lock(&mutexListEjecucion);
	estaAqui = list_any_satisfy(tSafaEstados->exec, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->exec, (void*)existe);
		moverExecToExit(dtb);
		pthread_mutex_unlock(&mutexListEjecucion);
		return;
	}
	pthread_mutex_unlock(&mutexListEjecucion);


	pthread_mutex_lock(&mutexListBloqueado);
	estaAqui = list_any_satisfy(tSafaEstados->block, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->block, (void*)existe);
		moverBlockToExit(dtb);
		pthread_mutex_unlock(&mutexListBloqueado);
		return;
	}
	pthread_mutex_unlock(&mutexListBloqueado);


	pthread_mutex_lock(&mutexListPendReady);
	estaAqui = list_any_satisfy(tSafaEstados->pendReady, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->pendReady, (void*)existe);
		moverPenReadyToExit(dtb);
		pthread_mutex_unlock(&mutexListPendReady);
		return;
	}
	pthread_mutex_unlock(&mutexListPendReady);

}


void mostrarMetricaTodo(){

	//int cant_new =  list_size(tSafaEstados->new->elements);
	int cant_block =  list_size(tSafaEstados->block);
	int cant_exit =  list_size(tSafaEstados->exit->elements);
	int cant_exec =  list_size(tSafaEstados->exec);
	//int cant_pendReady =  list_size(tSafaEstados->pendReady);
	int cant_ready =  list_size(tSafaEstados->ready->elements);

	t_metrica* metrica = malloc( sizeof(t_metrica) );
	metrica->cant_colas = 0;
	//metrica->cant_dtb = cant_block + cant_exit + cant_exec + cant_pendReady + cant_new + cant_ready;

	pthread_mutex_lock(&mutexListFinalizado);
	double prom_diego_exit = prom_usaron_diego(cant_exit, tSafaEstados->exit->elements);
	double prom_eje_exit = prom_para_exit( cant_exit, tSafaEstados->exit->elements );
	double tiempo_exit =  tiempo_repuesta( cant_exit, tSafaEstados->exit->elements );
	double porc_exit = porc_usaron_diego( cant_exit, tSafaEstados->exit->elements );
	metrica->cant_colas++;
	pthread_mutex_unlock(&mutexListFinalizado);


	pthread_mutex_lock(&mutexListListo);
	double prom_diego_ready = prom_usaron_diego( cant_ready, tSafaEstados->ready->elements);
	double tiempo_ready =  tiempo_repuesta( cant_ready, tSafaEstados->ready->elements );
	double porc_ready = porc_usaron_diego( cant_ready, tSafaEstados->ready->elements );
	metrica->cant_colas++;
	pthread_mutex_unlock(&mutexListListo);


	pthread_mutex_lock(&mutexListEjecucion);
	double prom_diego_exec = prom_usaron_diego( cant_exec, tSafaEstados->exec);
	double tiempo_exec =  tiempo_repuesta( cant_exec, tSafaEstados->exec );
	double porc_exec = porc_usaron_diego( cant_exec, tSafaEstados->exec );
	metrica->cant_colas++;
	pthread_mutex_unlock(&mutexListEjecucion);


	pthread_mutex_lock(&mutexListBloqueado);
	double prom_diego_block = prom_usaron_diego( cant_block, tSafaEstados->block);
	double tiempo_block =  tiempo_repuesta( cant_block, tSafaEstados->block );
	double porc_block = porc_usaron_diego( cant_block, tSafaEstados->block );
	metrica->cant_colas++;
	pthread_mutex_unlock(&mutexListBloqueado);

	double prom_diego_total = (prom_diego_exit + prom_diego_ready + prom_diego_exec  + prom_diego_block)/metrica->cant_colas;
	double tiempo_respuesta = (tiempo_exit + tiempo_ready + tiempo_exec + tiempo_block)/metrica->cant_colas;
	double porc_usaron_diego = (porc_exit + porc_ready + porc_exec + porc_block)/metrica->cant_colas;

	printf("Promedio que usaron al diego %lf \n", prom_diego_total);
	printf("Tiempo de respuesta del sistema %lf \n", tiempo_respuesta);
	printf("Porcentaje que usaron al diego %lf \n", porc_usaron_diego);
	printf("Sentencias promedio para que termine en exit %lf \n", prom_eje_exit);
}

void mostrarMetrica(int pid){
	bool existe(DTB_SAFA* element){
		return element->idGDT == pid;
	}

	bool estaAqui;
	DTB_SAFA* dtb;
	double porc_diego;

	pthread_mutex_lock(&mutexListFinalizado);
	estaAqui = list_any_satisfy( tSafaEstados->exit->elements, (void*)existe);
	 if(estaAqui){
		dtb = list_find( tSafaEstados->exit->elements, (void*)existe);
		porc_diego =  (dtb->pc/dtb->cant_usaron_diego)*100;
		printf("DTB con id %d espero %d en la cola new \n", dtb->idGDT, dtb->cant_sentencia_new);
		printf("Porcenteja de sentencias ejecutadas que fueron al diego: %0.2lf \n", porc_diego);
		printf("DTB con id %d ejecutó %d hasta llegar a la cola exit \n", dtb->idGDT, dtb->pc);
		if(dtb->se_ejecuto)
			printf("Tiempo de respuesta dtb con id %d es %lf \n", dtb->idGDT, dtb->tiempo_repuesta);

	 }
	pthread_mutex_unlock(&mutexListFinalizado);


	pthread_mutex_lock(&mutexListListo);
	estaAqui = list_any_satisfy(tSafaEstados->ready->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->ready->elements, (void*)existe);
		porc_diego =  (dtb->pc/dtb->cant_usaron_diego)*100;
		printf("DTB con id %d espero %d en la cola new \n", dtb->idGDT, dtb->cant_sentencia_new);
		printf("Porcenteja de sentencias ejecutadas que fueron al diego: %0.2lf \n", porc_diego);
		if(dtb->se_ejecuto)
			printf("Tiempo de respuesta dtb con id %d es %lf \n", dtb->idGDT, dtb->tiempo_repuesta);

	}
	pthread_mutex_unlock(&mutexListListo);


	pthread_mutex_lock(&mutexListEjecucion);
	estaAqui = list_any_satisfy(tSafaEstados->exec, (void*)existe);
	if(estaAqui){
		dtb = list_find(tSafaEstados->exec, (void*)existe);
		porc_diego =  (dtb->pc/dtb->cant_usaron_diego)*100;
		printf("DTB con id %d espero %d en la cola new \n", dtb->idGDT, dtb->cant_sentencia_new);
		printf("Porcenteja de sentencias ejecutadas que fueron al diego: %0.2lf \n", porc_diego);
		if(dtb->se_ejecuto)
			printf("Tiempo de respuesta dtb con id %d es %lf \n", dtb->idGDT, dtb->tiempo_repuesta);

	}
	pthread_mutex_unlock(&mutexListEjecucion);


	pthread_mutex_lock(&mutexListBloqueado);
	estaAqui = list_any_satisfy(tSafaEstados->block, (void*)existe);
	if(estaAqui){
		porc_diego =  (dtb->pc/dtb->cant_usaron_diego)*100;
		dtb = list_find(tSafaEstados->block, (void*)existe);
		printf("DTB con id %d espero %d en la cola new \n", dtb->idGDT, dtb->cant_sentencia_new);
		printf("Porcenteja de sentencias ejecutadas que fueron al diego: %0.2lf \n", porc_diego);
		if(dtb->se_ejecuto)
			printf("Tiempo de respuesta dtb con id %d es %lf \n", dtb->idGDT, dtb->tiempo_repuesta);

	}
	pthread_mutex_unlock(&mutexListBloqueado);

	pthread_mutex_lock(&mutexListNuevo);
	estaAqui = list_any_satisfy( tSafaEstados->new->elements, (void*)existe);
	if(estaAqui){
		dtb = list_find( tSafaEstados->new->elements, (void*)existe);
		printf("Este dtb (id = %d) sigue en la cola new  \n", dtb->idGDT);

	}
	pthread_mutex_unlock(&mutexListNuevo);

}










