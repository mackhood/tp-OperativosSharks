 /*============================================================================
 Name        : configurar.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include "bitmap.h"
#include "configurar.h"
#include "consola_linux.h"
#include "../src/mercadoDeJugadores.h"
#include <sharksCommons/protocolo.h>
//#include <sharksCommons/protocolo_msj.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/


/* =================================================================
					MDJFileSystem
 =================================================================*/
	void configurarMDJ(t_config* config) {
		if (configurarFIFA(config) != 0) {
			log_error(log_mdj, "Error al configurar CPU");
			morir();
		} else {
		log_debug(log_mdj, "puerto:%d %s", config_mdj->puerto_fifa,
				SUCCESS_BULLET);
		log_debug(log_mdj, "ip:%s %s", config_mdj->ip, SUCCESS_BULLET);
		log_debug(log_mdj, "punto_de_montaje:%s %s",
				config_mdj->punto_de_montaje,
				SUCCESS_BULLET);
		log_debug(log_mdj, "retardo:%d %s", config_mdj->retardo_fifa,
		SUCCESS_BULLET);
		log_trace(log_mdj, "Configuración %s", SUCCESS_BULLET);
			log_trace(log_mdj, "Configuración %s", SUCCESS_BULLET);
		}
	}
	void inicializar(int argc, char *argv[]) {
		logFS();
		logMDJ();
	crearMdjConfig();
		imRun = true;
		leer_config(argc, argv);
		empezar();
		memoria();
	}

	void memoria() {
	dir = string_new();
	sentencia = string_new();
	lineasComandos = string_new();
	argumentos = string_new();
	syscom = string_new();

	}
void chauMemoria() {
	free(syscom);
	free(argumentos);
	free(dir);
	free(r);
	free(sentencia);
	free(word);

	word = NULL;
	syscom = NULL;
	r = NULL;
	dir = NULL;
	argumentos = NULL;
	sentencia = NULL;

	log_trace(log_consola, "Saliendo del Mercado de Pases");
	log_destroy(log_consola);
}
void liberarMemoria() {

	list_destroy_and_destroy_elements(tabla, free);
	chauMemoria();
	free(word);
	word = NULL;

	metadata->magic = NULL;
	free(metadata);
	liberarBitmap();

	//bitarray_destroy(bitarray);

}
void leer_config(int argc, char* argv[]) {
		int opt;
		t_config* config;
	opterr = 1;

		while ((opt = getopt(argc, argv, "c:")) != -1) {
			switch (opt) {
			case 'c':
				printf("Levantando config... %s\n\n", optarg);
				config = config_create(optarg);
				break;
			case ':':
				fprintf(stderr, "El parametro '-%c' requiere un argumento.\n",
						optopt);
				break;
			case '?':
			default:
				fprintf(stderr, "El parametro '-%c' es invalido. Ignorado.\n",
						optopt);
				break;
			}
		}

		configurarMDJ(config);
		config_destroy(config);
	}

	void empezar() {

		t_archivos = malloc(sizeof(t_archivo));
		metadata = malloc(sizeof(t_metadata));

		tabla = list_create();
		//metadata->magic = string_new();

	char* montaje = string_duplicate(config_mdj->punto_de_montaje);
	pathRaiz(montaje);
	free(montaje);
	montaje = NULL;
		int meta;
		meta = readMetadata();
		int bitm;
	//bitm = leerBitmap();
	bitm = crearBitarray();
		if (meta == -1 || bitm == -1) {
			log_error(fifa_log, "Error al leer archivos iniciales");
			morir();
		}
	}

void crearMdjConfig() {

	config_mdj = malloc(sizeof(t_config_mdj));
	//config_mdj->ip = malloc(sizeof(char) * strlen_null("127.0.0.1\0"));
	//config_mdj->punto_de_montaje = string_new();
	config_mdj->puerto_fifa = -1;
	config_mdj->retardo_fifa = -1;

	}
	void destruirConfiguracion() {

	free(config_mdj->ip);
	config_mdj->ip = NULL;
	free(config_mdj->punto_de_montaje);
		config_mdj->punto_de_montaje = NULL;
		/*free(config_mdj->puerto_fifa);
		 free(config_mdj->punto_de_montaje);
		 free(config_mdj->retardo_fifa);
		 free(config_mdj->ip);
		 */
	free(config_mdj);
	config_mdj = NULL;

	}
	void logMDJ() {
		log_mdj = log_create("Mercado_De_Jugadores.log", "Mercado De Jugadores",
		true, LOG_LEVEL_TRACE);
		log_trace(log_mdj, "\tAbierto el Mercado de Jugadores...");
		logConsola();
	}
	void logFS() {
		alogFS();
		internal_log = log_create("internal.log", "FIFA ♣ ", false, LOG_LEVEL_TRACE);
	}
	;
	void crear_MDJ_CREAR(int clave) {
		jugador->cabecera = CABECERA_MDJ;
		jugador->clave = clave;

	}
int configurarFIFA(t_config* config_mdj_aux) {

	if (config_mdj_aux == NULL) {

		log_error(log_mdj, "El parametro -c <config_file> es obligatorio.\n");
			morir();
			return EXIT_FAILURE;
		}

		int failures = 0;
	void validar(char* key) {
		bool config = config_has_property(config_mdj_aux, key);
		if (!config) {
			printf("Se requiere configurar %s\n", key);
				failures++;
			}
		}
	validar(CFG_PORT_MDJ);
	validar(CFG_IP_MDJ);
	validar(CFG_PATH);
	validar(CFG_RETARDO_FIFA);


		if (failures > 0) {
			log_warning(log_mdj, "Por favor revisar el archivo %s", config_mdj_aux->path);
			log_error(internal_log, "Error al configurar");
		return (EXIT_FAILURE);
		} else {
		char* ip = config_get_string_value(config_mdj_aux, CFG_IP_MDJ);
		//config_mdj->ip = string_new();
		config_mdj->ip = string_duplicate(ip);


		config_mdj->puerto_fifa = config_get_int_value(config_mdj_aux,
				CFG_PORT_MDJ);
		char* montaje = config_get_string_value(config_mdj_aux, CFG_PATH);
		config_mdj->punto_de_montaje = string_duplicate(montaje);


		config_mdj->retardo_fifa = config_get_int_value(config_mdj_aux,
				CFG_RETARDO_FIFA);

		config_mdj_aux->path = NULL;
		free(ip);
		free(montaje);
		montaje = NULL;
		ip = NULL;
		return (EXIT_SUCCESS);
		}

	}
