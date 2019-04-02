 /*============================================================================
 Name        : cpu.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*


/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sharksCommons/protocolo.h>
#include <cpu/cpu_2.h>
/* =================================================================
				    cpu
 =================================================================*/
int main(int argc, char *argv[]) {


	signal(SIGINT, verificar_senial);
	signal(SIGINT, finalizar_por_senial);
	leer_config_cpu(argc, argv);


	conectar_DMA();
	conectar_Safa();
	conectar_fm9();

	ejecutar();

	for (int i = 10; i > 0; --i) {
		printf("cuenta regresiva: %d \n", i);
	};
	morir();
	return EXIT_SUCCESS;
}
