/*
 * consola.c
 *
 *  Created on: 8 sep. 2018
 *      Author: utnso
 */

#include "consola.h"


COMANDO comandos[] = {
		{"Ejecutar",ejecutarScript},
		{"Status",statusColas},
		{"Finalizar",finalizarDTB},
		{"Metricas",metricaDTB},
		{(char *) NULL, (Function *) NULL}
}; // para generalizar las funciones reciben un string.




void handleConsolaGDT(t_safa* tSafa){


	puts("-_____________________________________________________");
	puts("CONSOLA");
	puts("------ Escriba un comando ------");
	puts("1. - Ejecutar  <ruta-al-escritorio>");
	puts("2. - Status    <id_dtb>");
	puts("3. - Finalizar <id_dtb>");
	puts("4. - Metricas  <id_dtb>");
	char* linea;
//ejecutar prueba.txt
	while (1) {
		linea = readline("\nSharks: ");

		if (strcmp(linea, "exit")==0){
			free(linea);
			puts("EXIT.");
			break;
		}

		if (ejecutar_linea(linea)){
			add_history(linea);
		}
		free(linea);
	}
}

COMANDO * buscar_comando(char* linea) {
	int i;

	for (i = 0; comandos[i].nombre; i++){
		// Paso a miniscula y comparo
		string_to_lower(linea);
		char* comando = string_duplicate(comandos[i].nombre);
		string_to_lower(comando);

		if (strcmp (linea, comando) == 0){
			free(comando);
			return (&comandos[i]);
		}
		free(comando);

	}
	return ((COMANDO *) NULL);
}

int ejecutar_linea (char * linea){
	char * linea_aux = string_duplicate(linea);
	int i = 0;
	char * funcion;

	if(string_contains(linea, " "))
	{
			while (linea_aux[i] != ' ') i++;

			funcion = malloc((sizeof(char) * i) + 1);
			strncpy(funcion, linea_aux, i);
			funcion[i] = '\0';
	}else{
			funcion = string_duplicate(linea_aux);
	}

	COMANDO * comando = buscar_comando(funcion);

	char** args = string_split(linea_aux, " ");
	if (!comando) {
		printf ("%s: el comando ingresado es incorrecto.", funcion);
		return (-1);
	}

	free(funcion);
	free(linea_aux);
	//Llamo a la función
	(*(comando->funcion)) (args);
	free(args[0]);
	free(args[1]);

	free(args);
	return 1;
}


void ejecutarScript(char** args){

	if(args[1] !=NULL){
		char* parametro = malloc( (sizeof(char)*string_length(args[1])) + 1);
		memcpy(parametro, args[1], string_length(args[1]) + 1);
		logInfo("Operacion ejecutar %s", parametro);
		//char* arg = string_duplicate(parametro);
		initNuevoGDT(parametro);
		free(parametro);
	} else {
		puts("Debe ingresar un parametro para la funcion Ejecutar");
	}
}

void statusColas(char** arg){
	char * parametro = arg[1];
	if(parametro==NULL) {
		mostrarStatusTodo();
	} else {
		mostrarStatus(atoi(parametro));
	}
	logInfo("Operacion status %s", parametro);
}

void finalizarDTB(char** arg){
	int pid = atoi(arg[1]);
	logInfo("Operacion finalizar %d", pid);
	finalizarById(pid);
}

void metricaDTB(char** arg){
	logInfo("Operacion opcional metrica %s", arg[1]);

	char* parametro = arg[1];
	if(parametro==NULL)
		mostrarMetricaTodo();
	else
		mostrarMetrica( atoi(parametro) );
}






