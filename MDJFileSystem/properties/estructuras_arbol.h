
#ifndef _ESTRUCTURAS_ARBOL_H_
#define _ESTRUCTURAS_ARBOL_H_

#include "configurar.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stab.h>
#include <stdlib.h>
#include <sys/dir.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stab.h>
#include <stdlib.h>
#include <sys/dir.h>
#include <dirent.h>
#include <aio.h>
#include <curses.h>
#include <ncurses.h>


typedef struct {
	ino_t numero_inodo; /* inode number */
	off_t sgt_off; /* offset to the next dirent */
	unsigned short tamanio; /* length of this record */
	unsigned char tipo; /* type of file; not supported
	 by all file system types */
	char* nombre; /* filename */
} t_dir;
/*typedef struct {
 dev_t st_dipositivo;  dispositivo
 ino_t st_ino;  numero de inode
 mode_t st_mode;  modo del fichero
 nlink_t st_nlink;  numero de hard links
 uid_t st_uid;  UID del propietario
 gid_t st_gid;  GID del propietario
 dev_t st_rdev;  tipo del dispositivo
 off_t st_size;  tamaño total en bytes
 blksize_t st_blksize;  tamaño de bloque preferido
 blkcnt_t st_blocks;  numero de bloques asignados
 timer_t ultimo_a;  ultima hora de acceso
 timer_t ultima_mod; ultima hora de modificación
 timer_t ultimo_inodo; ultima hora de cambio en inodo
 } t_estado;*/

typedef struct {
	int dato;
	char* *i, *d;
} t_arbol;
typedef struct {
	int dato;
	t_arbol *i, *d;
} arbol;
arbol *elemento, *aux, *cabecera, *ant, *aux2, *ant2;
void gotoxy(int x, int y);
int buscar(void);
void insertar(void);
void buscarmenmay(void);
void buscarmaymen(void);
void eliminar(void);
void imprimir(struct stat estru);
void liberarA();
arbol* p_arbol;

#endif /* _ESTRUCTURAS_ARBOL_H_ */
