#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <stddef.h>
//#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
//#include <sys/select.h>
#include <sharksCommons/logs.h>
#include "modulos/safaConfig.h"
#include "modulos/consola.h"
#include "modulos/planificadorCP.h"
#include "modulos/conexionCPUDAM.h"


#define SAFA_LOG_PATH "./safa.log"
#define PROGRAM_NAME "SAFA"

#define ACTIVE_CONSOLE 1


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) + 24 )

#define BUF_LEN     ( 1024 * EVENT_SIZE )
int fd_inotify, watch_descriptor, max_fd;
fd_set readfds;
struct timeval waiting;
void* thread_continuous_scan_notify(char * argv);
#define EVENT_SIZE ( sizeof (struct inotify_event) + 24 )
#define BUF_LEN ( 1024 * EVENT_SIZE )

const char *message1 = "/home/utnso/workspace/tp-2018-2c-Sharks/safa/properties/safa.properties";

void initNuevaConfiguracion(int argc , char* argv);
void  verificadorArchivos(char *  );
void init_notify(char* );

pthread_t notify_thread;


/*** INICIO:  VARIABLES GLOBALES ******/

/*** FIN :VARIABLES GLOBALES ***/


/*------------- FUNCIONES ---------------*/
void initConfiguracion(int, char*);
void initThreadConsola ();
void initThreadPlanificadores();
void verificarEstadoNoCorrupto(int* variable);
void initThread();
void estaOperativo();
void initThreadCpuAndDam ();


#endif  /* PLANIFICADOR_H_ */
