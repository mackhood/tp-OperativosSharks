 /*============================================================================
 Name        : configuracionCPU.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*


/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include "configuracionCPU.h"
#include "cpu_2.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/



/* =================================================================
				    cpu
 =================================================================*/
void leer_config_cpu(int argc, char* argv[]) {
	int opt;
	t_config* config_aux = NULL;
	opterr = 1;
	iniciarDatos();

	while ((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
		case 'c':
			log_trace(mostrar_log_cpu, "Levantando config... %s\n", optarg);
			config_aux = config_create(optarg);
			break;
		case ':':
			log_warning(mostrar_log_cpu,
					"El parametro '-%c' requiere un argumento.\n",
					stderr, optopt);
			break;
		case '?':
		default:
			log_error(mostrar_log_cpu,
					"El parametro '-%c' es invalido. Ignorado.\n",
					stderr, optopt);
			break;
		}
	}

	configurar(config_aux);
	config_destroy(config_aux);
	log_debug(log_cpu, "puerto safa:%d  %s", config_cpu->puerto_safa,
			SUCCESS_BULLET);
	log_debug(mostrar_log_cpu, "ip safa:%s %s", config_cpu->ip_safa,
			SUCCESS_BULLET);
	log_debug(log_cpu, "puerto dam:%d  %s", config_cpu->port_dma,
			SUCCESS_BULLET);
	log_debug(log_cpu, "ip dam:%s %s", config_cpu->ip_dam, SUCCESS_BULLET);
	log_debug(log_cpu, "ip fm9:%s %s", config_cpu->ip_fm9, SUCCESS_BULLET);
	log_debug(log_cpu, "retardo:%d %s", config_cpu->retardo_cpu,
			SUCCESS_BULLET);
	log_trace(log_cpu, "Configuración %s", SUCCESS_BULLET);

}
void configurar(t_config* config) {
	if (config_incorrecta(config) != 0) {
		log_error(log_cpu, "Error al configurar CPU \n");
		morir();
	} else {
		log_trace(log_cpu, "Configuración %s", SUCCESS_BULLET);
	}

}
void iniciarDatos() {
	logCPU();
	iniciarLista();
	iniciarDTB();
}
int config_incorrecta(t_config* config) {
	config_cpu = malloc(sizeof(t_config_cpu));
	if (config == NULL) {

		log_error(log_cpu, "El parametro -c <config_file> es obligatorio.\n");
		morir();
		return EXIT_FAILURE;
	}
	int failures = 0;
	void validar(char* key) {
		if (!config_has_property(config, key)) {
			printf("Se requiere configurar \"%s\"\n", key);
			failures++;
		}
	}

	validar(CFG_PORT_DMA);
	validar(CFG_PORT_FM9);
	validar(CFG_PORT_SAFA);
	validar(CFG_IP_FM9);
	validar(CFG_IP_SAFA);
	validar(CFG_IP_DMA);
	validar(CFG_INTERVAL);

	if (failures > 0) {
		printf("Por favor revisar el archivo \"%s\"\n", config->path);
		morir();
		return EXIT_FAILURE;
	} else {
		config_cpu->puerto_safa = config_get_int_value(config, CFG_PORT_SAFA);


		config_cpu->ip_safa = string_duplicate(
				config_get_string_value(config, CFG_IP_SAFA));

		config_cpu->port_dma = config_get_int_value(config, CFG_PORT_DMA);

		config_cpu->ip_dam = string_duplicate(
				config_get_string_value(config, CFG_IP_DMA));

		config_cpu->port_fm9 = config_get_int_value(config, CFG_PORT_FM9);

		config_cpu->ip_fm9 = string_duplicate(
				config_get_string_value(config, CFG_IP_FM9));

		config_cpu->retardo_cpu = config_get_int_value(config, CFG_INTERVAL);

	}
	return (EXIT_SUCCESS);
}
void destuirConfiguracion() {

	//free(config_cpu->ip_dam);
	config_cpu->ip_dam = NULL;
	//free(config_cpu->ip_fm9);
	config_cpu->ip_fm9 = NULL;
	//free(config_cpu->ip_safa);
	config_cpu->ip_safa = NULL;
		free(config_cpu);
	}
void destruirDTBCPU() {

	list_destroy_and_destroy_elements(dtb->archivos_abiertos, free);
	free(dtb->cabecera);
	free(dtb->path);
	free(dtb->recurso);
	dtb->cabecera = NULL;
	dtb->path = NULL;
	dtb->recurso = NULL;
	free(dtb);
	dtb = NULL;
}
void logueoErrores(char* msj, int cod, int tipo, ...) {
	/*
		t_log* logger = log_create("fallos_del_sistema.log", "El GranDTp!",
				false, LOG_LEVEL_WARNING);

		t_log* errores = log_create("fallos_del_sistema.log", "El GranDTp!",
				true, LOG_LEVEL_WARNING);

		switch (tipo) {
		case LOG_LEVEL_MOSTRAR:
			log_error(errores, "Error %d: %s \n", cod, msj);
			break;
		case LOG_LEVEL_WARNING:
			log_warning(logger, "Cuidado! %d: %s \n", cod, msj);

			break;
		case LOG_LEVEL_ERROR:
			log_error(logger, "Error %d: %s \n", cod, msj);
			break;
		default:
			log_warning(errores, "Cuidado! %d: %s \n", cod, msj);

			break;
		}

		log_destroy(errores);

	 log_destroy(logger);*/
	}
void logCPU() {

		mostrar_log_cpu = log_create("info.log", "CPU!", true, LOG_LEVEL_TRACE);
		log_cpu = log_create("cpu.log", "CPU!", false, LOG_LEVEL_TRACE);
		log_trace(log_cpu, " Iniciando CPU ......");

	log_trace(mostrar_log_cpu, "Crear logs %s", SUCCESS_BULLET);

	log_info(log_cpu, "Se creó el archivo de log correctamente, cpu.log");
	}


