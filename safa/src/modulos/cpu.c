#include "cpu.h"




sem_t  semCpuLibre;
pthread_mutex_t  listaCpuMutex;

void initCPUConfiguracion(){
	pthread_mutex_init(&listaCpuMutex,NULL);
	sem_init(&semCpuLibre,1,0);
}

void handleNuevoCPU(int socketClient,t_safa* tSafa){
	CPU* cpu;
	cpu = crearCPU(socketClient,tSafa);
	logInfo("Conexión establecida con cpu. FD: %d", socketClient);
	if(enviar_mensaje(socketClient, CABECERA_SAFA, HANDSHAKE, "") ==-1)
		logWarning("error enviar handshake con cpu. puede que se haya desconectado");

	logInfo("Creando nuevo CPU asociado a fd :%d",socketClient);
	agregarCPU(cpu);

}

CPU* crearCPU(int socketClient, t_safa* safa){
	CPU* cpu = malloc(sizeof(CPU));
	cpu->fd = socketClient;
	cpu->libreEjecucion = true;  // esta libre para la ejecucion
	cpu->t_safa = safa;
	return cpu;
}


void bloquear_dummy(int socket, DTB* c_dtb){
	DTB_SAFA* s_dtb;
	s_dtb =	map_dtb_cpu_safa(c_dtb);
	CPU* cpu = findCPUByFd(socket);

	if(s_dtb->flag==0){

		removerCPU(cpu);
		setEstadoLibreCPU(cpu);
		agregarCPU(cpu);
		moverExecToBlock(s_dtb);
		logInfo("Se envia pcb dummy a bloqueado pid:  %d", s_dtb->idGDT);
	}


}

void desalojar(int socket, DTB* c_dtb){
	DTB_SAFA* s_dtb;
	s_dtb =	map_dtb_cpu_safa(c_dtb);
	CPU* cpu = findCPUByFd(socket);

	if(cpu==NULL){
		logError("Error buscando cpu socke: %d", socket);
		return;
	}

	removerCPU(cpu);
	setEstadoLibreCPU(cpu);
	agregarCPU(cpu);
	moverExecToBlock(s_dtb);
	logInfo("Se envia pcb a bloqueado pid:  %d", s_dtb->idGDT);
}

DTB_SAFA* map_dtb_cpu_safa(DTB* c_dtb){

	DTB_SAFA* pcb = malloc( sizeof(DTB_SAFA) );
	pcb->flag = c_dtb->flag;
	pcb->idGDT = c_dtb->id;
	pcb->pc = c_dtb->pc;
	pcb->posMemoria = c_dtb->posMemoria;
	pcb->quantum = c_dtb->quantum;
	pcb->tablaDeArchivosAbiertos = c_dtb->archivos_abiertos;
	pcb->path = malloc( strlen_null(c_dtb->path) );

	memcpy(pcb->path, c_dtb->path, strlen_null( c_dtb->path ));

	return pcb;


}


DTB* map_dtb_safa_cpu(DTB_SAFA* c_dtb,int codigo){

	DTB* pcb = (DTB*)calloc (1, sizeof(DTB) );
	pcb->cabecera= strdup(CABECERA_SAFA);
	pcb->codigo = codigo;
	pcb->flag = c_dtb->flag;
	pcb->id = c_dtb->idGDT;
	pcb->path = malloc( (sizeof(char)*string_length(c_dtb->path)) + 1);
	pcb->quantum=c_dtb->quantum;
	pcb->archivos_abiertos = c_dtb->tablaDeArchivosAbiertos;
	memcpy(pcb->path, c_dtb->path, string_length( c_dtb->path ) + 1);
	pcb->recurso=strdup("");
	return pcb;


}




void abortarDTB(int socket, DTB* c_dtb){
	CPU* cpu;
	DTB_SAFA* s_dtb;
	s_dtb =	map_dtb_cpu_safa(c_dtb);
	cpu = findCPUByFd(socket);

	if(cpu==NULL){
		logError("Error buscando cpu socke: %d", socket);
		return;
	} else {
		removerCPU(cpu);
		setEstadoLibreCPU(cpu);
		agregarCPU(cpu);
		moverExecToExit(s_dtb);
		logInfo("Se envia pcb a exit pid:  %d", s_dtb->idGDT);
	}


}
void agregarCPU(CPU* cpu){
	pthread_mutex_lock(&listaCpuMutex);

	list_add(tSafa->listCpu,cpu);
	sem_post(&semCpuLibre);

	pthread_mutex_unlock(&listaCpuMutex);
}
/**
 * Busqueda de una cpu libre para poder
 * ser usado en el planificador de  corto plazo
 * para enviar a ejecutar un dtb
 * Se decrementa el semaforo contador
 * de CPUs libres.
 */
CPU* buscarCPU(){
	bool estaLibre(CPU* element){
	   return element->libreEjecucion;
	}
	CPU* cpu= calloc (1,sizeof(CPU));
	sem_wait(&semCpuLibre); //cada vez que se ocupa uno descremento el semaforo
	pthread_mutex_lock(&listaCpuMutex);

	cpu = list_find(tSafa->listCpu, (void*)estaLibre);
	cpu->libreEjecucion = false;

	pthread_mutex_unlock(&listaCpuMutex);
	logInfo("La cpu esta ocupada fd :%d",cpu->fd);

	return cpu;
}

bool estaLibre(CPU* element){

		return element->libreEjecucion;
}




/*
 * Elimino la cpu de la lista CPUs
 */
void removerCPU(CPU* cpu) {

	bool predicado(void* element) {
		CPU* cpuTemp = element;
		return cpuTemp->fd == cpu->fd;
	}

	if (cpu != NULL) {

		if (cpu->libreEjecucion == true) {
			sem_wait(&semCpuLibre);
		}
		pthread_mutex_lock(&listaCpuMutex);
		list_remove_and_destroy_by_condition(tSafa->listCpu, predicado,(void*) eliminarCPU);
		pthread_mutex_unlock(&listaCpuMutex);
	}
}
/*
 * Busco una CPU por fd
 */

CPU* findCPUByFd(int fd) {
	bool predicado(void* element) {
		CPU* cpuTemp = element;
		return cpuTemp->fd == fd;
	}
	CPU* cpu;
	pthread_mutex_lock(&listaCpuMutex);
		cpu = list_find(tSafa->listCpu , predicado);
	pthread_mutex_unlock(&listaCpuMutex);

	return cpu;
}
/**
 * Marco la cpu como libre
 * Aumento el semaforo contador de CPUs libre
 */
void setEstadoLibreCPU(CPU* cpu){
	pthread_mutex_lock(&listaCpuMutex);
	cpu->libreEjecucion = true;
	sem_post(&semCpuLibre);
	pthread_mutex_unlock(&listaCpuMutex);

}
void eliminarCPU(CPU* cpu) {
	if (cpu != NULL) {
		cpu->t_safa = NULL;
		free(cpu);
	}
}



void operacion_wait(int socket, DTB* dtb){
	logInfo("Operacion wait..");

	GDT* gdt = buscar_gdt_by_id(dtb->id);
	if( dictionary_has_key(tabla_recursos, dtb->recurso) ){

		t_recurso* rec = get_recurso( dtb->recurso );

		if(rec->valor > 0){

			rec->valor--;
			asigna_recurso(rec, gdt);
			actualizar_recurso(dtb->recurso, rec);
			dtb->codigo = OPERACION_WAIT_OK;
			char* mensaje_ok = serializarDTB( dtb );

			if(enviar(socket, mensaje_ok ))
				logError("Error al enviar dtb a cpu");

			free( mensaje_ok );

		} else {

			queue_push(rec->en_espera, gdt);
			actualizar_recurso(dtb->recurso, rec);
			dtb->codigo = OPERACION_WAIT_ERROR;
			char* mensaje_error = serializarDTB( dtb );
			if(enviar(socket, mensaje_error ))
				logError("Error al enviar dtb a cpu");

			free( mensaje_error );
		}

		destroyrecurso(rec);
	} else {

		crear_recurso_en_sistema(dtb->recurso, crear_recurso(dtb->recurso) );
		t_recurso* rec = get_recurso( dtb->recurso );

		if(rec->valor > 0){
			rec->valor--;
			asigna_recurso(rec, gdt);
			actualizar_recurso(dtb->recurso, rec);
			dtb->codigo = OPERACION_WAIT_OK;
			char* mensaje_ok = serializarDTB( dtb );

			if(enviar(socket, mensaje_ok ))
				logError("Error al enviar dtb a cpu");

			free( mensaje_ok );

		} else {
			queue_push(rec->en_espera, gdt);
			actualizar_recurso(dtb->recurso, rec);
			dtb->codigo = OPERACION_WAIT_ERROR;
			char* mensaje_error = serializarDTB( dtb );
			if(enviar(socket, mensaje_error ))
				logError("Error al enviar dtb a cpu");

			free( mensaje_error );
		}
	  destroyrecurso(rec);
	}
	//actualizarGDT(gdt);
}


void operacion_signal(int socket, DTB* dtb){
	logInfo("Operacion signal..");

	if( dictionary_has_key(tabla_recursos, dtb->recurso) ){

		t_recurso* rec = get_recurso( dtb->recurso );
		rec->valor++;
		actualizar_recurso(dtb->recurso, rec);
		desbloquear_esperando_recurso( rec );
		dtb->codigo = OPERACION_SIGNAL_OK;
		char* mensaje_ok = serializarDTB( dtb );
		if(enviar(socket, mensaje_ok ))
			logError("Error al enviar dtb a cpu");

		free( mensaje_ok );

	} else {

		crear_recurso_en_sistema(dtb->recurso, crear_recurso(dtb->recurso) );
		dtb->codigo = OPERACION_SIGNAL_OK;
		char* mensaje_ok = serializarDTB( dtb );
		if(enviar(socket, mensaje_ok ))
			logError("Error al enviar dtb a cpu");
		free( mensaje_ok );
	}
}






