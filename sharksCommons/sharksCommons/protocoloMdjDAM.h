
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/string.h>
#include <string.h>
#include "serealizacion.h"

typedef struct{
	char* cabecera;
	int cod;
	int base;
	int offset;
	char* path; 	  // ruta del escriptorio
	char* buffer;
	int clave;
	int lineas;
	int tamArchivo;
} E_MDJ;



/* Serializacion/Desserializacion */

int strlen_null_mdj(char* str);
typedef struct {
	char* clave;
	char* valor;
	char* path;
} t_clavevalor_m;

/*------Mensajes -----*/
typedef struct {
	int id_dbt;
	int keyword;
	t_clavevalor_m clave_valor;
} t_mensaje_cpu_m;

typedef struct{
	char* cpu;
	char* cpu_ahora;  //Me dice en que cpu estaria actualmente
	char* valor;
} t_status_clave_m;



t_clavevalor_m* deserializar_clavevalor_m(void* buffer);
int sizeof_clavevalor_m(t_clavevalor_m cv);
void* serializar_clavevalor_m(t_clavevalor_m clave_valor);
E_MDJ* crearE_MDJ(char* cabecera, int codigo, int base, int offset, char* path, char* buffer,
		int clave, int linea, int tamArchivo);
E_MDJ* deserializar_E_MDJ(char* buffer);
char* serializar_E_MDJ(E_MDJ* object);
void destruir_E_MDJ(E_MDJ* fifa);

