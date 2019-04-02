
#include "protocoloMdjDAM.h"

E_MDJ* crearE_MDJ(char* cabecera, int codigo, int base, int offset, char* path, char* buffer,
		int clave, int linea, int tamArchivo) {
	E_MDJ* e_mdj = calloc(1, sizeof(E_MDJ));

	e_mdj->cod = codigo;
	e_mdj->base = base;
	e_mdj->offset = offset;
	e_mdj->clave = clave;
	e_mdj->tamArchivo = tamArchivo;
	e_mdj->lineas = linea;
	e_mdj->path = malloc(strlen_null_mdj(path));
	e_mdj->cabecera = malloc(strlen_null_mdj(cabecera));
	e_mdj->buffer = malloc(strlen_null_mdj(buffer));
	memcpy(e_mdj->path, path, strlen_null_mdj(path));
	memcpy(e_mdj->cabecera, cabecera, strlen_null_mdj(cabecera));
	memcpy(e_mdj->buffer, buffer, strlen_null_mdj(buffer));
	return e_mdj;

}


char* serializar_E_MDJ(E_MDJ* object){

	char* cod_cabecera = string_new();
	char* codigo =   armar_codigo( object->cod );
	string_append(&cod_cabecera, object->cabecera);
	string_append(&cod_cabecera, codigo);

	char* buffer = string_new();
	char* payPath =	armar_mensaje(cod_cabecera, object->path);
	string_append(&buffer, payPath);

	char* base = string_itoa(object->base);
	char* paybase = armar_mensaje("", base);
	string_append(&buffer, paybase);
	char* offset = string_itoa(object->offset);
	char* payoffset = armar_mensaje("", offset);
	string_append(&buffer, payoffset);
	char* clave = string_itoa(object->clave);
	char* payclave = armar_mensaje("", clave);
	string_append(&buffer, payclave);
	char* lineas = string_itoa(object->lineas);
	char* paylineas = armar_mensaje("", lineas);
	string_append(&buffer, paylineas);
	char* paybuffer = armar_mensaje("", object->buffer);
	string_append(&buffer, paybuffer);
	char* tamarchivo = string_itoa(object->tamArchivo);
	char* paytamarchivo = armar_mensaje("", tamarchivo);
	string_append(&buffer, paytamarchivo);


	free( payPath );
	free( base );
	free( paybase );
	free( offset );
	free( payoffset );
	free( clave );
	free( payclave );
	free( lineas );
	free( paylineas );
	free( paybuffer );
	free( tamarchivo );
	free( paytamarchivo );
	free(cod_cabecera);
	free(codigo);
	return buffer;
}



E_MDJ* deserializar_E_MDJ(char* buffer){
	int inicio, fin;
	fin = 1;
	inicio = 0;
	E_MDJ* object = malloc(sizeof(E_MDJ));

	char* cab = string_substring(buffer, inicio, fin);
	object->cabecera = malloc( strlen_null(cab) );
	memcpy(object->cabecera, cab, strlen_null(cab) );
	free(cab);

	char* cod = string_substring(buffer, fin, 2);
	object->cod = atoi( cod );
	free( cod );

	char* path_pay =  string_substring(buffer, 3, 10);
	int payload1 = atoi( path_pay );
	char* path = string_substring(buffer, 3 + 10, payload1);
	object->path = malloc( strlen_null(path) );
	memcpy(object->path, path, strlen_null(path) );
	free( path_pay );
	free( path );


	char* pay_base = string_substring(buffer, 13 + payload1, 10);
	int payload2  = atoi( pay_base );
	char* base = string_substring(buffer, 13 + payload1 + 10, payload2);
	object->base = atoi( base );
	free(base);
	free(pay_base );


	char* offset_pay = string_substring(buffer, 13 + payload1 + 10 + payload2, 10);
	int payload3  = atoi( offset_pay );
	char* offset = string_substring(buffer,13 + payload1 + 10 + payload2 + 10, payload3);
	object->offset = atoi( offset );
	free(offset);
	free( offset_pay );

	char* pay_clave = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3, 10);
	int payload4 = atoi( pay_clave );
	char* clave = string_substring(buffer, 13 + payload1 + 10 + payload2 + payload3 + 10, payload4);
	object->clave = atoi( clave );
	free( pay_clave );
	free( clave );


	char* pay_lineas = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3 + 10 + payload4, 10);
	int payload5 = atoi( pay_lineas );
	char* lineas  = string_substring(buffer, 13 + payload1 + 10 + payload2 + payload3 + 10 + payload4 + 10, payload5);
	object->lineas = atoi( lineas );
	free( pay_lineas );
	free( lineas );


	char* pay_buffer = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3
			+ 10 + payload4 + 10 + payload5, 10);
	int payload6  = atoi( pay_buffer );
	char* buffer_ = string_substring(buffer,13 + payload1 + 10 + payload2 + payload3 + 10 + payload4
			+ 10 + payload5 + 10, payload6);
	object->buffer = malloc( strlen_null(buffer_) );
	memcpy(object->buffer, buffer_, strlen_null(buffer_) );
	free( pay_buffer );
	free( buffer_ );


	char* pay_tam = string_substring(buffer, 13 + payload1 + 10 + payload2 + 10 + payload3 + 10
			+ payload4 + 10 + payload5 + 10 + payload6, 10);
	int payload7 = atoi( pay_tam );
	char* tam = string_substring(buffer,13 + payload1 + 10 + payload2 + payload3 + 10 + payload4
			+ 10 + payload5 + 10 + payload6 + 10, payload7);
	object->tamArchivo = atoi( tam );

	free( pay_tam );
	free( tam );


	return object;
}




int sizeof_clavevalor_m(t_clavevalor_m cv) {
	int tamanio_clave = (strlen_null(cv.clave))* sizeof(char);
	int tamanio_valor = (strlen_null(cv.valor)) * sizeof(char);
	int tamanio_path = (strlen_null(cv.path))* sizeof(char);

	int tamanio = sizeof(int) * 3 + tamanio_clave + tamanio_path + tamanio_valor;
	return tamanio;
};
void* serializar_clavevalor(t_clavevalor_m clave_valor) {
	int tamanio_clave = strlen_null(clave_valor.clave);
	int tamanio_valor = strlen_null(clave_valor.valor);
	int tamanio_path = strlen_null(clave_valor.path);
	int size_cv = sizeof_clavevalor_m(clave_valor);

	void* buffer = malloc(size_cv);

	  //memcpy (new, old, arraysize * sizeof (struct foo));
	memcpy(buffer, &(tamanio_clave), sizeof(int));
	memcpy(buffer + sizeof(int), &tamanio_valor, sizeof(int));
	memcpy(buffer + sizeof(int)*2, &(tamanio_path), sizeof(int));

	memcpy(buffer + sizeof(int) * 3, clave_valor.clave, tamanio_clave * sizeof(char));

	if (tamanio_valor > 0) {
			memcpy(buffer + sizeof(int)*2 + tamanio_clave*sizeof(char), clave_valor.valor, tamanio_valor * sizeof(char));
		};
	if (tamanio_path > 0){
		memcpy(buffer + sizeof(int)*3 + (tamanio_clave + tamanio_valor)*sizeof(char), clave_valor.path, tamanio_path * sizeof(char));
	};



	return buffer;
};
t_clavevalor_m* deserializar_clavevalor_m(void* buffer) {
	t_clavevalor_m* auxiliar;
	int* l_clave = buffer;
	int* l_valor = l_clave + 1;
	int* l_path = l_valor + 1;

	char* clave = (char*) (l_valor + 1);
	char* valor = (char*)(l_path + 1);
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
t_mensaje_cpu_m* deserializar_mensaje_cpu(void* buffer) {
	t_mensaje_cpu_m* auxiliar;
	int id;
	int key;
	t_clavevalor_m data;


	auxiliar->id_dbt = id;
	auxiliar->keyword = key;
	auxiliar->clave_valor.clave = data.clave;
	auxiliar->clave_valor.valor = data.valor;
	auxiliar->clave_valor.path =data.path;

	free(buffer);
  	return auxiliar;
};
int strlen_null_mdj(char* frase) {
	 if (frase == NULL) {
	 		return 0;
	 	}
	 	return strlen(frase) + 1;
 }


void destruir_E_MDJ(E_MDJ* fifa) {
	free(fifa->buffer);
	free(fifa->cabecera);
	free(fifa->path);
	free(fifa);
}


