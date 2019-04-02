#ifndef LIBENIAC_LOGS_H_
#define LIBENIAC_LOGS_H_

#include <commons/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <commons/string.h>
#include <stdbool.h>

//TIPOS DE NIVEL PARA LOGGEAR

#define LOG_FILE "LOG_FILE"
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"

t_log* crearLogger(char * dir_log, char * logMemoNombreArch);

void logDestroy();
void initMutexlog(char* archivo,char* nombrePrograma,bool activarConsola,t_log_level level);
void logError(const char* string, ...);
void logWarning(const char* string, ...);
void logTrace(const char* string, ...);
void logDebug(const char* string, ...);
void logInfo(const char* string, ...);

#endif /* LIBENIAC_LOGS_H_ */
