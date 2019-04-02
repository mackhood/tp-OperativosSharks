 /*============================================================================
 Name        : bitmap.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


#ifndef PROPERTIES_BITMAP_H_
#define PROPERTIES_BITMAP_H_
#include <commons/bitarray.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fileSystem.h"
#include "configurar.h"




void liberarBitmap();
void actualizar_bitmap();
int crearBitarray();
t_bitarray* bita;
#endif /* PROPERTIES_BITMAP_H_ */
