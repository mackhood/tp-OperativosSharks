 /*============================================================================
 Name        : fileSystem.c
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/

/* =================================================================
				Type: INCLUDE
 =================================================================*/
#include "fileSystem.h"
#include "configurar.h"
#include "bitmap.h"
//#include <sharksCommons/protocoloMdjDAM.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/serealizacion.h>
#include <commons/string.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <bits/mman.h>
#include <bits/mman-linux.h>
#include <commons/bitarray.h>
/* =================================================================
 Type: VARIABLES
 =================================================================*/
//faccessat (int __fd, const char *__file, int __type, int __flag)
/* =================================================================
				    MDJFileSystem
 =================================================================*/
void alogFS() {
	fifa_log = log_create("fifa.log", " ♣ FIFA ♣ ", true,
			LOG_LEVEL_TRACE);
}
void finLogs() {
	log_destroy(fifa_log);
}
void dest_logFS() {
	finLogs();
	//log_destroy(fifa_log);
	log_destroy(internal_log);
}
int pathRaiz(char* path) {
	int raiz;



	raiz = chdir(path);

	if (raiz != 0) {
		log_error(fifa_log, "Raiz %s", FAILURE_BULLET);
		return (EXIT_FAILURE);
	} else {

		//log_debug(fifa_log, "Raiz: %s", SUCCESS_BULLET);
	};


	return (EXIT_SUCCESS);
}
char* crearPathArchivo(char* path) {
	char* pathArchivo;
	pathArchivo = string_new();
	string_append(&pathArchivo, config_mdj->punto_de_montaje);

	if (strstr(path, ARCH) == NULL) {
		string_append(&pathArchivo, ARCH);
	}
	if (string_starts_with(path, "/")) {
		path = string_substring_from(path, 1);
	}

	string_append(&pathArchivo, path);
	dir = string_duplicate(pathArchivo);
	log_debug(fifa_log, "Path de archivo: %s %s", dir, SUCCESS_BULLET);


	//list_add(tabla, pathArchivo);
	free(pathArchivo);
	return (dir);
}
char* dividirPath(char* mensaje) {
	char * path = string_new();
	char ** split;
	int i = 0;
	int j = 0;
	int k = 0;
	split = string_split(mensaje, "/");

	while (split[i] != NULL/*'\0'*/) {
		log_info(internal_log, "Path: %s,", split[i]);
		i++;
	};
	i -= 2;
	while (j <= i) {
		string_append(&path, "/");
		string_append(&path, split[j]);
		j++;
	}

	log_info(internal_log, "Path: %s", path);
	while (split[k] != NULL) {
		free(split[k]);
		k++;
	}
	free(split);

	return path;
}
int cambioDePermisos(char* path, char* archivo) {

	char mode[] = "0766";
	char *buf = string_new();
	string_append(&buf, path);
	int j = -2;
	j = chdir(buf);
	int i;
	i = strtol(mode, 0, 8);

	if (j == -1) {
		log_error(fifa_log, "%s: No se pudo acceder", strerror(errno));

		free(buf);
		return (EXIT_FAILURE);
	} else if (archivo != NULL) {
		string_append(&buf, archivo);
		j = chmod(archivo, i);
		log_info(fifa_log, "Acceso al directorio del %s %s", archivo,
				SUCCESS_BULLET);
	} else {
		log_info(fifa_log, "Acceso al directorio: %s", SUCCESS_BULLET);
		j = chmod(buf, i);
	};

	if (j != 0) {
		log_error(fifa_log, "%s: No se pudieron cambiar los permisos",
				strerror(errno));
		free(buf);

		return (EXIT_FAILURE);
		//exit(1);
	} else {
		free(buf);

		return EXIT_SUCCESS;
	}

}
void crear_directorio(char* path) {
	char* pathmkdir = string_from_format(" mkdir -p %s", path);
	system(pathmkdir);

	cambioDePermisos(path, NULL);
	free(pathmkdir);
}
int readMetadata() {

	t_config *configuracion;
	char* montaje = string_duplicate(config_mdj->punto_de_montaje);
//	char* ruta = strdup(config_mdj->punto_de_montaje);
	int sizeM = strlen_null(montaje) + strlen_null(METAD)
			+ strlen_null(BIN_METAD);
	char* rutaArc = malloc(sizeof(char) * sizeM);
	//char *rutaArc1 = string_new();
	//string_append(&rutaArc1,montaje);
	//string_append(&rutaArc1, ARCH);


	//t_archivos->nombre = strdup(rutaArc);
	//t_archivos->info.hard_link = atoi(rutaArc1);
	//t_archivos->info.num_inodo = 0;
	//list_add_in_index(tabla, t_archivos->info.num_inodo, (t_archivo*) t_archivos);

	//realloc(rutaArc, sizeof(char) * strlen(ruta));
	//char* mont = string_duplicate(config_mdj->punto_de_montaje);
	//rutaArc = string_new();

	//string_append(&rutaArc, montaje);
	string_append(&montaje, METAD);
	string_append(&montaje, BIN_METAD);
	configuracion = config_create(montaje);
//	configuracion = config_create("/home/fifa-examples/fifa-entrega/Metadata/Metadata.bin");


	metadata->c_bloques = config_get_int_value(configuracion, "CANTIDAD_BLOQUES");
	log_trace(fifa_log, "Cantidad de Bloques: %d", metadata->c_bloques);
	metadata->size_bloques = config_get_int_value(configuracion,
			"TAMANIO_BLOQUES");
	log_trace(fifa_log, "Tamanio Bloques: %d", metadata->size_bloques);

	metadata->magic = string_duplicate(MAGIC_NUMBER);
	log_trace(fifa_log, "Magic Number: %s", metadata->magic);



	t_archivos->nombre = string_duplicate("Metadata.bin");
	t_archivos->info.hard_link = atoi(rutaArc);
	t_archivos->info.num_inodo = 1;
	t_archivos->info.bloque_tamanio = metadata->size_bloques;
	t_archivos->info.bloque_numero = metadata->c_bloques;


	/*list_add_in_index(tabla, t_archivos->info.num_inodo,
			(t_archivo*) t_archivos);

	 //log_debug(inter, "%s", t_archivos->info.hard_link);*/

	//free(ruta);
	free(rutaArc);
	//free(rutaArc1);
	//rutaArc = NULL;
	rutaArc	 = NULL;
	//ruta = NULL;

	free(montaje);
	montaje = NULL;
	config_destroy(configuracion);
	return 0;
}
void archivosTabla(char* ruta, int i, int size_bloques, char* ruta2) {

	t_archivos->nombre = ruta2;
	t_archivos->info.hard_link = atoi(ruta);
	t_archivos->info.num_inodo = i;

	//t_archivos->info.bloque_numero = metadata->c_bloques;

	//log_debug(fifa_log, "%s", t_archivos->nombre);
	list_add_in_index(tabla, t_archivos->info.num_inodo,
			(t_archivo*) t_archivos);
}


int leerBitmap() {
	/*

	char* ruta2 = string_duplicate(BIN_BIT);
	char* ruta = string_duplicate(config_mdj->punto_de_montaje);

	string_append(&ruta, ruta2);

	int bitmap = open(ruta, O_RDWR);

	free(ruta);
	ruta = NULL;
	free(ruta2);
	ruta2 = NULL;

	if (bitmap == 0) {
		log_error(fifa_log, "no abrio el bitmap");
		close(bitmap);
		return (ERROR_MDJ);
	}

	int f = fstat(bitmap, &myStruct);

	if (f != 0) {
		log_error(fifa_log, "no abrio el bitmap");
		return (ERROR_MDJ);
	} else {

	}

	posicion = mmap(0, myStruct.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,
			bitmap, 0);

	t_bitarray* bita = bitarray_create_with_mode(posicion, myStruct.st_size,
			LSB_FIRST);

	 char* posicion = mmap(0, myStruct.st_size, PROT_READ | PROT_WRITE,
	 MAP_SHARED, bitmap, 0);

	 memoria_mapeada = mmap(posicion, metadata->c_bloques,
	PROT_READ | PROT_WRITE,
	 MAP_SHARED, bitmap, 0);

	 memoria_mapeada = mmap(0, myStruct.st_size, PROT_WRITE | PROT_READ,
	 MAP_SHARED, bitmap, 0);
	//memcpy(memoria_mapeada, bitmap, myStruct.st_size);
	//memcpy(memoria_mapeada, posicion, myStruct.st_size);
	//msync(posicion, myStruct.st_size, MS_SYNC);
	 = bitarray_create_with_mode(memoria_mapeada,
	 metadata->c_bloques, LSB_FIRST);


	if (posicion == MAP_FAILED) {
		log_error(fifa_log, "error en mmap");
		char* error = string_itoa(strerror(errno));
		fprintf(stderr, "mmap failed %d: %s\n", errno, error);
		close(bitmap);
		free(error);
		error = NULL;
		return (ERROR_MDJ);
	}

	log_debug(fifa_log, "%d", myStruct.st_blocks);
	log_debug(fifa_log, "%d", myStruct.st_size);

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
			printf("BIT %d: %d|", bit, lar);
			log_info(internal_log, "Bitarray:%d-> %d", bit, lar);
		} else {
			printf("BIT %d: %d|", bit, j);
			log_info(internal_log, "Bitarray:%d-> %d", bit, j);
		};
		bit++;
	};
	 */


	return (EXIT_SUCCESS);

}




int verificar_envio(int enviar) {
	if (enviar != -1) {
		log_info(fifa_log, "Envio Exitoso");
		return (EXIT_SUCCESS);
	}
	log_error(fifa_log, "Error al enviar info");
	return (EXIT_FAILURE);
}

/* =================================================================
 Secundarias
 =================================================================*/
void* responderMDJ(int clave) {
	E_MDJ* e_mdj = calloc(1, sizeof(E_MDJ));
	char* path = string_new();
	char* buffer = string_new();

	e_mdj->cod = 0;
	e_mdj->base = 0;
	e_mdj->offset = 0;
	e_mdj->clave = clave;
	e_mdj->tamArchivo = 0;
	e_mdj->lineas = 0;
	e_mdj->path = path;
	e_mdj->cabecera = CABECERA_MDJ;
	e_mdj->buffer = buffer;
	memcpy(e_mdj->path, path, strlen_null_mdj(path));
	memcpy(e_mdj->cabecera, CABECERA_MDJ, strlen_null_mdj(CABECERA_MDJ));
	memcpy(e_mdj->buffer, buffer, strlen_null_mdj(buffer));

	free(path);
	path = NULL;
	free(buffer);
	buffer = NULL;
	return (e_mdj);
}
void armar_archivo(int archivo) {
	char* tamanio = "TAMANIO=";
	char* bloques = string_new();
	int bit;
	int bit2;
	char* var;
	bit = verificar_bloque();
	if (bit != -1) {
		bit2 = agregar_bloque(bit);
	}

	if (bit != -1 && bit2 != -1) {
		var = string_itoa(bit);
		string_append(&bloques, "BLOQUES=[");
		string_append(&bloques, var);
		string_append(&bloques, "]");

		int i = write(archivo, tamanio,
				(sizeof(char)) * (strlen_null(tamanio)));
		free(tamanio);
		tamanio = string_new();
		string_append(&tamanio, string_itoa(i));
		string_append(&tamanio, "\n");
		write(archivo, tamanio, (sizeof(char)) * (strlen(tamanio)));
		write(archivo, bloques, (sizeof(char)) * (strlen(bloques)));
		//fwrite(tamanio,sizeof(char),string_length(tamanio),archivo);
		//fwrite(bloques,sizeof(char),string_length(bloques),archivo);
		free(var);
		log_info(fifa_log, "%s", tamanio);
		log_info(fifa_log, "%s", bloques);
	}
	free(tamanio);
	free(bloques);
}

int validarApertura(char* path) {
	int fp;
	char* pathArmado = crearPathArchivo(path);

	fp = open(pathArmado, O_RDONLY);

	if (fp == -1) {
		log_error(fifa_log, "%d:  No se pudo abrir el Escriptorio. %s",
		PATH_INEXISTENTE, FAILURE_BULLET);
		free(pathArmado);
		return (PATH_INEXISTENTE);
	} else {
		log_info(fifa_log, "Path existe %s", SUCCESS_BULLET);
		responderDAM(OPERACION_MDJ_VALIDA);

		free(pathArmado);
		return (OPERACION_MDJ_VALIDA);
	}
}
void responderDAM(int clave) {
	char* msj;
	E_MDJ* respuesta;
	respuesta = responderMDJ(clave);
	msj = serializar_E_MDJ(respuesta);

	enviarM = enviar(fd_dam, msj);
	verificar_envio(enviarM);
	free(msj);
	destruir_E_MDJ(respuesta);

}
t_archivo* recorrer_archivo(char* path) {

	t_config *config = config_create(path);
	//archivo = malloc(sizeof(t_archivo));
	t_archivos->info.tamanio_bytes = config_get_int_value(config, "TAMANIO");
	t_archivos->bloques = config_get_array_value(config, "BLOQUES");
	t_archivos->info.hard_link = atoi(path);
	log_info(fifa_log, "Tamanio: %d", t_archivos->info.tamanio_bytes);
	log_info(fifa_log, "Bloques: ", *(t_archivos->bloques));
	config_destroy(config);
	return t_archivos;
}
char* armar_pathBloque(char *path, int bloqueSig, t_archivo *archivo) {
	char *path2 = string_new();
	string_append(&path2, path);
	string_append(&path2, archivo->bloques[bloqueSig]);
	string_append(&path2, ".bin");
	log_info(fifa_log, "Path del bloque: %s\n", path2);
	return path2;
}
t_path* recuperar_datos(char* path, char* mensaje) {
	t_path* estructura = malloc(sizeof(t_path));
	/*int size;
	int start;
	char* var;

	 var = string_duplicate(mensaje);
	size = atoi(var);
	 estructura->path = string_duplicate(path);
	log_info(fifa_log, "Path del archivo: %s", estructura->path);
	free(var);

	start = 4 + size;

	var = string_substring(mensaje, start, 4);
	size = atoi(var);
	start += 4;
	free(var);
	var = string_substring(mensaje, start, size);
	estructura->offset = atoi(var);
	log_info(fifa_log, "offset: %d", estructura->offset);
	free(var);

	start += size;

	var = string_substring(mensaje, start, 4);
	size = atoi(var);
	start += 4;
	free(var);
	var = string_substring(mensaje, start, size);
	estructura->size = atoi(var);
	log_info(fifa_log, "size: %d", estructura->size);
	free(var);

	start += size;

	if (atoi(codigo) == 15) {
		var = string_substring(mensaje, start, 4);
		size = atoi(var);
		start += 4;
		free(var);
		estructura->buffer = string_substring(mensaje, start, size);
		log_info(fifa_log, "Buffer: %s\n", estructura->buffer);
	} else {
		estructura->buffer = NULL;
	}
	 */
	return estructura;
}
int verificar_bloque() {
	int bit = -1;
	int prueba = -1;
	bool comprobar = true;
	cant_Bloques = metadata->c_bloques;
	log_debug(internal_log, "bloques: %d", cant_Bloques);
	while (bit < cant_Bloques && comprobar == true) {
		bit++;
		comprobar = bitarray_test_bit(bitarray, bit);
		log_debug(fifa_log, "bit: %d", bit);
	}
	if (bit == cant_Bloques && comprobar == false) {
		return prueba;
	} else {
		return bit;
	}
}
int agregar_bloque(int bit) {

	if (bit == -1) {
		return -1;
	} else {
		bitarray_set_bit(bitarray, bit);
		return bit;
	}
}
char* armar_pathBloqueNuevo(int bloqueSig) {

	char *path2 = string_new();
	char *var = string_itoa(bloqueSig);
	string_append(&path2, config_mdj->punto_de_montaje);
	string_append(&path2, BLOQUE);
	string_append(&path2, var);
	string_append(&path2, ".bin");
	log_info(log_mdj, "Path del bloque: %s\n", path2);
	free(var);
	return path2;
}
char* crear_string_bloques(char ** bloques, char * bloques_nuevos) {
	char * bloque = strdup("");
	int i = 0;
	int j = 0;

	string_append(&bloque, "[");

	while (bloques[j] != NULL /*'\0'*/)
		j++;

	j -= 2;

	while (i <= j) {
		string_append(&bloque, bloques[i]);
		string_append(&bloque, ",");
		i++;
	}
	string_append(&bloque, bloques[i]);

	if (!strcmp(bloques_nuevos, "")) {
		string_append(&bloque, "]");
	} else {
		string_append(&bloque, bloques_nuevos);
		string_append(&bloque, "]");
	}

	//log_info(log_mdj,_compuesto("Bloques final: ",bloque);

	return bloque;
}
void modificar_archivo(char* path, int tamanio, char* bloques) {
	char* tamanio2 = string_itoa(tamanio);


	//archconf = config_create(path);

	config_set_value(archconf, "TAMANIO", tamanio2);
	config_set_value(archconf, "BLOQUES", bloques);

	log_info(log_mdj, "Tamanio: %d\n", tamanio);
	log_info(log_mdj, "Bloque: %s\n", bloques);

	config_save_in_file(archconf, path);

	config_destroy(archconf);
	free(tamanio2);
}


/* =================================================================
 MDJFileSystem
 =================================================================*/
void abrir_archivo(char* path) {
	char* pathArmado;
	int archivo;
	pathArmado = crearPathArchivo(path);

	if (validarApertura(path) == PATH_INEXISTENTE) {
		responderDAM(ARCH_NO_ABIERTO);
	} else {

		archivo = open(pathArmado, O_RDONLY);
		responderDAM(OPERACION_MDJ_VALIDA);
		log_debug(fifa_log, "Archivo abierto %s", SUCCESS_BULLET);
	}
	free(pathArmado);

}
void datos(char *path, int offset, int size) {
		char *mensaje; // para enviar los datos
	char *path2 = string_new(); //para sacar cada path de bloques
		char *pathBloque; // para guardar los path hechos
		FILE *bloques; //para abrir cada archivo de bloques
		char *lectura = strdup(""); // para guardar lo que se lee
		char *lectura2 = malloc(64);
		int restoSize; // lo que falta leer
		t_archivo *archivo; //guarda la info del archivo en gral
		div_t bloque; //guarda los datos de la division para sacar los bloques y el offset
		int bloqueSig; // guarda el bloque al que hay que ir
		char *pathArmado;
		char * read;

		string_append(&path2, config_mdj->punto_de_montaje);
		string_append(&path2, BLOQUE);

		pathArmado = crearPathArchivo(path);

		archivo = recorrer_archivo(pathArmado);
		bloque = div(offset, tBloques);
		bloqueSig = bloque.quot;

		pathBloque = armar_pathBloque(path2, bloqueSig, archivo);

		bloques = fopen(pathBloque, "r");
		free(pathBloque);
		fseek(bloques, bloque.rem, SEEK_SET);
		restoSize = size;

		while (string_length(lectura) < size) {
			if (restoSize <= metadata->c_bloques - bloque.rem) {
				fread(lectura2, sizeof(char), restoSize, bloques);
				read = string_substring(lectura2, 0, restoSize);
				string_append(&lectura, read);
				restoSize = size;

			} // preguntar estructura de los bloques.bin
			else {


				fread(lectura2, sizeof(char), metadata->c_bloques - bloque.rem,
						bloques);
				read = string_substring(lectura2, 0,
						metadata->c_bloques - bloque.rem);
				string_append(&lectura, read);

				free(read);

				fclose(bloques);

				restoSize = size - string_length(lectura);

				bloqueSig++;
				if (archivo->bloques[bloqueSig] != '\0') {
					pathBloque = armar_pathBloque(path2, bloqueSig, archivo);
					bloques = fopen(pathBloque, "r");
					free(pathBloque);
					bloque.rem = 0;
				} else {
					restoSize = size + 1;
				}
			}
		}

		if (restoSize == size) {
		responderDAM(OPERACION_MDJ_VALIDA);

			string_append(&lectura, "\n");
			log_trace(fifa_log, "Se realizo la lectura: %s", lectura);
		} else {

		responderDAM(SIN_ARCHIVO);
			log_error(fifa_log, "%d: No se pudo realizar la lectura", SIN_ARCHIVO);
		}

		fclose(bloques);
		free(read);
	read = NULL;
		free(mensaje);
	mensaje = NULL;
		free(lectura);
	lectura = NULL;
		free(lectura2);
	lectura2 = NULL;
		int i = 0;
		while (archivo->bloques[i] != NULL) {
			free(archivo->bloques[i]);
			i++;
		}
		free(archivo->bloques);
		free(archivo);
		free(pathArmado);

	}

int crear_archivo_mdj(char* path, int bytes) {
		int verificar = verificar_bloque();
		int map;
	int j = 0;
	int r = 0;
		char* pathArmado = crearPathArchivo(path);
		char* pathAux = dividirPath(pathArmado);

		log_info(fifa_log, "Dir: %s", pathAux);
		log_info(fifa_log, "Arch: %s", pathArmado);

	if (verificar == -1) {
		log_error(log_mdj, "%d: No espacio", SIN_ESPACIO_MDJ);
		responderDAM(SIN_ESPACIO_MDJ);
			free(pathArmado);
			free(pathAux);

			return (EXIT_FAILURE);
		} else {

			crear_directorio(pathAux);
			map = creat(pathArmado, O_RDWR);
		close(map);
		cambioDePermisos(pathAux, pathArmado);

		//map = open(pathArmado, O_RDWR | O_CREAT, S_IRWXU);
		if (map == -1) {
			log_error(fifa_log, "%d: El archivo no se creo.", SIN_ARCHIVO);
			responderDAM(SIN_ARCHIVO);
			free(pathArmado);
			free(pathAux);

			return (EXIT_FAILURE);
		} else {
			t_archivos->nombre = pathArmado;
			agregar_bloque(verificar);
			map = open(pathArmado, O_RDWR);

			armar_archivo(map);
			close(map);
			int tamanio = 0;
			char* bloque = armar_pathBloqueNuevo(verificar);
			map = open(bloque, O_RDWR);
			int i = 0;
			if (bytes < metadata->size_bloques) {
				for (i = 0; i < bytes; i++) {
				lseek(map, 0, SEEK_END);
					tamanio = write(map, "\n", bytes);
				log_info(internal_log, "%d", i);
				i++;
			}
				close(map);
				map = open(pathArmado, O_RDWR);

				armar_archivo(map);
				close(map);
			} else {
				div_t tam = div(bytes, metadata->size_bloques);
				tamanio = bytes;
				for (i = 0; i <= tam.quot; i++) {
						lseek(map, 0, SEEK_END);
					r = write(map, "\n", metadata->size_bloques);
						log_info(internal_log, "%d", tamanio);
						i++;
					j = j + r;
					close(map);
						verificar = verificar_bloque();
						agregar_bloque(verificar);
						bloque = armar_pathBloqueNuevo(verificar);
					//modificar_archivo(path, j, bloque);
						map = open(bloque, O_RDWR);
					tamanio = tamanio - j;
				}
				close(map);
				verificar = verificar_bloque();
				agregar_bloque(verificar);
				bloque = armar_pathBloqueNuevo(verificar);

				map = open(bloque, O_RDWR);
				lseek(map, 0, SEEK_END);
				r = write(map, "\n", tamanio);
				j = j + r;
				//modificar_archivo(path, j, bloque);
				close(map);
				map = open(pathArmado, O_RDWR);

				armar_archivo(map);
				close(map);
				if (j >= bytes) {
					log_info(fifa_log, "El archivo se creó correctamente");
					responderDAM(OPERACION_MDJ_VALIDA);
					free(pathArmado);
					free(pathAux);

					return (EXIT_SUCCESS);
				} else {
					log_error(fifa_log, "Error en el tamaño del Archivo");
					return (EXIT_FAILURE);
				}
					}
				}
			}

}






void guardarDatos(char *path, int offset, int size, char *buffer) {

	char* path2 = string_new(); //para sacar cada path de bloques
	char* pathBloque; // para guardar los path hechos
	FILE* bloques; //para abrir cada archivo de bloques
	//guarda la info del archivo en gral
	div_t bloque; //guarda los datos de la division para sacar los bloques y el offset
	int bloqueSig;
	int bloqueAnterior = t_archivos->bloquesAsignados; // guarda el bloque al que hay que ir
	int guardado = 0;
	char *bloques_agregados = string_new();
	;
	char *pathArmado;
	char *bloques_final;
	//int sizeAguardar = 0;
	int flag = 0;
	//int stop;
	char *write;
	int bit;
	int i = 0;

	string_append(&path2, config_mdj->punto_de_montaje);
	string_append(&path2, BLOQUE);

	pathArmado = crearPathArchivo(path);

	t_archivos = recorrer_archivo(pathArmado);
	while (t_archivos->bloques[i] != NULL)
		i++;
	bloque = div(bloqueAnterior, offset);
	bloqueSig = bloque.quot;

	if (bloqueSig >= i) {
		bloqueSig = verificar_bloque();
		if (bloqueSig != -1) {
			bit = agregar_bloque(bloqueSig);
		}
		if (bloqueSig != -1 && bit != -1) {
			pathBloque = armar_pathBloqueNuevo(bloqueSig);
			bloques = fopen(pathBloque, "r+");
			bloque.rem = 0;

			string_append(&bloques_agregados, ",");
			string_append(&bloques_agregados, string_itoa(bloqueSig));

			if (flag == 0) {
				flag = 1;
				//stop = guardado;
			}

		} else {
			guardado = size + 1;
		}

	} else {
		pathBloque = armar_pathBloque(path2, bloqueSig, t_archivos);

		bloques = fopen(pathBloque, "r+");
		fseek(bloques, bloque.rem, SEEK_SET);
	}

	if (offset < t_archivos->info.tamanio_bytes) {
		while (guardado < size) {
			if ((size - guardado) <= (metadata->c_bloques - bloque.rem)) {
				write = string_substring(buffer, guardado, size - guardado);
				fwrite(write, sizeof(char), string_length(write), bloques);
				//fputs(write,bloques);
				guardado = size;
				fclose(bloques);
				flag = 2;
				free(write);
				/*if(flag == 1){
				 sizeAguardar = guardado - stop;
				 }*/

			} else {
				write = string_substring(buffer, guardado,
						metadata->c_bloques - bloque.rem);
				fwrite(write, sizeof(char), string_length(write), bloques);
				//fputs(write,bloques);
				guardado += metadata->c_bloques - bloque.rem;

				fclose(bloques);

				free(write);

				if (t_archivos->bloques[bloqueSig + 1] != NULL /*'\0'*/) {
					bloqueSig++;
					pathBloque = armar_pathBloque(path2, bloqueSig, t_archivos);
					bloques = fopen(pathBloque, "r+");
					bloque.rem = 0;
					flag = 2;
				} else {
					bloqueSig = verificar_bloque();
					if (bloqueSig != -1) {
						bit = agregar_bloque(bloqueSig);
					}

					if (bloqueSig != -1 && bit != -1) {
						pathBloque = armar_pathBloqueNuevo(bloqueSig);
						bloques = fopen(pathBloque, "r+");
						bloque.rem = 0;

						string_append(&bloques_agregados, ",");
						string_append(&bloques_agregados,
								string_itoa(bloqueSig));

						if ((size - guardado)
								> (metadata->size_bloques - bloque.rem))
						 {
						 string_append(&bloques_agregados,",");
						}

						flag = 2;

					} else {
						guardado = size + 1;

						/*if(flag == 0){
						 guardado = size + 1;
						 }
						 else{
						 sizeAguardar = guardado - stop;
						 guardado = size + 1;
						 }*/
					}
				}
			}
		}
	}

	else {
		while (guardado < size) {
			if ((size - guardado) <= (metadata->c_bloques - bloque.rem)) {
				write = string_substring(buffer, guardado, size - guardado);

				fputs(write, bloques);
				//sizeAguardar += size - guardado;
				guardado = size;
				flag = 1;
				fclose(bloques);
				free(write);

			} // preguntar estructura de los bloques.bin
			else {
				write = string_substring(buffer, guardado,
						metadata->c_bloques - bloque.rem);
				//fwrite(write,sizeof(char),string_length(write),bloques);
				fputs(write, bloques);
				guardado += metadata->c_bloques - bloque.rem;

				fclose(bloques);

				free(write);

				bloqueSig = verificar_bloque();
				if (bloqueSig != -1) {
					bit = agregar_bloque(bloqueSig);
				}

				if (bloqueSig != -1 && bit != -1) {
					pathBloque = armar_pathBloqueNuevo(bloqueSig);
					bloques = fopen(pathBloque, "r+");
					bloque.rem = 0;

					string_append(&bloques_agregados, ",");
					string_append(&bloques_agregados, string_itoa(bloqueSig));

					/*if((size - guardado) > (64-bloque.rem))
					 {
					 string_append(&bloques_agregados,",");
					 }*/

					if (flag == 0) {
						flag = 1;
						//stop = guardado;
					}
				} else {
					guardado = size + 1;
				}
			}
		}
	}

	if (flag == 1) {
		t_archivos->info.tamanio_bytes += size;
		bloques_final = crear_string_bloques(t_archivos->bloques,
				bloques_agregados);
		/*modificar_archivo(pathArmado, t_archivos->info.tamanio_bytes,
		 bloques_final);*/
		free(bloques_final);
	}

	if (flag == 2) {
		t_archivos->info.tamanio_bytes = offset + size;
		bloques_final = crear_string_bloques(t_archivos->bloques,
				bloques_agregados);
		/*modificar_archivo(pathArmado, t_archivos->info.tamanio_bytes,
		 bloques_final);*/
		free(bloques_final);
	}

	if (guardado == size) {
		responderDAM(OPERACION_MDJ_VALIDA);
		log_info(fifa_log, "Se realizaron cambios en el archivo");

	} else {

		responderDAM(SIN_ARCHIVO);
		log_error(fifa_log,
				"%d: No se pudieron realizar cambios en el archivo",
				ERROR_OPERACION);
	}

	free(path2);

	free(pathBloque);
	i = 0;
	while (t_archivos->bloques[i] != NULL) {
		free(t_archivos->bloques[i]);
		i++;
	}
	free(t_archivos->bloques);
	free(t_archivos);
	free(bloques_agregados);
	free(pathArmado);
}

void borrar_archivo(char* path) {
	char *pathArmado;

	int i = 0;

	pathArmado = crearPathArchivo(path);
	t_archivo* archivo;
	mode_t modo = 0766;
	chmod(path, modo);
	archivo = recorrer_archivo(pathArmado);
	while (archivo->bloques[i] != NULL) {
		bitarray_clean_bit(bitarray, atoi(archivo->bloques[i]));
		i++;
	}

	remove(pathArmado);

	log_trace(fifa_log, "Se elimino %s %s", pathArmado, SUCCESS_BULLET);

	free(pathArmado);

	i = 0;
	while (archivo->bloques[i] != NULL) {
		free(archivo->bloques[i]);
		i++;
	}
	path = NULL;
	free(archivo->bloques);
	free(archivo);
}
