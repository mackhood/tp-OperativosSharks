
#include "dma.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

t_config* config;
pthread_t pthPeticionCpu;

const static int cantidad_conexiones=3000;

int main(int argc , char *argv[]){

    init_configuracion(argc, argv[1]);
    init_servidores();

	fd_dma = initSocketNConexiones(dma_struct->puerto_dma, cantidad_conexiones); //iniciar servidor

	while (true) {
		 int socket = aceptarConexionSocket(fd_dma);

         if( socket < 0){
        	 perror("error en operacion accept");
        	 exit(EXIT_FAILURE);
         }
         logInfo("Creando thread para atender las conexiones CPu's");

         char* mensaje_handshake = recibir(socket, TAM_RECIBIR);
         char* cod = string_substring(mensaje_handshake,  1, 2);
         if( atoi(cod) == HANDSHAKE){
        	 logInfo("Mensaje Recibido: %s ", mensaje_handshake);
        	 if (enviar(socket, HANDSHAKE_DAM) == -1){
        		 logWarning("error enviar handshake con cpu. puede que se haya desconectado");
        		 continue;
        	 }

//        	 DTB_DAM* object =  crearDTBDAM(CABECERA_CPU, 0, 100,
//        			 "/home/fifa-examples/fifa-entrega/Archivos/scripts/complejo.escriptorio_2", 0, 5);
//        	 char* msj = serializarDTBDAM( object );
//        	 enviar(socket, msj);

         } else {

        	 logError("Error handshake cpu");
        	 perror("Error handshake con cpu");
        	 exit(EXIT_FAILURE);
         }

         free(mensaje_handshake);
         free(cod);
         pthread_create(&pthPeticionCpu, NULL, (void*)atenderCpu, &socket);
	}
    return 0;
}

void atenderCpu(int* socket){

		while (true) {
			char* mensaje_recibido_cpu = recibir(*socket, TAM_RECIBIR);

			char* cod = get_codigo( mensaje_recibido_cpu );
			if(atoi(cod)== 66){
				logError("Error, se desconectó cpu");
				free(cod);
				free(mensaje_recibido_cpu);
				break;
			}
			free(cod);

			DTB_DAM* dtb = deszerializarDTBDAM(mensaje_recibido_cpu);
			switch (dtb->cod) {

			case OPERACION_ABRIR:
				operacion_abrir(mensaje_recibido_cpu, ABRIR_ARCHIVO_SAFA);
				break;
			case OPERACION_DUMMY_DMA:
				operacion_abrir(mensaje_recibido_cpu, DUMMY_SAFA);
				break;
			case OPERACION_FLUSH:
				operacion_flush(mensaje_recibido_cpu);
				break;

			case OPERACION_CREAR:
				operacion_crear(mensaje_recibido_cpu);
				break;
			case OPERACION_BORRAR:
				operacion_borrar(mensaje_recibido_cpu);
				break;
			default:
				logWarning("No reconoce operacion enviada por cpu");

		}
		free(mensaje_recibido_cpu);
		destruir_DTBDAM(dtb);
	}
}



void init_configuracion(int argc , char* argv){

	if(argc==1){
	   perror("Debes ingresar el path del arhivo de configuración \n");
	   exit(1);
	}


	config = config_create(argv);
	if(config  == NULL){
		perror("Error al abrir el archivo de configuracion");
		exit(1);
	}

	dma_struct = malloc(sizeof(t_dma_config));
	dma_struct->puerto_dma =config_get_int_value(config, DFG_PUERTO_DMA);
	dma_struct->puerto_safa =config_get_int_value(config, DFG_PUERTO_SAFA);
	dma_struct->puerto_mdj = config_get_int_value(config, DFG_PUERTO_MDJ);
	dma_struct->puerto_fm9 = config_get_int_value(config,DFG_PUERTO_FM9);

	char* ip_safa = config_get_string_value(config,DFG_IP_SAFA);
	dma_struct->ip_safa = malloc( string_length(ip_safa) + 1 );
	memcpy(dma_struct->ip_safa, ip_safa,  string_length(ip_safa) + 1);

	char* ip_mdj = config_get_string_value(config, DFG_IP_MDJ);
	dma_struct->ip_mdj = malloc( string_length(ip_mdj) + 1 );
	memcpy(dma_struct->ip_mdj, ip_mdj,  string_length(ip_mdj) + 1);

	char* ip_fm9 = config_get_string_value(config, DFG_IP_FM9);
	dma_struct->ip_fm9 = malloc( string_length(ip_fm9) + 1 );
	memcpy(dma_struct->ip_fm9, ip_fm9,  string_length(ip_fm9) + 1);

	dma_struct->t_size = config_get_int_value(config, DFG_T_SIZE);

	config_destroy(config);

	pthread_mutex_init(&mutexSafa, NULL);
	pthread_mutex_init(&mutexMdj, NULL);
	pthread_mutex_init(&mutexFm9, NULL);

	initMutexlog(DMA_LOG_PATH,"DMA",1,LOG_LEVEL_TRACE);
	logInfo("PUERTO DMA: %d", dma_struct->puerto_dma);
	logInfo("PUERTO SAFA: %d", dma_struct->puerto_safa);
	logInfo("PUERTO MDJ: %d", dma_struct->puerto_mdj);
	logInfo("PUERTO FM9: %d", dma_struct->puerto_fm9);
	logInfo("IP SAFA: %s", dma_struct->ip_safa);
	logInfo("IP MDJ: %s", dma_struct->ip_mdj);
	logInfo("IP FM9: %s", dma_struct->ip_fm9);
	logInfo("Transferencia : %d", dma_struct->t_size);



}


void init_servidores(){

	fd_safa = conectarCliente(dma_struct->ip_safa, dma_struct->puerto_safa);
	if(fd_safa==FAIL){
		logError("Error conectarse con safa");
		return;
	}

	if(enviar_mensaje(fd_safa, CABECERA_DMA, HANDSHAKE, "")==-1)
		logWarning("error enviar handshake con SAFA.");

	char* mensajeInicialSafa = recibir(fd_safa,TAM_RECIBIR);
	char* cod_safa = string_substring(mensajeInicialSafa, 0, 3);

	if(strcmp(cod_safa, HANDSHAKE_SAFA)==0 ){
		logInfo("Conexion establecida con safa..");
	} else {
		logWarning("No se pudo realizar handshake con safa..");
		perror("error al conectarse con safa");
	}
	free(cod_safa);
	free(mensajeInicialSafa);

	fd_fm9 = conectarCliente(dma_struct->ip_fm9, dma_struct->puerto_fm9);

	if(fd_fm9==FAIL){
		logError("Error conectarse con fm9");
		return;
	}


	if(enviar_mensaje(fd_fm9, CABECERA_DMA, HANDSHAKE, "")==-1)
		logWarning("error enviar handshake a Fm9.");

	char* mensajeInicialfm9 = recibir(fd_fm9,TAM_RECIBIR);

	char* cod_fm9 = string_substring(mensajeInicialfm9, 0, 3);
	char* tam_linea = get_mensaje( mensajeInicialfm9 );
	tam_max_linea = atoi( tam_linea );
	if(strcmp(cod_fm9, HANDSHAKE_FM9)==0 ){
		logInfo("Conexión establecida con fm9..");
	} else {
		logWarning("No se pudo realizar handshake con fm9..");
		perror("error al conectarse con safa");
	}

	free(mensajeInicialfm9);
	free(cod_fm9);
	free( tam_linea );


	fd_mdj = conectarCliente(dma_struct->ip_mdj, dma_struct->puerto_mdj);

	if(fd_mdj==FAIL){
		logError("Error handshake con mdj");
		return;
	}

	if(enviar_mensaje(fd_mdj, CABECERA_DMA, HANDSHAKE, "")==-1)
		logWarning("error al enviar handshake a MDJ.");


	char* mensajeInicialMdj = recibir(fd_mdj,TAM_RECIBIR);
	char* cod_mdj = string_substring(mensajeInicialMdj, 0, 3);

	if(strcmp(cod_mdj, HANDSHAKE_MDJ)==0 ){

		logInfo("Conexion establecida con mdj..");

	} else {
		logWarning("No se pudo realizar handshake con mdj..");
		perror("error al conectarse con mdj");
		return;
	}
	free(cod_mdj);
	free(mensajeInicialMdj);
}
