#include "mercadoDeJugadores.h"
#include <pthread.h>
#include "../properties/fileSystem.h"
#include "../properties/conexiones.h"
#include "../properties/configurar.h"
#include <sharksCommons/sockets.h>
#include <sharksCommons/protocolo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/

#define TAM_RECIBIR 1024
/* =================================================================
				    MDJFileSystem
 =================================================================*/
int main(int argc, char* argv[]) {

	signal(SIGINT, verificar_senial);
	inicializar(argc, argv);
	signal(SIGINT, finalizar_por_senial);

	pthread_create(&consola_thread, NULL, &consolaMDJ, NULL);
	pthread_create(&conect, NULL, &conectarFifa, NULL);

	pthread_attr_setdetachstate(&at_conect, PTHREAD_CREATE_DETACHED);
	pthread_attr_setdetachstate(&conect, PTHREAD_CANCEL_ENABLE);

	pthread_detach(&at_conect);
	pthread_detach(&consola_thread);



	for (;;)
		;


	morir();

	return (0);
}
/* =================================================================
				FIFA FileSystem
 =================================================================*/
void* crearArchivo(char* path, int linea) {

	FILE* archivo = fopen(path, "wb+");
	size_t tam = 0;
	//fclose(archivo);
	char* dato = "\n";
	fseek(archivo, 0, SEEK_SET);
	do {
		tam = fwrite(dato, sizeof(dato), 1, archivo);
		fseek(archivo, 0, SEEK_END);
	} while (tam != linea);
	fclose(archivo);
	return (EXIT_SUCCESS);
	/* TODO Crear Archivoch
	 Parámetros: [Path, Cantidad de Líneas]
	 Descripción: Cuando el El Diego reciba la operación de crear un archivo deberá llamar a esta operación que creará el archivo dentro del path solicitado. El archivo creado deberá tener la cantidad de bloques necesarios para guardar las líneas indicadas por la operación crear con su contenido vacío.*/
}

int tamanioArchivo(char* path) {

	int valor;

	if (open(path, O_RDONLY, 0777) == -1) {
		perror("Error al abrir el archivo\n");
	}
	struct stat *buffer = calloc(1, sizeof(struct stat));
	stat(path, buffer);

	valor = buffer->st_size;
	free(buffer);
	return valor;
}

/* =================================================================
			CONSOLA
 =================================================================*/
void consolaMDJ(char* argv[]){
	initialize_readline();

	while (done != 1) {
		lineasComandos = readline("Mercado de Jugadores: ");

		if (!lineasComandos) {
			log_error(log_consola, "AlgoSalioMal %s\n", FAILURE_BULLET);
			done = 1;
			break;
		};
		argumentos = stripwhite(lineasComandos);
		if (*argumentos) {
			add_history(argumentos);
			ejecutar_linea(argumentos);
		};
		free(lineasComandos);
		lineasComandos = NULL;
	};
//  nombre_programa = argv[3];
	/*initialize_readline();
	int done = 0;
	//com_ayuda();
  while(done != 1){
	  lineas = readline("Mercado de Jugadores: ");

      if (!lineas){
    	  log_error(log_consola,"AlgoSalioMal %s\n",FAILURE_BULLET);
    	  done = 1;
			free(lineas);
			free(argumentos);
			clear_history();
			rl_free((char*) lineas);
    	  break;
      };

      argumentos = stripwhite(lineas);
		//free(lineas);

      if (*argumentos){
          add_history(argumentos);
          ejecutar_linea(argumentos);
      };

		//free(argumentos);
		//clear_history();
		//rl_free_line_state();
		//rl_free((char*) lineas);
    };
	free(argumentos);
	free(lineas);

	rl_clear_history();
	rl_free((char*) lineas);
	 */
	clear_history();
}
void eliminarPath(char* pathArchivo) {
	log_trace(log_mdj, "Se elimina el path %s", pathArchivo);

	remove(pathArchivo);
}
/* =================================================================
				Type: CONFIGURACIONES
 =================================================================*/
void morir(){
	imRun = false;

	liberarMemoria();
	log_trace(log_mdj, "Fin de la Terminal %s", SUCCESS_BULLET);
	//pthread_attr_destroy(&at_conect);
	//pthread_attr_destroy(&at_consola_thread);

///	close(fd_dam);
	//close(socket_mdj);
	log_debug(log_mdj, "Cerrar conexiones %s", SUCCESS_BULLET);
	//log_destroy(internal_log);
	//finArbol(config_mdj->punto_de_montaje);
	//config_destroy(config_mdj_aux);
	destruirConfiguracion();

	log_trace(log_mdj, "Borrar configuración %s", SUCCESS_BULLET);
	final_readline();
	cerrar();
	log_debug(log_mdj, "Finalizo la Consola %s", SUCCESS_BULLET);
	dest_logFS();
	log_trace(log_mdj, "Cerrar logs FiFa %s", SUCCESS_BULLET);

	//	destruir_E_MDJ(jugador);
	chauMemoria();
	log_trace(log_mdj, "Liberar Memoria %s", SUCCESS_BULLET);

	//pthread_attr_destroy(&consola_thread);
	//eliminarEstructura(comando);

	log_trace(log_mdj, "Fin de ejecución %s", SUCCESS_BULLET);
	log_destroy(log_mdj);
	//pthread_exit(&conect);
	//pthread_exit((pthread_t*) consola_thread);
	//pthread_exit((pthread_t*) &conect);
	pthread_cancel(conect);
	printf("Finalizar Hilos %s", SUCCESS_BULLET);



}


void conectarFifa() {

	if (conexionesMDJ() != 0) {
		morir();
	}
	ejecucion();
}
void esperarDAM() {
	int dam = 0;
	int control;

	while (dam < 1) {
		char* mensaje_handshake = recibir(fd_dam, TAM_RECIBIR);
		char* handshake = string_substring(mensaje_handshake, 0, 3);
		if (strcmp(handshake, HANDSHAKE_DAM) == 0) {
			log_info(log_mdj, "Conexión con elDiego %s", SUCCESS_BULLET);
			control = enviar(fd_dam, HANDSHAKE_MDJ);
			if (control != -1) {
				log_trace(internal_log, "Se envió HANDSHAKE_DAM %s", HANDSHAKE_DAM);
				dam = 8;
			} else {
				log_error(internal_log, "Error al enviar HANDSHAKE_DAM %s",
						FAILURE_BULLET);
			log_warning(log_mdj, "Este no es elDiego %s", handshake);
			log_warning(log_mdj, "Eliminado %s", FAILURE_BULLET);
			close(fd_dam);
			dam++;
			}
		}
		free( mensaje_handshake );
		free( handshake );

	}
}
int conexionesMDJ() {

	int yes = 1;

	struct sockaddr_in servidor;
	int c;

	//Creating socket
	if ((socket_mdj = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		log_error(log_mdj, "Error al crear socket: %s", FAILURE_BULLET);
	}

	setsockopt(socket_mdj, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	servidor.sin_port = htons(config_mdj->puerto_fifa);

	//Binding socket
	if (bind(socket_mdj, (struct sockaddr*) &servidor,
			sizeof(struct sockaddr_in)) != 0) {
		log_error(log_mdj, "Error en el Bind socket: %s", FAILURE_BULLET);
	} else {
		log_trace(log_mdj, "Crear socket: %s", SUCCESS_BULLET);
	}
	//Listening socket
	if (listen(socket_mdj, BACKLOG) != 0) {
		log_error(log_mdj, "Error al escuchar socket: %s", FAILURE_BULLET);
		return (ERROR_OPERACION);
	} else {
		log_trace(log_mdj, "Escuchando socket %s", SUCCESS_BULLET);
	};

	c = sizeof(struct sockaddr_in);

	//accept connection from an incoming client
	fd_dam = accept(socket_mdj, (struct sockaddr*) &servidor,
			(socklen_t*) &c);
	if (fd_dam < 0) {
		log_error(log_mdj, "Error al aceptar al Diego %s", FAILURE_BULLET);
	} else {
		log_debug(log_mdj, "elDiego conectado: %d%s", fd_dam,
				SUCCESS_BULLET);
	}

	esperarDAM();
	return EXIT_SUCCESS;
}




/* =================================================================
				Type: EJECUCIÓN
 =================================================================*/
void ejecucion() {
	char *mensaje;
	int codigo;

	float retardo = ((config_mdj->retardo_fifa) / 100);
	while (imRun) {

		sleep(retardo);

		esperarMsj();

		codigo = jugador->clave;
		log_trace(internal_log, "Cabecera: %s", jugador->cabecera);
		log_trace(internal_log, "Código: %d", jugador->cod);
		log_trace(internal_log, "Path: %s", jugador->path);	// = "basura7.txt";
		log_trace(internal_log, "Base: %d", jugador->base);
		log_trace(internal_log, "Offset: %d", jugador->offset);
		log_trace(internal_log, "Clave: %d", jugador->clave);
		log_trace(internal_log, "Linea: %d", jugador->lineas);
		log_trace(internal_log, "Buff: %s", jugador->buffer);
		log_trace(internal_log, "tamanio: %d", jugador->tamArchivo);

		jugador->lineas = 15;
		jugador->path = string_duplicate("/scripts/cpu_bound.escriptorio");
		//jugador->path = string_duplicate("/equipo/sacachispa.txt");
		jugador->buffer = string_duplicate("Holaaaa\n como va");
		jugador->tamArchivo = (sizeof(char))
				* strlen_null_mdj("Holaaaa\n como va");
		jugador->offset = 1;
		codigo = CREAR_ARCHIVO;
		switch (codigo) {

		case ABRIR_ARCHIVO:

			log_trace(internal_log, "Path para abrir: %s", jugador->path);
			abrir_archivo(jugador->path);

			break;
		case CREAR_ARCHIVO:
			log_trace(internal_log, "Líneas: %d Crear archivo: %s ",
					jugador->lineas, jugador->path);
			//jugador->clave = EXITO_OPERACION;
			//mensaje = serializar_E_MDJ(jugador);
			//enviar(fd_dam, mensaje);
			//int lineas;
			//lineas = jugador->lineas;
			crear_archivo_mdj(jugador->path, jugador->lineas);
			actualizar_bitmap();

			break;
		case BORRAR_ARCHIVO:

			borrar_archivo(jugador->path);
			log_trace(internal_log, "Path a Borrar: %s", jugador->path);
			jugador->clave = EXITO_OPERACION;
			mensaje = serializar_E_MDJ(jugador);
			enviar(fd_dam, mensaje);
			actualizar_bitmap();
			break;
		case OBTENER_DATOS:
			log_trace(internal_log, "Datos: %s, Offset: %d Size: %d",
					jugador->path,
					jugador->offset, jugador->tamArchivo);
			datos(jugador->path, jugador->offset, jugador->tamArchivo);
			actualizar_bitmap();
			break;
		case GUARDAR_DATOS:

			guardarDatos(jugador->path, jugador->offset, jugador->tamArchivo,
					jugador->buffer);
			log_trace(internal_log, "Path: %s, Offset: %d Size: %d Datos: %s",
					jugador->path, jugador->offset, jugador->tamArchivo,
					jugador->buffer);
			actualizar_bitmap();
			break;
		default:
			log_warning(log_mdj, "%d", codigo);
			log_error(log_mdj, "Mensaje incorrecto: %d", codigo);
			flag = 1;
			break;
		}
	}
	free(mensaje);
   }

/* =================================================================
				Type: FUNCIONES
 =================================================================*/
void verificar_senial(int num) {
	signal(SIGINT, SIG_DFL);
	log_error(log_mdj, "No se entiende señal\t\t Intentar otra vez\n");
}
void esperarSenial(int seg) {
	if (signal(SIGINT, verificar_senial) == SIG_ERR) {
		log_error(log_mdj, "%s No se puede cambiar senial", FAILURE_BULLET);
	}
}
void finalizar_por_senial(int sig) {

	signal(sig, SIG_IGN);
	log_error(log_mdj, "%s Abortando FIFA \n", FAILURE_BULLET);
	morir();

}
void validar(char* path){
/* TODO Validar Archivo
	  Parámetros: [Path]
	  Descripción: Cuando el El Diego reciba la operación de abrir un archivo deberá validar que el archivo exista.*/

	if (access(path, F_OK) != -1) {
		log_trace(internal_log,"Archivo: %s válido \n", path);
		//puts("EL ARCHIVO EXSISTE");
		//return true;
	}else{
		log_error(internal_log,"Archivo: %s No es válido \n",path);
		// |return false;
	}
}
void esperarMsj() {
	log_debug(fifa_log, "Esperando mensajes");

	char* mensaje = recibir(fd_dam, TAM_RECIBIR);
	log_trace(internal_log, "deserializar mensaje: %s", mensaje);
	printf("\n mensaje: %s\n", mensaje);
	jugador = deserializar_E_MDJ(mensaje);
	free(mensaje);
	mensaje = NULL;
}
void cerrar(){
	rl_clear_history();
	//rl_free((char*) lineasComandos);
	//free(lineasComandos);
	clear_history();
}




