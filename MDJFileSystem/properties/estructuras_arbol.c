#include "estructuras_arbol.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stab.h>
#include <stdlib.h>
#include <sharksCommons/protocolo.h>
#include <sys/dir.h>
#include <dirent.h>
#include <aio.h>
#include <curses.h>
#include <ncurses.h>


int dato;
/*void main(void) {
	int y, opc;
	arbol* p_arbol;
	do{
		clear();
		y = 10;
		gotoxy(10, y++);
		printf("0 - Salir \n");
		gotoxy(10, y++);
		printf("1 - Buscar \n");
		gotoxy(10, y++);
		printf("2 - Insertar \n");
		gotoxy(10, y);
		printf("3 - Borrar \n");
		gotoxy(10, y += 5);
		printf("Cual es su opcion: ");
		getch();
		scanf ("%d", &opc);
		//fgets (opc, 2, stdin);
		//opc = getch();
		switch (opc) {
		case 0:
			break;
		case 1:
			printf("\n\nDato a buscar: ");

			p_arbol->dato = getch();
			scanf ("%d", &opc);
			if (buscar())
				printf("\n\nDato existe");
			else
				printf("\n\nDato inexistente");
			break;
		case 2:
			printf("\n\nDato a insertar: ");
			scanf ("%s", &p_arbol->dato);
			p_arbol->dato = getch();
			insertar();
			printf("\n\nDato Insertado");
			break;
		case 3:
			printf("\n\nDato a borrar: ");
			p_arbol->dato = getch();
			eliminar();
			break;
		default:
			printf("\n\nOpcion incorrecta");
		};
		if (opc)
			getch();
	}while(opc);

 }*/

void gotoxy(int x, int y){
	 printf("\033|[%d;%df",y,x);
	 }
int buscar(void) {
	/*if (!cabecera) {
		printf("No hay arbol");
		return (0);
	}
	ant = NULL;
	aux = cabecera;
	while (aux) {
		if (dato == aux->dato)
			return (1);
		else {
			ant = aux;
			if (dato > aux->dato)
				aux = aux->d;
			else
				aux = aux->i;
		}
	 }*/
	return (0);
}

void insertar(void) {
	if (!cabecera) {
		cabecera = malloc(sizeof(arbol));
		cabecera->dato = dato;
		cabecera->d = NULL;
		cabecera->i = NULL;
		return;
	}
	if (!buscar()) {
		aux = malloc(sizeof(arbol));
		aux->dato = dato;
		aux->i = NULL;
		aux->d = NULL;
		if (dato > ant->dato)
			ant->d = aux->d;
		else
			ant->i = aux->i;
	} else
		printf("\n\nDato existente");
}

void buscarmenmay(void) {
	aux2->d = aux->d;
	ant2 = aux;
	while (aux2->i) {
		ant2 = aux2;

	}
	aux->dato = aux2->dato;
	if (aux2->d)
		ant2->i = aux2->d;
	free(aux2);
	ant2->d = NULL;
}

void buscarmaymen(void) {
	aux2->i = aux->i;
	ant2 = aux;
	while (aux2->d) {
		ant2 = aux2;
		aux->d = aux2->d;
	}
	aux->dato = aux2->dato;
	if (aux2->i)
		ant2->d = aux2->i;
	free(aux2);
	ant2->i = NULL;
}

void eliminar(void) {
	if (!buscar()) {
		printf("\n\nElemento no encontrado.");
		liberarA();
		return;
	}
	if (aux->d == NULL && aux->i == NULL) {
		if (ant->dato > dato)
			ant->i = NULL;
		else
			ant->d = NULL;
		free(aux);
	} else if (aux->d != NULL)
		buscarmenmay();
	else
		buscarmaymen();
	printf("\n\nElemento Borrado");
}
void liberarA() {
	free(cabecera->d);
	//free(cabecera->dato);
	free(cabecera->i);
	free(cabecera);
	free(aux->d);
	//free(aux->dato);
	free(aux->i);
	free(aux);
	//free(p_arbol->dato);
	//free(p_arbol->d);
	//free(p_arbol->i);

}
