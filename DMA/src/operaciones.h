
#include <commons/config.h>
#include <commons/collections/queue.h>
#include <sharksCommons/logs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   //strlen
#include <errno.h>
#include <sys/time.h>
#include <sharksCommons/serealizacion.h>
#include <sharksCommons/sockets.h>
#include <sharksCommons/protocolo.h>
#include <sharksCommons/mensaje.h>
#include <sharksCommons/protocoloCpuDAM.h>
#include <sharksCommons/protocoloMdjDAM.h>
#include <sharksCommons/protocoloSafaDAM.h>
#include <sharksCommons/protocoloFm9DAM.h>
#include <pthread.h>
#include <stdbool.h>


typedef struct{
	int	puerto_dma;
	int	puerto_safa;
	int	puerto_mdj;
	int	puerto_fm9;
	char* ip_safa;
	char* ip_mdj;
	char* ip_fm9;
	int t_size;
}t_dma_config;


int fd_safa;
int fd_fm9;
int fd_mdj;
int fd_dma;

int tam_max_linea;
t_dma_config* dma_struct;

pthread_mutex_t mutexMdj;
pthread_mutex_t mutexFm9;
pthread_mutex_t mutexSafa;

#define TAM_RECIBIR 20000

void operacion_abrir(char*, int);
void operacion_flush(char*);
void operacion_crear(char*);
void operacion_borrar(char*);

