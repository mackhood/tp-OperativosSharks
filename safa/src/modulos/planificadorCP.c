

#include "planificadorCP.h"


void initConfigPCP(t_safa* tSafa){
//	logInfo("Incializando colas de planificacion");
//	initConfigAdminColas();
}


void handlePlanificadorCP(t_safa* tSafa) {
	logInfo("INICIO -> handlePlanificadorCP .... ");

	int algoritmo;

	logInfo("Se inicio el Planficador Corto Plazo");

	algoritmo = getAlgoritmoPlanificacion(tSafa->config->algoritmoPlanificacion);



	switch (algoritmo) {
	case ROUND_ROBIN:
		logInfo("Se ejecuta algoritmo Round Robin");
		ejecutarAlgoritmo(tSafa,algoritmo);//
		break;
	case VRROUND_ROBIN:
		ejecutarAlgoritmo(tSafa,algoritmo);
		logInfo("Se ejecuta algoritmo Virtual Round Robin");
		break;

	case SJF_RR_1:
	logInfo("Se ejecuta algoritmo Round Robin");
		ejecutarAlgoritmo(tSafa,algoritmo);
		break;

	default:
		logError("El algoritmo ingresaso es invalido");
		exit (-1);
		break;
	}

	logInfo("FIN -> handlePlanificadorCP .... ");
}

int getAlgoritmoPlanificacion(char* algoritmo){

    if(string_equals_ignore_case(algoritmo,NAME_ROUND_ROBIN)){
        return ROUND_ROBIN;
    }
	if(string_equals_ignore_case(algoritmo,NAME_V_ROUND_ROBIN)) {
        return VRROUND_ROBIN;
    }
    if(string_equals_ignore_case(algoritmo,NAME_SJF_RR_1)) {
        return SJF_RR_1;
    }
    return -1;
}

/**
 * Funcionalidad del dispatcher
 * asignar cpu a un programa GDT, en este caso DTB que es
 * el elemento planficable
 * 1.Obtengo el dtb de la cola de ready
 * 2.Mando a ejecutar en la CPU que fue seleccionada (libre)
 * 3.
 *
 */
void ejecutarAlgoritmo(t_safa* tSafa,int algoritmo){



	while (1){

		if(hayCpuLibres(tSafa)&& !queue_is_empty(tSafaEstados->ready)) {
		algoritmo = getAlgoritmoPlanificacion(tSafa->config->algoritmoPlanificacion);


	if (algoritmo == ROUND_ROBIN) {

		logInfo("Ejecutando algoritmo  Round robin..");
		DTB_SAFA* dtb1; //=  malloc(sizeof(DTB_SAFA)); no hace falta
		dtb1 = getDTBReady();
		CPU* cpu; //= malloc(sizeof(CPU)); no hace falta
		cpu = buscarCPU();

		if( !dtb1->se_ejecuto ){
			/* se_ejecuto es un flag para saber si
			*  ya se ejecutó antes y asi calcular
			*  el tiempo de respuesta*/
			dtb1->tiempo_repuesta = difftime(time(0), dtb1->horacreacion);
			dtb1->se_ejecuto = true;

		}

		moverReadyToExec( dtb1 ); //  tambien se debe remover de exec para permitir mas dtb en memoria (ready)
		//enviarAEjecutar( dtb1 );
		logInfo("Inicio de ejecucion del DTB:%d en la cpu: %d", dtb1->idGDT,cpu->fd);
		DTB* dtb3 = map_dtb_safa_cpu(dtb1, EJECUTAR);
		char* mensaje = serializarDTB( dtb3 );
		if( enviar(cpu->fd, mensaje)==-1 ){
			logError("Error al enviar dtb a cpu");
		}
		logInfo("se envio dtb a cpu");
		destruir_DTB(dtb3);
		pthread_t thread_pcp_cpu_RR;
		pthread_create(&thread_pcp_cpu_RR, NULL, (void*)handlPCP_CPURR ,  cpu);
		pthread_detach(thread_pcp_cpu_RR);
	}

	if(algoritmo == VRROUND_ROBIN){

		DTB_SAFA* dtb1 =  malloc(sizeof(DTB_SAFA));
		if(!list_is_empty(tSafaEstados->colaPrioridad->elements))
			dtb1 = getDTBColaPrioridad();
		dtb1=getDTBReady();
		logInfo("Ejecutando algoritmo ");

		CPU* cpu = malloc(sizeof(CPU));
		cpu = buscarCPU();
		enviarAEjecutar(dtb1);
		logInfo("Inicio de ejecucion del DTB:%d en la cpu: %d", dtb1->idGDT,cpu->fd);
		DTB* dtb3 = map_dtb_safa_cpu(dtb1,EJECUTAR);
		char * mensaje = serializarDTB(dtb3);
		enviar_mensaje(cpu->fd,CABECERA_SAFA,EJECUTAR,mensaje);
		logInfo("se envio dtb a cpu");
		destruir_DTB( dtb3 );
		pthread_t thread_pcp_cpu_VRR;
		pthread_create(&thread_pcp_cpu_VRR, NULL, (void*)handlPCP_CPUVRR ,  cpu);
		pthread_detach(thread_pcp_cpu_VRR);
	} if( algoritmo == SJF_RR_1) {

		DTB_SAFA* dtb1 =  malloc(sizeof(DTB_SAFA));
		//	armarListaColaPrioridad();
		//dtb1=getDTBListaPrioridad();//DEBE SER UNA LISTA NO UNA COLA
		logInfo("Ejecutando algoritmo ");
		CPU* cpu = malloc(sizeof(CPU));
		cpu = buscarCPU();
		enviarAEjecutar( dtb1 );
		logInfo("Inicio de ejecucion del DTB:%d en la cpu: %d", dtb1->idGDT,cpu->fd);
		DTB* dtb3 = map_dtb_safa_cpu(dtb1,EJECUTAR);
		char  *mensaje = serializarDTB(dtb3);
		enviar_mensaje(cpu->fd,CABECERA_SAFA,EJECUTAR,mensaje);
		logInfo("se envio dtb a cpu");
		destruir_DTB(dtb3);
		pthread_t thread_pcp_cpu_SJF;
		pthread_create(&thread_pcp_cpu_SJF, NULL, (void*)handlSJF_RR_1 , cpu);
		pthread_detach(thread_pcp_cpu_SJF);
	}
  }
}}


/**
 * Manejador de pedidos y respuesta
 * por parte de la cpu;
 */

void handlPCP_CPURR(CPU* cpu){
	logInfo("INICIANDO ->ROUND ROBIN");


	int quantum = tSafa->config->quantum ;
	do  {
		char* mensaje = recibir(cpu->fd,1024); // LA RECEPCION DE MENSAJE NO ESTA BIEN DEFINIDA JANIEL MODIFICAR

		DTB* dtb  = deszerializarDTB(mensaje);
		DTB_SAFA* dtb4 =map_dtb_cpu_safa(dtb);



	switch(dtb->codigo){

		case PEDIDO_DESALOJO_DUMMY:
						bloquear_dummy(cpu->fd, dtb);
						break;

				logInfo("CPU envio request para bloquear dtb_dummy");
				break;

		case PEDIDO_DESALOJO_ABRIR:
			desalojar(cpu->fd, dtb);
			break;
		case PEDIDO_DESALOJO_FLUSH:
			desalojar(cpu->fd, dtb);
			break;
		case PEDIDO_DESALOJO_CREAR:
			desalojar(cpu->fd, dtb);
			break;
		case PEDIDO_DESALOJO_BORRAR:
			desalojar(cpu->fd, dtb);

			break;


		case PEDIDO_WAIT:
			operacion_wait(cpu->fd, dtb);
			quantum --;
			dtb->quantum=quantum;

			break;

		case PEDIDO_DESALOJO_WAIT:
				abortarDTB(cpu->fd, dtb);
				dtb->quantum=quantum;

				break;

		case PEDIDO_SIGNAL:
				operacion_signal(cpu->fd, dtb);
				quantum --;
				dtb->quantum=quantum;
				break;



		case 02://ejecuto una linea
				quantum --;
				if(quantum<0){

				enviarABLOCK(dtb4);
				return;
				}
				break;

			case 03://abortar dtb
			enviarAEXIT(dtb4);
				break;
			case 05:// Finalizo ejecucion de todas las lineas
				enviarAEXIT(dtb4);
				break;

			case 00:
				enviarABLOCK(dtb4);

				break;

			case FILE_NO_OPEN:
				abortarDTB(cpu->fd, dtb);
				break;
			case FALLO_SEGMENTO:
				abortarDTB(cpu->fd, dtb);
				break;
			case SIN_ESPACIO_FM9:
				abortarDTB(cpu->fd, dtb);
				break;

			//errores en operacion close
			case ARCH_NO_OPEN:
				abortarDTB(cpu->fd, dtb);
				break;
			case FAIL_MEMORIA:
				abortarDTB(cpu->fd, dtb);
				break;


			default:


				logWarning("No reconoce operacion enviada por cpu");


				break;
		}

	free(mensaje);
	}while (quantum >0);


}


void handlPCP_CPUVRR(CPU* cpu){
	logInfo("INICIANDO ->virtual round robin");


	int quantum = tSafa->config->quantum ;
	do  {
		char* mensaje = recibir(cpu->fd,1024);
		DTB* dtb  = deszerializarDTB(mensaje);
		DTB_SAFA* dtb2= map_dtb_cpu_safa(dtb);
				switch(dtb->codigo){


			case PEDIDO_DESALOJO_DUMMY:
			bloquear_dummy(cpu->fd, dtb);

			logInfo("CPU envio request para bloquear dtb_dummy");
			break;

			case 102://ejecuto una linea
				quantum --;
				if(quantum<0){

					enviarABLOCK(dtb2);
				}
				break;
			case 103://abortar dtb
				enviarAEXIT(dtb2);
				break;

			case 105:// Finalizo ejecucion
				if (quantum>0){

					dtb2->quantum=quantum;
					enviarAColaPrioridad( dtb2 );
				}//si el quantum  es 0 lo envia a exit
					enviarAEXIT(dtb2);
				break;

				case PEDIDO_DESALOJO_ABRIR:
						desalojar(cpu->fd, dtb);
						break;
					case PEDIDO_DESALOJO_FLUSH:
						desalojar(cpu->fd, dtb);
						break;
					case PEDIDO_DESALOJO_CREAR:
						desalojar(cpu->fd, dtb);
						break;
					case PEDIDO_DESALOJO_BORRAR:
						desalojar(cpu->fd, dtb);

						break;


					case PEDIDO_WAIT:
						operacion_wait(cpu->fd, dtb);
						quantum --;
						dtb->quantum=quantum;
						if(quantum<=0){
							desalojar(cpu->fd,dtb);
						}

						break;

					case PEDIDO_DESALOJO_WAIT:
							dtb->quantum=quantum;
							abortarDTB(cpu->fd, dtb);

							break;

					case PEDIDO_SIGNAL:
							operacion_signal(cpu->fd, dtb);
							quantum --;
							dtb->quantum=quantum;
							if(quantum<=0){
								desalojar(cpu->fd,dtb);
								return;
							}

							break;


						case FILE_NO_OPEN:
							abortarDTB(cpu->fd, dtb);
							break;
						case FALLO_SEGMENTO:
							abortarDTB(cpu->fd, dtb);
							break;
						case SIN_ESPACIO_FM9:
							abortarDTB(cpu->fd, dtb);
							break;

						//errores en operacion close
						case ARCH_NO_OPEN:
							abortarDTB(cpu->fd, dtb);
							break;
						case FAIL_MEMORIA:
							abortarDTB(cpu->fd, dtb);
							break;




			default:
				break;
		}free(mensaje);
	}while (quantum >0);

	logInfo("FIN ->handlerPCP_CPUVRR");
}

void handlSJF_RR_1(CPU* cpu){
	logInfo("INICIANDO ->ROUND ROBIN");

	int quantum = 1 ;
	do {
		char* mensaje = recibir(cpu->fd,1024);

		DTB* dtb  = deszerializarDTB(mensaje);
		DTB_SAFA* dtb4 =map_dtb_cpu_safa(dtb);

	switch(dtb->codigo){


			case 102://ejecuto una linea
				quantum --;
				dtb4->pc++;
//				if(dtb4->siguiente_io>0)
//					dtb4->siguiente_io --;
				dtb4->siguiente_io=(int)queue_pop(dtb4->prox_io);

				break;

			case 103://abortar dtb

			enviarAEXIT(dtb4);

				break;
			case 95:// Finalizo ejecucion de todas las lineas
				enviarAEXIT(dtb4);
				break;

			case 90: // BLoquear DTB
				enviarABLOCK(dtb4);
				break;
			case 07: //termino su quantum y se lo manda a bloqueados

				enviarABLOCK(dtb4);
				break;
			default:
				break;


			case PEDIDO_DESALOJO_DUMMY:
			bloquear_dummy(cpu->fd, dtb);

			logInfo("CPU envio request para bloquear dtb_dummy");
			break;

			case PEDIDO_DESALOJO_ABRIR:

					desalojar(cpu->fd, dtb);
					break;
				case PEDIDO_DESALOJO_FLUSH:
					desalojar(cpu->fd, dtb);
					break;
				case PEDIDO_DESALOJO_CREAR:
					desalojar(cpu->fd, dtb);
					break;
				case PEDIDO_DESALOJO_BORRAR:
					desalojar(cpu->fd, dtb);

					break;


				case PEDIDO_WAIT:
					operacion_wait(cpu->fd, dtb);

					break;

				case PEDIDO_DESALOJO_WAIT:
						abortarDTB(cpu->fd, dtb);


						break;

				case PEDIDO_SIGNAL:
						operacion_signal(cpu->fd, dtb);
											break;




					case FILE_NO_OPEN:
						abortarDTB(cpu->fd, dtb);
						break;
					case FALLO_SEGMENTO:
						abortarDTB(cpu->fd, dtb);
						break;
					case SIN_ESPACIO_FM9:
						abortarDTB(cpu->fd, dtb);
						break;

					//errores en operacion close
					case ARCH_NO_OPEN:
						abortarDTB(cpu->fd, dtb);
						break;
					case FAIL_MEMORIA:
						abortarDTB(cpu->fd, dtb);
						break;








	} }while (quantum >0);

	logInfo("FIN ->handlerPCPRR_CPU");


}

void armarListaColaPrioridad() {

	bool menosOperaciones(void * t1,void* t2){
		DTB_SAFA* dtb1=t1;
		DTB_SAFA* dtb2=t2;
		return dtb1->siguiente_io>dtb2->siguiente_io ||
				dtb1->cant_sentencia_new-dtb1->pc
				<dtb1->pc-dtb1->pc;
	}

	list_sort(tSafaEstados->ListaPrioridad,(void*)menosOperaciones);

}







