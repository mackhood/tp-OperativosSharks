#include "parser.h"


#define return_comentario t_cpu_operacion COMENTARIO = { .valido = false }; return COMENTARIO
#define RETORNA_ERROR 		t_cpu_operacion NO_VALIDO = { .valido = false }; return NO_VALIDO
#define abrir_w		"abrir"//		001
#define concentrar_w	"concentrar"//		002
#define asignar_w	"asignar"	//	003
#define wait_w		"wait"	//		004
#define senial_w	"signal"	//	005
#define flush_w		"flush"		//	006
#define close_w		"closes"	//	007
#define crear_w		"crear"	//		008
#define borrar_w	"borrar" //		009
/*#define _CHECK_CLAVE x if(string_length(x) > 40){\
						fprintf(stderr, "La clave <%.40s...> es muy larga\n", (x)); \
						RETURN_ERROR; \
					   }*/

bool validar(char* path, char* line) {
	if (path == NULL) {
		fprintf(stderr, "Error faltan datos en la linea: %s", (line));
		return (false);
	} else {
		return (true);
	};
}
void destruir_operacion(t_cpu_operacion op) {
	if (op._raw) {
		string_iterate_lines(op._raw, (void*) free);
		free(op._raw);
	}
	//(void*) free(op);
}

t_cpu_operacion parse(char* line){
	if (string_is_empty(line) || string_equals_ignore_case(line, " ")) {
		fprintf(stderr, "Línea vacía fin de Escriptorio\n");
		return_comentario
;

	}else if(string_starts_with(line, "#")){
		return_comentario;
	};

	t_cpu_operacion ret;

	//bool 	string_starts_with(char * text, char * begin);



	char* auxLine = string_duplicate(line);
	string_trim(&auxLine);					//vacios de la derecha y la izquierda
	char** split = string_n_split(auxLine, 4, " "); //Separa un string tantas veces por su separador como n lo permita

	char* keyword = split[0];
	char* path = split[1];
	char* linea = split[2];
	char* dato = split[3];
	char* recurso = split[1];
	/*if(string_length(path) > 100){
		fprintf(stderr, "La clave <%.100s...> es muy larga\n", (path)); \
		RETURN_ERROR;
	 }*/



	ret._raw = split;
	/*enum{
		abrir,
		concentrar,
		asignar,
		wait,
		senial,
		flush,
		close,
		crear,
		borrar,
	}clave;*/
	if(string_starts_with(keyword, abrir_w)) {
			ret.keyword = abrir;
		ret.valido =validar(path, line);
			ret.argumentos.abrir.path = split[1];
	} else if(string_starts_with(keyword, concentrar_w)) {
			ret.keyword = concentrar;
		ret.valido =true;
	} else if(string_starts_with(keyword, asignar_w)) {
				ret.keyword = asignar;
		ret.valido =validar(path, line);
				ret.argumentos.asignar.path = split[1];
		ret.valido =validar(path, line);
				ret.argumentos.asignar.linea = split[2];
		ret.valido =validar(path, line);
				ret.argumentos.asignar.dato = split[3];
	} else if(string_starts_with(keyword, wait_w)) {
					ret.keyword = wait;
		ret.valido=validar(recurso, line);
					ret.argumentos.wait.recurso = split[1];
	} else if(string_starts_with(keyword, senial_w)) {
						ret.keyword = senial;
		ret.valido=validar(recurso, line);
						ret.argumentos.senial.recurso = split[1];
	} else if(string_starts_with(keyword, flush_w)) {
							ret.keyword = flush;
		ret.valido=validar(path, line);
							ret.argumentos.flush.path = split[1];
	} else if(string_starts_with(keyword, close_w)) {
								ret.keyword = closes;
		ret.valido=validar(path, line);
								ret.argumentos.closes.path = split[1];
	} else if(string_starts_with(keyword, crear_w)) {
									ret.keyword = crear;
		ret.valido=validar(path, line);
									ret.argumentos.crear.path = split[1];
		ret.valido=validar(linea, line);
									ret.argumentos.crear.linea =split[2];
	} else if(string_starts_with(keyword, borrar_w)) {

										ret.keyword = borrar;
		ret.valido=validar(path, line);
										ret.argumentos.borrar.path = split[1];
	} else {

		fprintf(stderr, "No se encontro el keyword <%s>\n", keyword);
		ret.valido=false;
	};

	free(auxLine);
	return ret;
}
