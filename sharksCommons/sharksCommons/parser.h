
#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <commons/string.h>

typedef struct {
	bool valido;
	enum keyword{
		abrir = 9000,
			concentrar,
			asignar,
			wait,
			senial,
			flush,
			closes,
			crear,
			borrar,
		}keyword;
	union argumentos {
		struct {
				char* path;
			}abrir;
			struct {

			}concentrar;
			struct {
				char* path;
				char* linea;
				char* dato;
			}asignar;
			struct {
				char* recurso;
			}wait;
			struct {
				char* recurso;
			}senial;
			struct {
				char* path;
			}flush;
			struct {
				char* path;
			}closes;
			struct {
				char* path;
				int linea;
			}crear;
			struct {
				char* path;
			}borrar;
		}argumentos;
		char** _raw; //Para uso de la liberación
	}t_cpu_operacion;

	/**
	* @NAME: parse
	* @DESC: interpreta una linea de un archivo CPU y
	*		 genera una estructura con el operador interpretado
	* @PARAMS:
	* 		line - Una linea de un archivo CPU
	*/
	t_cpu_operacion parse(char* line);

	/**
	* @NAME: destruir_operacion
	* @DESC: limpia la operacion generada por el parse
	* @PARAMS:
	* 		op - Una operacion obtenida del parse
	*/
void destruir_operacion(t_cpu_operacion op);
bool validar(char* path, char* line);
#endif /* PARSER_H_ */
