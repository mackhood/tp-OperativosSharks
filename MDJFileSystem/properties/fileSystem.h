 /*============================================================================
 Name        : fileSystem
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


/* =================================================================
				Type: INCLUDE
 ================================================================= */
#include "bitmap.h"
#include "configurar.h"
//#include <sharksCommons/protocoloMdjDAM.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <sys/mman.h>
#include <commons/bitarray.h>
#include <commons/log.h>
#include <sharksCommons/protocolo.h>

/* =================================================================
 Type: DIFINE
 ================================================================= */
#define METAD 		"Metadata/"
#define BIN_METAD	"Metadata.bin"
#define BIN_BIT		"Metadata/Bitmap.bin"
#define BLOQUE 		"Bloques/"
#define ARCH 		"Archivos/"
#define COD_BLOQUE	88800
#define COD_ARCH	99900
#define TODO_OK_BULLET	 	" ♥ "
#define TREBOL_BULLET 		" ♣ "
//#define FAILURE_BULLET      " ✖ "
//#define SUCCESS_BULLET      " ✔ "
//#define PENDING_BULLET      " • "
#define MAGIC_NUMBER	"FIFA\0"
#define SIZE_B		"TAMANIO_BLOQUES"
#define CANTIDAD	"CANTIDAD_BLOQUES"
#define ERROR_MDJ		-1

/* =================================================================
				Type: ESTRUCTURAS
 ================================================================= */

t_bitarray* bitarray;

/* =================================================================
 Type: VARIABLES
 ================================================================= */

//t_log* fifa_log;
//t_list* tabla;
//t_config_mdj* config_mdj;
t_config* archconf;
struct stat myStruct;
void* bitmap;
char* posicion;
int tBloques;
int bit;
int socket_mdj;
int fd_dam;
int enviarM;
int tamanioBitmap;
int cant_Bloques;
/* =================================================================
 MDJFileSystem
 ================================================================= */
void alogFS();
void dest_logFS();
void finLogs();
int pathRaiz(char* path);
char* crearPathArchivo(char* path);
char* dividirPath(char* mensaje);
int cambioDePermisos(char* path, char* archivo);
void crear_directorio(char* path);
int readMetadata();
int verificar_envio(int enviar);
int validarApertura(char* path);
char* armar_pathBloqueNuevo(int bloqueSig);
int agregar_bloque(int bit);
void modificar_archivo(char* path, int tamanio, char* bloques);
char* crear_string_bloques(char ** bloques, char * bloques_nuevos);
//t_archivo* recorrer_archivo(char* path);
//t_path* recuperar_datos(char* codigo, char* mensaje);
//char* armar_pathBloque(char *path, int bloqueSig, t_archivo *archivo);
int verificar_bloque();
void armar_archivo(int archivo);
void archivosTabla(char* ruta, int i, int size_bloques, char* ruta2);
/* =================================================================
 Type: FUNCIONES
 ================================================================= */
void validar_archivo(char* path);
int crear_archivo_mdj(char* path, int bytes);
void borrar_archivo(char *mensaje);
void datos(char *path, int offset, int size);
void guardarDatos(char *path, int offset, int size, char *buffer);
void* respuestaMDJ(int clave);
void responderDAM(int clave);
