/*
 ============================================================================
 Name        : CPU.c
 Author      : Camila Palomino
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "cpu_2.h"

t_log* log_cpu;
t_log* mostrar_log_cpu;
//	char *ruta_log = strdup("/home/utnso/CPUlog");
//-c ${workspace:/cpu/src/cpu.cfg}

void verificar_senial (int num){
	signal (SIGINT, SIG_DFL);
	if (num == SIGSEGV) {
		signal(SIGSEGV, SIG_IGN);
	};
	log_error(log_cpu,"No se entiende señal\t\t Intentar otra vez\n");
}
void esperarSenial(int seg){

	if (signal(SIGINT, verificar_senial) == SIG_ERR) {
		perror("No se puede cambiar signal");
	}
}
void finalizar_por_senial(int sig){

	signal(sig, SIG_IGN);
	log_error(log_cpu,"%s Abortando CPU \n",FAILURE_BULLET);
	morir();

}


void ejecutar() {
	//int i = 0;
	pc = 0;

	log_info(mostrar_log_cpu, "comenzando a ejecutar");

	int tiempo = config_cpu->retardo_cpu;
	float tempo = (tiempo / 100);
	log_info(mostrar_log_cpu, "Retardo de Ejecución: %d", tiempo);

	recibirDTB();


	while (imRunning) {
		sleep(tempo);

		switch (verificarFlag(dtb)) {

		case DUMMY_EJECUTAR:
			dummy();
			break;
		case EJECUTAR_LINEA:
			ejecutarSentencias(tiempo);
			break;
		default: {
			log_error(mostrar_log_cpu, "Flag DTb inválido");
		};
		};
		recibirDTB();
	}

}

void dummy() {
	log_debug(mostrar_log_cpu, "Ejecutar Operación Dummy");

	dummyDAM();
	dummySAFA();

	log_trace(mostrar_log_cpu, "Operación dummy terminada %s", SUCCESS_BULLET);

}
;
void verificarEnvio(int num) {
	if (num == FAIL) {
		log_error(mostrar_log_cpu, "Error al realizar el envio %s",
				FAILURE_BULLET);
	} else {
		log_info(mostrar_log_cpu, "\t Envio correcto %s", SUCCESS_BULLET);
	}
}
void morir(){

	destuirConfiguracion();
	log_error(log_cpu, "Fin Configuración %s\n", SUCCESS_BULLET);

	free(ultima_linea);
	ultima_linea = NULL;
	log_info(mostrar_log_cpu, "se elimino configuracion %s\n", SUCCESS_BULLET);
	destruirDTBCPU();
	//free(mensaje);
	log_debug(log_cpu, "Se libera DTB %s", SUCCESS_BULLET);
	//log_error(mostrar_log_cpu, "se aborta cpu por algún error");

	borrarPruebas();
	log_debug(mostrar_log_cpu, "Liberar recursos %s", SUCCESS_BULLET);
	log_destroy(log_cpu);
	log_destroy(mostrar_log_cpu);
	//list_destroy(prueba);
	printf("\t\t FIN\n");
	exit(EXIT_SUCCESS);
}

int verificarFlag(DTB* dbt) {

	int flag;
	flag = dbt->flag;
	//log_trace(mostrar_log_cpu, "Flag %d", flag);
	if (flag < 0 || flag > 1) {
		log_error(log_cpu, "El flag no se encuentra inicializado %d",
				ERROR_OPERACION);
		return (ERROR_OPERACION);
	} else if (flag == 1) {

		//	log_info(mostrar_log_cpu, "empezar a ejecutar sentencias");
		return (EJECUTAR_LINEA);
	} else if (flag == 0) {
		return (DUMMY_EJECUTAR);		//(DUMMY_EJECUTAR);
	}
	log_error(mostrar_log_cpu, "Error en Flag %d", flag);
	return 0;
}


bool verificarGDT(char* archivo) {
	t_list* buscar = dtb->archivos_abiertos;
	bool existe;
	bool _comparar(void* algo) {
		return string_equals_ignore_case((char*) algo, archivo);
	}
	;

	existe = list_any_satisfy(buscar, (void*) _comparar);
	if (existe) {
		log_warning(mostrar_log_cpu, "Busqueda: %d", OPERACION_VALIDA);
		list_destroy(buscar);
		//return (existe);
	} else {
		log_warning(mostrar_log_cpu, "Busqueda: %d", ERROR_OPERACION);
		list_destroy(buscar);
		//return (existe);
	}
	existe = true;
	return (existe);
}



bool escriptorioVacio(char* line) {
	//line = NULL;
	t_cpu_operacion op;
	op = parse(line);



	if (!op.valido) {
		log_warning(log_cpu, "Fin De Escriptorio.");

		destruir_operacion(op);
		return (true);
	} else {

		destruir_operacion(op);
		return (false);
	};
}

bool codigoBueno(int codigo_operacion){


	switch (codigo_operacion) {
	case abrir:
		return (codigo_operacion == abrir_file);
		break;
	case flush:
		return (codigo_operacion == flush_file);
		break;
	case concentrar:
		return (codigo_operacion == concentrar_file);
		break;
	case asignar:
		return (codigo_operacion == asignar_file);
		break;
	case signal_file:
		return (codigo_operacion == signal_file);
		break;
	case wait_file:
		return (codigo_operacion == wait_file);
		break;
	case close_file:
		return (codigo_operacion == close_file);
		break;
	case borrar:
		return (codigo_operacion == borrar_file);
		break;
	case crear:
		return (codigo_operacion == crear_file);
		break;
	default:
		log_error(mostrar_log_cpu, "Línea no válidad");
		return (false);
		break;
	}
}

void ejecutarSentencias(int tiempo) {
	bool ejecutar = true;
	char* linea = NULL;
	imRunning = true;
	int i = 0;
	int unaLinea = 1;
	int j = 1;

	pc++;

	dtb->pc = pc;
	printf("Contador del programa:%d\n", dtb->pc);
	log_info(log_cpu, "Contador del programa:%d", dtb->pc);
	char* line = NULL;
	line = armarMensaje_Solicitar(PEDIR_LINEA, dtb->id, dtb->posMemoria,
			unaLinea);
	log_warning(mostrar_log_cpu, "mensaje: %s", line);
	enviar(socket_fm9, line);
	line = recibir(socket_fm9, TAM_RECIBIR);
	while (ejecutar) {

		if (escriptorioVacio(line)) {
			log_warning(mostrar_log_cpu, "Fin Escriptorio.");
			ejecutar = false;
			responderSafa(FIN_ARCHIVO);
		} else {
			log_debug(mostrar_log_cpu, "Ejecutando Sentencia %d.", unaLinea);
			//ultima_linea = realloc(ultima_linea, sizeof(line));
			ultima_linea = string_duplicate(line);

			while (i <= quantum && imRunning) {
				j = quantum - i;
				log_debug(mostrar_log_cpu, "Quantum: %d", j);
				usleep(tiempo);

				int op = extraer_mensaje_cpu(line);
				/*  TODO validar msj de FM9
		 char* mensaje_fm9 = recibir(socket_fm9);
		 validarMsjFM9(socket_fm9);*/

				if (op == EXITO_OPERACION) {
					i++;
				} else {
					i++;
					imRunning = false;
				}
				pc++;
				dtb->pc = pc;
				printf("Contador del programa:%d\n", dtb->pc);
				log_info(log_cpu, "Contador del progama: %d", dtb->pc);

				unaLinea++;

				if (string_is_empty(line)) {
					log_error(mostrar_log_cpu, "Script FIN.");
					imRunning = false;
					//morir();
				} else {
					ultima_linea = realloc(ultima_linea, sizeof(line));
					ultima_linea = string_duplicate(line);
				}

				line = armarMensaje_Solicitar(PEDIR_LINEA, dtb->id,
						dtb->posMemoria, unaLinea);
				enviar(socket_fm9, line);
				line = recibir(socket_fm9, TAM_RECIBIR);
			}

			if (j == 0) {
				log_debug(mostrar_log_cpu, "Fin de Quantum");
				responderSafa(FIN_QUANTUM);
				ejecutar = false;
			};
		};
	};
	free(linea);
}


int extraer_mensaje_cpu(char* linea) {
	t_cpu_operacion operacion;
	//operacion = malloc(sizeof(t_cpu_operacion));


	//bool imRunning2 = true;

	//log_trace(mostrar_log_cpu, "abrir Escriptorio");
	log_trace(log_cpu, "leer linea");

	//ultima_linea = realloc(ultima_linea, sizeof(linea));
	ultima_linea = string_duplicate(linea);


	log_debug(mostrar_log_cpu, "CPU:%d. Línea LEIDA:%s", ID, ultima_linea);
	operacion = parse(ultima_linea);

	if (!operacion.valido) {
		char* error;
		error = string_from_format("La línea: %s .No es válida", ultima_linea);
		log_error(log_cpu, error); // al loguear la linea uno ve el porque de invalidez :3
		free(error);
		destruir_operacion(operacion);
		return (OPERACION_CPU_INVALIDA);
	} else if (codigoBueno(operacion.keyword)) {

		ejecutarMensaje(operacion, ultima_linea);
		destruir_operacion(operacion);
		return (EXITO_OPERACION);
	} else {
		char* error;
		error = string_from_format("La línea: %s .No es válida", ultima_linea);
		log_error(log_cpu, error); // al loguear la linea uno ve el porque de invalidez :3
		free(error);
		destruir_operacion(operacion);
		return (OPERACION_CPU_INVALIDA);
	}
	//return (OPERACION);
	//imRunning = false;
	//free(ultima_linea);
	//free(linea);
	//free(paquete);
	//free(paquete);
	//free(linea);
	//free(ultima_linea);

}
int ejecutarMensaje(t_cpu_operacion operacion, char* linea) {

	log_trace(mostrar_log_cpu, "Ejecutar DTB");

	if (!operacion.valido) {
		char* error;
		error = string_from_format("La línea: %s .No es válida", ultima_linea);
		log_error(log_cpu, "operacion invalida %d", OPERACION_INVALIDA);
		log_error(mostrar_log_cpu, error);
		free(error);
		return (OPERACION_CPU_INVALIDA);
	} else {

		switch (operacion.keyword) {
		case abrir_file:
			abrirArchivo(operacion);
			/*Verificar si dicho archivo ya se encuentra abierto por el G.DT.
			 * Para esto se debe validar en las estructuras administrativas del DTB.
			 * En caso que ya se encuentre abierto, se procederá con la siguiente primitiva.EXITO_OPERACION
			 *
			 * */
			break;
		case concentrar_file:
			log_trace(mostrar_log_cpu, "Concentrando...");
			break;
		case asignar_file:
			asignarArchivo(operacion);
			break;
		case wait_file:
			waitArchivo(operacion);
			break;
		case senial:
			signalArchivo(operacion);
			break;
		case flush_file:
			flushArchivo(operacion);
			break;
		case close_file:
			cerrarArchivo(operacion);
			break;
		case crear_file:
			crearArchivo(operacion);
			break;
		case borrar_file:
			borrarFile(operacion);
			break;
		default:
			log_error(log_cpu, "operacion invalida %d\n", OPERACION_INVALIDA);
			return (OPERACION_CPU_INVALIDA);
		}

	}
	return (OPERACION_CPU_VALIDA);
}
