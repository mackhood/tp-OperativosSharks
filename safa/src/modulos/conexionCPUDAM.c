#include "conexionCPUDAM.h"

#define TAM_RECIBIR_SAFA 1024


/**
 * Logica para manejar las conexiones de CPUs | DAM
 * que llegan a SAFA
 *
 */

pthread_t mensaje_cpu;
pthread_t mensaje_dma;
const int CANTIDAD_CONEXIONES_SAFA = 5000;
int conexionProcesoDiego = 0;

void handler_conexion_dam_cpu(t_safa* tSafa){


		int sockets[CANTIDAD_CONEXIONES_SAFA];
		int i = 0;
		//int new_socket;
		tSafa->cantidadCpuConectados=0;

 	    while(true){

 	    	if ((sockets[i] = aceptarConexionSocket(tSafa->fdServidor)) <0){
				perror("error en operacion accept");
				exit(EXIT_FAILURE);
			}

 	    	char* mensaje_recibido = recibir(sockets[i], TAM_RECIBIR_SAFA);
 	    	char* header = get_header(mensaje_recibido);
		if(comparar_header(CABECERA_CPU, header ))	{
			  handleNuevoCPU(sockets[i],tSafa);
			  tSafa->cantidadCpuConectados++;
			  int op = (tSafa->cantidadCpuConectados && conexionProcesoDiego)?1:0;
			  tSafa->operativo=op;
//
//			  t_list* archivos_abiertos = list_create();
//			  	DTB* dtb = crearDTB(CABECERA_CPU, PEDIDO_DESALOJO_DUMMY, 100, 1, 0,
//			  		"path/archivo", 2, 4, "recurso", archivos_abiertos);
//			  	char* mensajeaenviar = serializarDTB( dtb );
//			  	enviar(sockets[i],   mensajeaenviar);
//
//			  	char* rec = recibir(sockets[i], 2000);
//			  	DTB* des = deszerializarDTB(rec );
//
//			  	puts( des->path );

//			  	C|22|100|1|0|path/archivo|2|4|recurso|0|

			  pthread_create(&mensaje_cpu, NULL, (void*) recibir_mensaje_cpu, &sockets[i]);

		} else if (comparar_header(CABECERA_DMA, header ) ){
			  logInfo("conexion establecida con DMA..");
			  tSafa->fdDma = sockets[i];
			  conexionProcesoDiego = 1;
			  if( enviar_mensaje(sockets[i], CABECERA_SAFA, HANDSHAKE, "") ==-1)
				  logWarning("error enviar handshake con DMA. Puede que se haya desconectado");


			  int op = (tSafa->cantidadCpuConectados && conexionProcesoDiego)?1:0;
			  tSafa->operativo=op;
			  pthread_create(&mensaje_dma, NULL, (void*) recibir_mensaje_dma, &sockets[i]);
		}
			free(header);
			free(mensaje_recibido);
			i++;
 	    }
	}


void recibir_mensaje_cpu(int* socket){

	while(true){
		char* mensaje_recibido = recibir(*socket,TAM_RECIBIR_SAFA);
		DTB* dtb = deszerializarDTB( mensaje_recibido );
		switch( dtb->codigo ){

			case PEDIDO_DESALOJO_DUMMY:
				bloquear_dummy(*socket, dtb);
				break;

			case PEDIDO_DESALOJO_ABRIR:
				desalojar(*socket, dtb);
				break;
			case PEDIDO_DESALOJO_FLUSH:
				desalojar(*socket, dtb);
				break;
			case PEDIDO_DESALOJO_CREAR:
				desalojar(*socket, dtb);
				break;
			case PEDIDO_DESALOJO_BORRAR:
				desalojar(*socket, dtb);
				break;

			case PEDIDO_WAIT:
				operacion_wait(*socket, dtb);
				break;

			case PEDIDO_DESALOJO_WAIT:
				abortarDTB(*socket, dtb);
				break;

			case PEDIDO_SIGNAL:
				operacion_signal(*socket, dtb);
				break;

			//errores en operacion asignar
			case FILE_NO_OPEN:
				abortarDTB(*socket, dtb);
				break;
			case FALLO_SEGMENTO:
				abortarDTB(*socket, dtb);
				break;
			case SIN_ESPACIO_FM9:
				abortarDTB(*socket, dtb);
				break;

			//errores en operacion close
			case ARCH_NO_OPEN:
				abortarDTB(*socket, dtb);
				break;
			case FAIL_MEMORIA:
				abortarDTB(*socket, dtb);
				break;


			default:
				logWarning("No reconoce operacion enviada por cpu");

		 }
		free(mensaje_recibido);
		destruir_DTB( dtb );
	}



}



void recibir_mensaje_dma(int* socket){


	while(true){
		char* mensaje_recibido_dam = recibir(*socket,TAM_RECIBIR_SAFA);
		t_dmv* estructura_dma = deserializar_t_dmv( mensaje_recibido_dam );

		switch( estructura_dma->cod ){
			case CREAR_ARCHIVO_SAFA:
				desbloquear_proceso( estructura_dma );
				break;
			case BORRAR_ARCHIVO_SAFA:
				desbloquear_proceso( estructura_dma );
				break;
			case ABRIR_ARCHIVO_SAFA:
				finalizacion_abrir( estructura_dma );
				break;
			case DUMMY_SAFA:
				finalizacion_dummy( estructura_dma );
				break;
			case GENERIC_ERROR:
				abortar_by_error( estructura_dma );
				break;
			case ERROR:
				abortar_by_error( estructura_dma );
				break;
		}
		free( mensaje_recibido_dam );
		destroy_t_dmv( estructura_dma );

	}

}



