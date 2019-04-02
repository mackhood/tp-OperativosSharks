#include "safa.h"



pthread_t threadCpuDAM;
pthread_t threadPlanificadorCP;
pthread_t threadPlanificadorLP;
pthread_t threadConsola;

const int CANT_CPU = 2000;

//void hora(){
//
//	time_t tiempo1 = time(0);
//	struct tm *hora = localtime(&tiempo1);
//	char output[50];
//	strftime(output,50,"%H:%M:%S",hora);
//
//	sleep(5);
//	time_t tiempo2 = time(0);
//	double dif= difftime (tiempo2, tiempo1);
//	printf("diferencia %lf \n", dif );
//	printf("%s\n",output);
//}


int main(int argc,  char*argv[]) {
	/** Init configuracion safa **/
	initConfiguracion(argc, argv[1]);
	initThread();
	init_notify(argv[1]);


}

void initThread(){

	logInfo("Creando thread para atender las conexiones CPu's y dma");
	pthread_create(&threadCpuDAM, NULL, (void*) handler_conexion_dam_cpu, tSafa);
	estaOperativo();
	pthread_create(&threadConsola, NULL, (void*)handleConsolaGDT, tSafa);
	pthread_create(&threadPlanificadorLP, NULL, (void*)handlePlanificadorLP, tSafa);
	pthread_create(&threadPlanificadorCP, NULL, (void*)handlePlanificadorCP, tSafa);


	pthread_join(threadConsola ,NULL);
	pthread_join(threadPlanificadorLP ,NULL);
//	pthread_create(&notify_thread, NULL, thread_continuous_scan_notify,(void*)message1 );
	//pthread_join(threadPlanificadorCP ,NULL);
	//pthread_join(notify_thread,NULL);


}


void estaOperativo(){
	puts("SAFA esta en un estado corrupto...");
	while(1){
			if(tSafa->operativo){
				puts("SAFA esta operativo...");
				break;
			}
		}
}

/**
*	levanto el archivo de configuracion y mapeo
*  A una estructura .
*  Inicio configuracion de CPU, PCP;AdminColas
*  Envio DTB_DUMMY a la cola de bloqueados
*/
void initConfiguracion(int argc , char* argv){

	if(argc==1){
		   perror("Debes ingresar el path del arhivo de configuración \n");
		   exit(1);
	}

	t_config* config = config_create(argv);
	if(config  == NULL){
		perror("Error al abrir el archivo de configuracion");
		free(config);
		exit(1);
	}

	/**--------------- cargo todos los datos de safa.properties -------------------------**/
	t_safa_config* safaConfig= malloc(sizeof(t_safa_config));
	safaConfig->puerto =config_get_int_value(config,"PUERTO");

	char* algoritmo = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
	safaConfig->algoritmoPlanificacion = malloc( string_length(algoritmo) + 1 );
	strcpy(safaConfig->algoritmoPlanificacion, algoritmo);

	safaConfig->quantum = config_get_int_value(config,"QUANTUM");
	safaConfig->gradoMultiprogramacion = config_get_int_value(config,"GRADO_MULTIPRGRAMACION");
	safaConfig->retardoPlanificacion = config_get_int_value(config,"RETARDO_DE_PLANIFICACION");
	config_destroy(config);
	//free(algoritmo);

	//logueo todos los datos de configuracion
	initMutexlog(SAFA_LOG_PATH,PROGRAM_NAME,ACTIVE_CONSOLE,LOG_LEVEL_TRACE);

	logInfo("PUERTO :%d -QUANTUM: %d -GRADO_MULTIPROGRAMCION: %d -RETARDO_PLANFICION: %d -ALGORITMO_PLANFICACION: %s\n",
				safaConfig->puerto,
				safaConfig->quantum,
				safaConfig->gradoMultiprogramacion,
				safaConfig->retardoPlanificacion,
				safaConfig->algoritmoPlanificacion
	);

	tSafa = malloc(sizeof(t_safa));
	tSafa->config = safaConfig;

	//Configuro listen para para atender conexiones de los clientes -> CPUs|DAM
	logInfo("Conexiones cpu|dma -> Puerto:%d\n",safaConfig->puerto);
	//inicializo socket
	tSafa->fdServidor = initSocketNConexiones(safaConfig->puerto, CANT_CPU);
	tSafa->operativo = 0;
	tSafa->listCpu = list_create();
	initCPUConfiguracion();
	initConfigAdminColas();
	initConfigPLP();
	initConfigPCP(tSafa);

}

void init_notify(char* path_file) {
	if (watch_descriptor > 0 && fd_inotify > 0) {
		inotify_rm_watch(fd_inotify, watch_descriptor);
	}
	fd_inotify = inotify_init();
	if (fd_inotify > 0) {
		watch_descriptor = inotify_add_watch(fd_inotify, path_file, IN_MODIFY);
	}
}

void do_notify() {
	char buffer[BUF_LEN];
	int length = read(fd_inotify, buffer, BUF_LEN);
	int offset = 0;
	while (offset < length) {

		struct inotify_event *event = (struct inotify_event *) &buffer[offset];

		if (event->len) {
			if (event->mask & IN_CREATE) {
				if (event->mask & IN_ISDIR) {
					printf("The directory %s was created.\n", event->name);
				} else {
					printf("The file %s was created.\n", event->name);
				}
			} else if (event->mask & IN_DELETE) {
				if (event->mask & IN_ISDIR) {
					printf("The directory %s was deleted.\n", event->name);
				} else {
					printf("The file %s was deleted.\n", event->name);
				}
			} else if (event->mask & IN_MODIFY) {
				if (event->mask & IN_ISDIR) {
					printf("The directory %s was modified.\n", event->name);
				} else {
					printf("The file %s was modified.\n", event->name);
				}
			}
		}
		offset += sizeof(struct inotify_event) + event->len;
	}
}

void* thread_continuous_scan_notify(char* argv) {
	while (1) {
		FD_ZERO(&readfds);
		FD_SET(fd_inotify, &readfds);
		if (max_fd < fd_inotify)
			max_fd = fd_inotify;
		waiting.tv_sec = 0;
		waiting.tv_usec = 10;
		select(max_fd + 1, &readfds, NULL, NULL, &waiting);
		printf( "thread_continuous_scan_notify");
		if (FD_ISSET(fd_inotify, &readfds)) {

			do_notify();
			initNuevaConfiguracion(2,argv);


		}
	}
}





void initNuevaConfiguracion(int argc , char* argv){

	if(argc==1){
		   perror("Debes ingresar el path del arhivo de configuración \n");
		   exit(1);
	}

	t_config* config = config_create(argv);
	if(config  == NULL){
		perror("Error al abrir el archivo de configuracion");
		free(config);
		exit(1);
	}

	/**--------------- cargo todos los datos de safa.properties -------------------------**/
	t_safa_config* safaConfig= malloc(sizeof(t_safa_config));
	safaConfig->puerto =config_get_int_value(config,"PUERTO");

	char* algoritmo = config_get_string_value(config,"ALGORITMO_PLANIFICACION");
	safaConfig->algoritmoPlanificacion = malloc( string_length(algoritmo) + 1 );
	strcpy(safaConfig->algoritmoPlanificacion, algoritmo);

	safaConfig->quantum = config_get_int_value(config,"QUANTUM");
	safaConfig->gradoMultiprogramacion = config_get_int_value(config,"GRADO_MULTIPRGRAMACION");
	safaConfig->retardoPlanificacion = config_get_int_value(config,"RETARDO_DE_PLANIFICACION");
	config_destroy(config);
	//free(algoritmo);

	//logueo todos los datos de configuracion
	initMutexlog(SAFA_LOG_PATH,PROGRAM_NAME,ACTIVE_CONSOLE,LOG_LEVEL_TRACE);

	logInfo("PUERTO :%d -QUANTUM: %d -GRADO_MULTIPROGRAMCION: %d -RETARDO_PLANFICION: %d -ALGORITMO_PLANFICACION: %s\n",
				safaConfig->puerto,
				safaConfig->quantum,
				safaConfig->gradoMultiprogramacion,
				safaConfig->retardoPlanificacion,
				safaConfig->algoritmoPlanificacion
	);

	tSafa = malloc(sizeof(t_safa));
	tSafa->config = safaConfig;


}





