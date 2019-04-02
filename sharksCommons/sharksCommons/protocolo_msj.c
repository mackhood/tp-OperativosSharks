#include <string.h>
#include "protocolo_msj.h"
#include <stdlib.h>
#include <unistd.h>




int sizeof_clavevalor (t_clavevalor cv){
	int tamanio_clave = (strlen_null(cv.clave))* sizeof(char);
	int tamanio_valor = (strlen_null(cv.valor)) * sizeof(char);
	int tamanio_path = (strlen_null(cv.path))* sizeof(char);

	int tamanio = sizeof(int) * 3 + tamanio_clave + tamanio_path + tamanio_valor;
	return tamanio;
};

t_clavevalor* deserializar_clavevalor(void* buffer){
	t_clavevalor* auxiliar;
	int* l_clave = buffer;
	int* l_valor = l_clave + 1;
	int* l_path = l_valor + 1;

	char* clave = (char*) (l_valor + 1);
	char* valor = (char*) (l_path + 1);
	char* path;

	if(*l_valor > 0 ){
		valor = clave + *l_clave;
			} else {
				valor = NULL;
			}
	if (*l_path > 0){
		path = valor + *l_valor;
	}else {
		path = NULL;
	};

	auxiliar->clave = clave;
	auxiliar->valor = valor;
	auxiliar->path = path;

	free(buffer);
	return auxiliar;
};

int sizeof_mensaje_cpu(t_mensaje_cpu mensaje_cpu){
  int tamanio;

  return tamanio;
};
void* serializar_mensaje_cpu(t_mensaje_cpu mensaje_cpu){
    void* buffer;

    return buffer;
};
//t_mensaje_cpu* deserializar_mensaje_cpu(void* buffer) {
//	t_mensaje_cpu* auxiliar;
//	int id;
//	int key;
//	t_clavevalor data;
//
//
//	auxiliar->id_dbt = id;
//	auxiliar->keyword = key;
//	auxiliar->clave_valor.clave = data.clave;
//	auxiliar->clave_valor.valor = data.valor;
//	auxiliar->clave_valor.path = data.path;
//
//	free(buffer);
//	return auxiliar;
//}



void* serializar_status_clave(t_status_clave* status_clave, int* tamanio) {
        int len_cpu = strlen_null(status_clave->cpu);
        int len_cpu_ahora = strlen_null(status_clave->cpu_ahora);
        int len_valor = strlen_null(status_clave->valor);

        *tamanio = len_cpu + len_cpu_ahora + len_valor + 3 * sizeof(int);

        void *buffer = malloc(*tamanio);
        void *tmp = buffer;

        memcpy(tmp, &len_cpu, sizeof(int));
        tmp = tmp + sizeof(int);
        memcpy(tmp, &status_clave->cpu, len_cpu);
        tmp = tmp + len_cpu * sizeof(char);

        memcpy(tmp, &len_cpu_ahora, sizeof(int));
        tmp = tmp + sizeof(int);
        memcpy(tmp, &status_clave->cpu_ahora, len_cpu_ahora);
        tmp = tmp + len_cpu_ahora * sizeof(char);

        memcpy(tmp, &len_valor, sizeof(int));
        tmp = tmp + sizeof(int);
        memcpy(tmp, &status_clave->valor, len_valor);
        tmp = tmp + len_valor * sizeof(char);

        return buffer;
}

t_status_clave* deserializar_status_clave(void* buffer) {
        int offset = 0;

        int *len_cpu = buffer;
        offset += sizeof(int);
        char *cpu = buffer + offset;
        offset += *len_cpu * sizeof(char);
        int *len_cpu_ahora = buffer + offset;
        offset += sizeof(int);
        char *cpu_ahora = buffer+offset;
        offset += *len_cpu_ahora * sizeof(char);
        int *len_valor = buffer+offset;
        offset += sizeof(int);
        char *valor = buffer+offset;

        t_status_clave* status_clave = malloc(sizeof(t_status_clave));
        status_clave->cpu = strdup(cpu);
        status_clave->cpu_ahora = strdup(cpu_ahora);
        status_clave->valor = strdup(valor);
        return status_clave;
}

void loggear(t_log* log_app, int type, char* level_char, char* template, ...) {
	va_list args;
	va_start(args, template);

	char* texto = string_from_vformat(template, args);

	switch (type) {

	case LOG_LEVEL_ERROR:
		log_error(log_app, texto);
		log_error(log_consola, texto);
		break;
	case LOG_LEVEL_WARNING:
		log_warning(log_app, texto);
		log_warning(log_consola, texto);
		break;
	case LOG_LEVEL_INFO:
		log_info(log_app, texto);
		log_info(log_consola, texto);
		break;
	case LOG_LEVEL_DEBUG:
		log_debug(log_app, texto);
		log_debug(log_consola, texto);
		break;
	case LOG_LEVEL_TRACE:
		log_trace(log_app, texto);
		log_trace(log_consola, texto);
		break;
	}

	free(texto);
	va_end(args);
}
/*
*/

