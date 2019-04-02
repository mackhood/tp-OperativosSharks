 /*============================================================================
 Name        : bitmap.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fileSystem.h"
#include "configurar.h"
/* =================================================================
				Type: ESTRUCTURAS
 =================================================================*/



/* =================================================================
				    MDJFileSystem
 =================================================================*/
int crearBitarray() {
	char* ruta2 = string_duplicate(BIN_BIT);
	char* ruta = string_duplicate(config_mdj->punto_de_montaje);
	//char* pos[metadata->c_bloques];
	string_append(&ruta, ruta2);

	log_warning(fifa_log, "%s", ruta);
	int bitmap = open(ruta, O_RDONLY);

	if (bitmap == -1) {
		perror("no se abrió\n");
		close(bitmap);
		return (ERROR_MDJ);
	}


	int pagesize;
	char *data;
	int f = fstat(bitmap, &myStruct);

	if (f != 0) {
		log_error(fifa_log, "no copio el bitmap");
		return (ERROR_MDJ);
	};

	pagesize = myStruct.st_size;
	data = mmap(0, myStruct.st_size, PROT_READ, MAP_SHARED, bitmap, 0);

	bita = bitarray_create_with_mode(data, metadata->c_bloques, LSB_FIRST);

	if (data == -1) {
		log_error(fifa_log, "");
		perror("Error en MMAP\n");
		close(bitmap);
		return (ERROR_MDJ);
	}


	int i = 0;
	/*while (read(bitmap, &data, sizeof(t_bitarray))) {
		printf("%-20s, %-10d", data[i], i);
	 }*/

	free(ruta);
	ruta = NULL;
	free(ruta2);
	ruta2 = NULL;







	//log_debug(fifa_log, "Bytes: %d", sizeof(bitarray));
	//log_debug(fifa_log, "Bytes: %d", bitarray_get_max_bit(bitarray));
	int bit = 0;
	int j = 0;
	//int k = -1;
	bool esBit;
	int lar;
	lar = 1;
	while (bit <= metadata->c_bloques) {

		esBit = bitarray_test_bit(bita, bit);
		if (esBit == 1) {
			//printf("BIT %d: %d|", bit, lar);
			log_info(fifa_log, "Bitarray:%d-> %d", bit, lar);
		} else {
			//printf("BIT %d: %d|", bit, j);
			log_info(internal_log, "Bitarray:%d-> %d", bit, j);
		};
		bit++;
	};

	return (EXIT_SUCCESS);

}
void actualizar_bitmap() {

	memcpy(posicion, bita, myStruct.st_size);
	msync(posicion, myStruct.st_size, MS_SYNC);
	log_debug(fifa_log, "actualizando bitmap");
}
void liberarBitmap() {
	int aux, i;
	i = 0;
	aux = bitarray_get_max_bit(bita);
	log_debug(fifa_log, "bytes: %d", aux);
	while (i != aux) {
		bitarray_clean_bit(bita, i);
		i++;
		log_debug(internal_log, "bytes: %d", i);
	}
	bitarray_destroy(bita);
	free(posicion);
	posicion = NULL;
}



