#include "planificadorLP.h"

	/**
	 *Planificador de Largo Plazo el subsistema de PLP será el punto de entrada,
	  encargado de crear los DTB asociados a los programas G.DT y agregarlos a
	  la cola NEW. Además, será el encargado de mantener el grado de
	  multiprogramación del sistema, planificando los pases a READY según el algoritmo FIFO (First In,
	  First Out) . Estos pases a READY implicarán la carga en memoria del script Escriptorio asociado,
	  responsabilidad del Gestor de programas G.DT descrito más adelante.
	 */

	int actualIdGDT = 100 ;
	sem_t gradoMultiprogramacion;

	void initConfigPLP(){

		pthread_mutex_init(&mutexIdGDT,NULL);
		sem_init(&gradoMultiprogramacion,0, tSafa->config->quantum);
		logInfo("\nInit.. PLP , EL Grado de multiprogramacion inicial es :%d",tSafa->config->quantum);



	}

	/**
	 * Funcion encargado de retornar un id
	 * para identificar a un DTB en el sistema
	 */
	int getIdGDT(){
		int id;
		pthread_mutex_lock(&mutexIdGDT);
		id = actualIdGDT++;
		pthread_mutex_unlock(&mutexIdGDT);
		return id;
	}


	void destruir_dtb(DTB_SAFA* pcb) {
		free(pcb->path);
		free(pcb);
	}




	/**
	 * Logica para pasar un DTB_DUMMY de NEW to READY
	 * 1. Me permite seguir el  grado de multiprogramacion
	 * 	  si ? => siguo con 2.)
	 * 2. Desbloqueo el dtb_dummy
	 * 3. Obtengo el dtb de la cola new(Segun FIFO)
	 * 4. seteo los datos del dtb dummy en el dtbdummy
	 * 5. Envio el dtb de la cola new a pend_ready
	 * 5. Envio al DTB_DUMMY a la cola de Exec      //Para que el PCP lo envie a ejecutar
	 * 6. Aviso al PCP . signal
	 */


	void handlePlanificadorLP(t_safa* tSafa){
		logTrace("empezando plp");

		logTrace("Creando estructuras dummy");
		tSafa->dtb_dummy = calloc(1, sizeof(DTB_SAFA));
		tSafa->dtb_dummy->flag = 0;
		enviarABLOCK(tSafa->dtb_dummy);
		DTB_SAFA* dtbNuevo = calloc(1, sizeof(DTB_SAFA));


		while(true){
			if(gradoMultiprogramacionPosible(tSafa)  && hayCpuLibres(tSafa)
					&& estaEnBlockDummy(tSafa->dtb_dummy) && hayDTBNuevo()){

				logInfo( "Se inicia carga de memoria");
				decrementarGradoMultiProgramacion(tSafa);

				tSafa->dtb_dummy = getDTB_DUMMYofBLOCK();
				dtbNuevo = getDTBNew();
				enviarAPendReady(dtbNuevo);
				setDatoToDummy( dtbNuevo,tSafa->dtb_dummy );
				enviarDummyCpu(tSafa->dtb_dummy);
				enviarAReady(tSafa->dtb_dummy);
				logInfo("Se envio a ready dummy");

			}
		}
	}

	void setDatoToDummy(DTB_SAFA* dtb,DTB_SAFA* dtbDummy){

		if(dtbDummy->path != NULL){

			liberarMemoriaDTB_SAFA(dtbDummy);
		}

//		*dtbDummy=*dtb;
		dtbDummy->idGDT =dtb->idGDT;
		int tamDTB = string_length(dtb->path) + 1;
		dtbDummy->path = malloc(tamDTB);
		memcpy(dtbDummy->path,dtb->path,tamDTB);
		dtbDummy->tablaDeArchivosAbiertos = list_create();
		dtbDummy->flag = 0;

	}

	void enviarDummyCpu(DTB_SAFA* dtbDummy){

		DTB* dtbOrigin = map_dtb_safa_cpu(dtbDummy, OPERACION_DUMMY);

		char* mensaje= serializarDTB(dtbOrigin);
		CPU* cpu = buscarCPU();
		if(enviar(cpu->fd, mensaje)==-1){
			logError("Error al enviar datos a cpu");
		}

		logInfo("se envio dummy");
		destruir_DTB(dtbOrigin);
		free( mensaje );
	}

	int gradoMultiprogramacionPosible(t_safa* tsafa ){
		if(tsafa->config->gradoMultiprogramacion>0){
			return 1;
		}else {
				return 0;
		}
	}

	int hayCpuLibres (t_safa* tsafa ) {

		if(tsafa->cantidadCpuConectados!=0) {

			return 1;
		}else {
			return 0;
		}
	}

	int hayDTBNuevo() {

		if (queue_is_empty(tSafaEstados->new))
			return 0;

		return 1;
	}

	int estaEnBlockDummy(DTB_SAFA* dtbDummy){

		if ( getEstadoPlanficacion(dtbDummy) == ESTADO_COLA_BLOCK) {

			return 1;
		}else {

			return 0;
		}
	}

	void decrementarGradoMultiProgramacion(t_safa*  tSafa){
		tSafa->config->gradoMultiprogramacion --;
	}

	int noEstaListoDummy (  DTB_SAFA* dtbDummy){
		return dtbDummy->flag;
	}
