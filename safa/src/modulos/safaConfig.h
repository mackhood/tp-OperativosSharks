#ifndef SRC_SAFACONFIG_H_
#define SRC_SAFACONFIG_H_

#include <sys/select.h>
#include <stdbool.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/config.h>
#include <sharksCommons/protocoloSafaCPU.h>
#include <sharksCommons/logs.h>
#include <time.h>

pthread_mutex_t mutextListGDT;
pthread_mutex_t mutextListRecurso;

typedef struct{

	int idGDT;	 //Id dtb
	int flag; //flag de inicializacion
	char* path; // ruta del escriptorio
	int quantum; //dato el ultimo algoritmo del hito presenciar, el quantum puede cambiar por DTB
	int pc;
	t_list* tablaDeArchivosAbiertos;
	int posMemoria;

	int total_sentencias;
	char* sentencias;
	int siguiente_io;
	int cant_sentencia_new;
	int cant_usaron_diego; //promedio
	t_queue* prox_io;
	bool se_ejecuto;
	double tiempo_repuesta;
	time_t horacreacion;  //se usa para calcular el tiempo de repuesta

}DTB_SAFA;


/* Configuracion properties safa */
typedef struct{
	int	puerto;
	int	quantum;
	int gradoMultiprogramacion;
	int retardoPlanificacion;
	char* algoritmoPlanificacion;
}t_safa_config;

/**Configuracion general safa **/
typedef struct {
	fd_set  fdSetSelect;  //conjunto descriptor
	fd_set	fdSetCliente;
	int		maximoSocket;
	int		fdServidor;
	int		fdDma;
	t_safa_config* config;
	DTB_SAFA* dtb_dummy;  // Es unico , y se envia a la cola de bloquados , ni bien levanta el proceso
	int cantidadCpuConectados;
	int conexionProcesoDiego;
	t_list* listCpu;
	int operativo;

}t_safa;


typedef struct {
	int idGDT;
	t_list* recursos;
}GDT;

GDT* buscar_gdt_by_id(int pid);
void agregarGDT(GDT* gdt);
GDT* crearGdt(int idGdt);
void destroyGdt(GDT* gdt);
void removeGDT(GDT* gdt);

typedef struct {
	int valor;
	char* recurso;
	t_queue* en_espera;
}t_recurso;

t_recurso* get_recurso(char* recurso);
void asigna_recurso(t_recurso* rec, GDT* gdt);
void destroyrecurso(t_recurso* rec);
void actualizar_recurso(char* recurso, t_recurso* rec);
t_dictionary* tabla_recursos;
t_list* tablaGDT;
t_recurso* crear_recurso(char* recurso);
t_recurso* extraer_recurso(char* recurso);
void crear_recurso_en_sistema(char* recurso, t_recurso* rec);
void load_config(t_config **config, int argc, char* dir);

typedef struct {
	char* path;
	int posMemoria;
}t_archivoAbierto;

t_archivoAbierto* crear_archivo_abierto(char* path, int posMemoria);


t_safa* tSafa;
t_config* config;
t_safa_config* safaConfig;






#endif /* SRC_SAFACONFIG_H_ */
