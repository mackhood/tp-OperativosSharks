/*
 ============================================================================
 Name        : FM9 - Funes Memory 9
 Author      : Sharks
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "FM9.h"
#include <commons/config.h>
#include <commons/string.h>
#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include "protocolo_FM9.h"
#include "fm9_utils.h"
#include "serializacion_fm9.h"

//Variables Globales

int cliente[30];
pthread_t hiloEsperarMensaje[30];
int hilo;
t_log *log_;
t_fm9 * data_fm9;
int socketServerFM9;
int controlador;
char* Memoria;
int tamanioMemoria;
int maxtamanioLinea;
int tamanioPagina;
int memoriaLibre;

//Administracion de Memoria
t_tablaSegmentos* tablaSegmentos;
t_tablaPaginas* tablaPaginas;
t_tablaLineas* tablaLineas;

//Funciones
void leerArchivoConfig(char* rutaArchivoConfig);
void esperar_mensaje(void *i);
void mostrarTablaSegmentos( t_tablaSegmentos* tablaSegmentos );
int segmentoLibre(void);

t_tablaSegmentos* crearEstrucTablaSegmentos(void);
t_tablaLineas* crearEstrucTablaLineas(void);
int cantidadDeLineas(int tamanioBytes);
int hayEspacioEnMemoria(int tamanioArchivo);
int segmentoLibre(void);
void cargartablaSegmentos( int pid, int seg, int tamanioArchivo);
void cargarLinea( int seg, int linea, int tamanioLinea, char* datalinea);
char * solicitarLinea ( int posMemoria, int linea);
int actualizarLinea( int seg, int linea, int tamanioLinea, char* datalinea);
int liberaRecursosMemoria( int pid, int referenciaMemoria);
void mostrarLineas(void);

int main(int argc, char *argv[]){

	crear_archivo_log("/home/utnso/Escritorio/FM9-LOG.txt");
	leerArchivoConfig(argv[1]);

	socketServerFM9 = iniciar_socket_server_f(data_fm9->IP_FM9,data_fm9->PUERTO_FM9,&controlador);

	char *log_aux = string_from_format("Se inicia Servidor de Memoria FM9 en Socket: %d ",socketServerFM9);
	escribir_log(log_aux);
	free(log_aux);

	tablaSegmentos = crearEstrucTablaSegmentos();
	tablaLineas=crearEstrucTablaLineas();

    // -- CREA ESPACIO DE MEMORIA --
	Memoria = malloc(tamanioMemoria);
    memset(Memoria,'\0',tamanioMemoria);
    log_aux = string_from_format("Se crea Memoria de Tamanio: %d Bytes",tamanioMemoria );
    memoriaLibre = tamanioMemoria;
    escribir_log(log_aux);
    // -- CREA ESPACIO DE MEMORIA --


    hilo = 0;
   	while (hilo < 30) {
    		cliente[hilo] = escuchar_conexiones_f(socketServerFM9,&controlador);
    		char *log_aux= string_from_format("FM9 - Conexión entrante en Socket: %d ",cliente[hilo]);
    		escribir_log(log_aux);
    		free(log_aux);

    		// Realizar HS y enviar Tamaño MARCO.
    		char *mensaje_recibido = recibir_f(cliente[hilo], &controlador);
    		char *header = get_header(mensaje_recibido);
    		if(strcmp(header,CABECERA_CPU)==0 || strcmp(header,CABECERA_DMA)==0 )
    		{
    		char *HS_OK = armar_mensaje(HANDSHAKE_FM9,""); // F|00|
    		enviar_f(cliente[hilo],HS_OK, &controlador);
    		free(HS_OK);
    		}
    		else
    		{
    			char *aux_log = string_from_format("FM9 - HANDSHAKE ERROR en Socket: %d ",cliente[hilo]);
    			escribir_log(aux_log);
    			free(aux_log);
    			cerrar_conexion_f(cliente[hilo]);
    			goto salir_handshake;
    		}
    		free(mensaje_recibido);
    		free(header);

    		//.:HILO ESCUCHA PETICIONES DE CLIENTES:.
    		pthread_create(&hiloEsperarMensaje[hilo], NULL, (void*) esperar_mensaje,(void *) cliente[hilo]);

    		log_aux= string_from_format("El Cliente %d tiene Hilo nro :%d ",cliente[hilo],hilo);
    		escribir_log(log_aux);
    		free(log_aux);

    		// RECIBIR

    		salir_handshake:
    		hilo++;
    	}
pthread_join(hiloEsperarMensaje[hilo],NULL);
free (Memoria);
free(tablaSegmentos);
free(tablaLineas);
return EXIT_SUCCESS;

}

void leerArchivoConfig(char* rutaArchivoConfig)
{

	data_fm9 = malloc(sizeof(t_fm9));
	data_fm9->IP_FM9 = strdup("");
	data_fm9->MODO = strdup("");
	t_config *configuracion = config_create(rutaArchivoConfig);
	string_append(&data_fm9->IP_FM9,config_get_string_value(configuracion,"IP_FM9"));
	string_append(&data_fm9->MODO,config_get_string_value(configuracion,"MODO"));
	data_fm9->PUERTO_FM9 = config_get_int_value(configuracion,"PUERTO_FM9");
	data_fm9->TAMANIO= config_get_int_value(configuracion,"TAMANIO");
	data_fm9->MAX_LINEA = config_get_int_value(configuracion,"MAX_LINEA");
	data_fm9->TAM_PAGINA = config_get_int_value(configuracion,"TAM_PAGINA");
	printf(" ----------------------------------------------------------\n");
	printf("IP FM9: %s  \n",data_fm9->IP_FM9);
	printf("PUERTO FM9: %d  \n",data_fm9->PUERTO_FM9);
	printf("MODO MEMORIA: %s  \n",data_fm9->MODO);
	printf("TAMANIO DE MEMORIA: %d  \n",data_fm9->TAMANIO);
	printf("MAXIMO DE LINEA: %d  \n",data_fm9->MAX_LINEA);
	printf("TAMANIO DE PAGINA: %d  \n",data_fm9->TAM_PAGINA);
	printf(" ----------------------------------------------------------\n");

	tamanioMemoria = data_fm9->TAMANIO;
	maxtamanioLinea=data_fm9->MAX_LINEA;
	tamanioPagina=data_fm9->TAM_PAGINA;

	config_destroy(configuracion);
}

void esperar_mensaje(void *i) {

	int cliente = (int) i;
	int chau = 0;
	while(chau != 1)
	{
		char *mensRec = malloc(1024);
		memset(mensRec,'\0', 1024);
		mensRec = recibir_f(cliente, &controlador);
		if(controlador != 0)
		{
			cerrar_conexion_f(cliente);
			chau =1;
			hilo=30;
		}
		char *header=get_header(mensRec);
		char *cod = get_codigo(mensRec);
		int codigo = atoi(cod);
		//escribir_log_compuesto(" HEADER: ",header);
		//escribir_log_con_numero(" CODIGO: ",codigo);
		if (!strcmp(header,CABECERA_DMA))
		{
			switch (codigo)
			{
					case ABRIR_ARCHIVO:
					{
					escribir_log(" OPERACION ABRIR_ARCHIVO ");

					int pid= atoi(string_substring(mensRec, 3, 10));
					int lineasArchivo= atoi(string_substring(mensRec, 13, 10));
					int tamanioPath= atoi(string_substring(mensRec, 23, 10));
					int tamanioLinea = atoi(string_substring(mensRec, 33+tamanioPath, 10));
					//int tamanioArchivo = atoi(string_substring(mensRec, 43+tamanioPath+tamanioLinea,10));

					int tamanioArchivoEnMemoria= lineasArchivo*maxtamanioLinea;

					char* dataLinea = string_substring(mensRec,43+tamanioPath,tamanioLinea);

					if ( hayEspacioEnMemoria(tamanioArchivoEnMemoria) == 1 ) {

						int linea = 1;
						int segLibre = segmentoLibre();
						cargartablaSegmentos(pid, segLibre, tamanioArchivoEnMemoria);
						cargarLinea( segLibre, linea, tamanioLinea,dataLinea);

						for (linea=1; linea < lineasArchivo; linea++){
							char *mensRecLinea = malloc(1024); memset(mensRecLinea,'\0', 1024);
							mensRecLinea = recibir_f(cliente, &controlador);
							int tamLinea = atoi(string_substring(mensRecLinea, 33+tamanioPath, 10));

							char * dataLinea_l = string_substring(mensRecLinea,43+tamanioPath,tamLinea);

							cargarLinea( segLibre, linea+1, tamLinea,dataLinea_l);
							free(mensRecLinea);
							free(dataLinea_l);
							}

							char *RTA_ABRIR_ARCHIVO= armar_mensaje("F01",string_itoa(segLibre)); // F|00|
							enviar_f(cliente,RTA_ABRIR_ARCHIVO, &controlador);
							free(RTA_ABRIR_ARCHIVO);
						} else {

							puts("ERROR - NO HAY ESPACIO EN MEMORIA - ERROR");
						}

					free(dataLinea);
					}
					break;
			}
			free(header);
			free(cod);
		}
		else{
			switch (codigo)
			{
					case SOLICITAR_LINEA:
					{
						char *log_aux2 = string_from_format("C07 - SOLICITAR LINEA: %d",cliente);
						escribir_log(log_aux2);
						free(log_aux2);

						int pid = atoi(string_substring(mensRec, 3, 10));
						int posMemoria = atoi(string_substring(mensRec, 13, 10));
						int nroLinea= atoi(string_substring(mensRec, 23, 10));

						char * dataLinea = malloc(maxtamanioLinea);
						memset(dataLinea,'\0',maxtamanioLinea);

						dataLinea = solicitarLinea(posMemoria,nroLinea);

						char * imp2 = string_from_format("Valor Linea: |%s|", dataLinea);
						escribir_log(imp2);

						char *RTA_SOLICITAR_LINEA= armar_mensaje("F01",dataLinea); // F|00|
						enviar_f(cliente,RTA_SOLICITAR_LINEA, &controlador);
						free(RTA_SOLICITAR_LINEA);

						free(imp2);
					}
					break;
					case ASIGNAR_LINEA: //C04
					{
						char *log_aux2 = string_from_format("C04 - ASIGNAR LINEA: %d",cliente);
						escribir_log(log_aux2);
						free(log_aux2);

						int pid = atoi(string_substring(mensRec, 3, 10));
						int posMemoria = atoi(string_substring(mensRec, 13, 10));
						int nroLinea= atoi(string_substring(mensRec, 23, 10));
						int tamDataLinea=atoi(string_substring(mensRec, 33, 10));
						char * dataLinea = string_substring(mensRec, 33+10, tamDataLinea);

						int LineaActualizada = actualizarLinea(posMemoria,nroLinea,tamDataLinea,dataLinea);

						if ( LineaActualizada == TRUE ){

							char *RTA_ASIGNAR_ARCHIVO= armar_mensaje("F01",""); //
							enviar_f(cliente,RTA_ASIGNAR_ARCHIVO, &controlador);
							free(RTA_ASIGNAR_ARCHIVO);

						} else {

							char *RTA_ASIGNAR_ARCHIVO= armar_mensaje("F02","SEG_FAULT"); //
							enviar_f(cliente,RTA_ASIGNAR_ARCHIVO, &controlador);
							free(RTA_ASIGNAR_ARCHIVO);
						}

					}
					break;
					case CERRAR_ARCHIVO: //C05
					{
						int pid= atoi(string_substring(mensRec, 3, 10));
						int referenciaMemoria= atoi(string_substring(mensRec, 13, 10));

						if (liberaRecursosMemoria(pid,referenciaMemoria) == TRUE) {

						char *RTA_CERRAR_ARCHIVO= armar_mensaje("F01",""); // F|00|
						enviar_f(cliente,RTA_CERRAR_ARCHIVO, &controlador);
						free(RTA_CERRAR_ARCHIVO);
						}else{
							char *RTA_CERRAR_ARCHIVO= armar_mensaje("F02","FALLO MEMORIA - 40002"); // F|00|
							enviar_f(cliente,RTA_CERRAR_ARCHIVO, &controlador);
							free(RTA_CERRAR_ARCHIVO);
						}
					}
					break;
					case 99:
					{
						mostrarTablaSegmentos(tablaSegmentos);
						mostrarLineas();
					}
					break;
			}
			free(cod);
			free(header);
		}
		chau:
		free(mensRec);
	}
}

t_tablaSegmentos* crearEstrucTablaSegmentos(void)
{
	int cantSegMax = SEGMENTOS;
	t_tablaSegmentos *estructuraTablaSegmentos= malloc(cantSegMax * sizeof(t_tablaSegmentos));
	int i;
	for(i=0;i<cantSegMax;i++){
		estructuraTablaSegmentos[i].pid=-1;
		estructuraTablaSegmentos[i].seg=-1;
		estructuraTablaSegmentos[i].base=-1;
		estructuraTablaSegmentos[i].limite=-1;
		estructuraTablaSegmentos[i].estado=0;
	}
	return estructuraTablaSegmentos;
}

t_tablaLineas* crearEstrucTablaLineas(void)
{
	int cantSegMax = SEGMENTOS;
	t_tablaLineas *estructuraTablaLineas= malloc(cantSegMax * sizeof(t_tablaLineas));
	int i;
	for(i=0;i<cantSegMax;i++){
		estructuraTablaLineas[i].seg=-1;
		estructuraTablaLineas[i].linea=-1;
		estructuraTablaLineas[i].base=-1;
		estructuraTablaLineas[i].tamanio=-1;
	}
	return estructuraTablaLineas;
}


int cantidadDeLineas(int tamanioBytes)
{
	double lineas= -1;
	if ((double)tamanioBytes/maxtamanioLinea - (int)(tamanioBytes/maxtamanioLinea) > 0){
		lineas = ((tamanioBytes/maxtamanioLinea) +1);
	}else{
		lineas = (int)(tamanioBytes/maxtamanioLinea);
	}
	return (int)lineas;
}
void mostrarTablaSegmentos( t_tablaSegmentos* tablaSegmentos ){
	int cantSegMax = SEGMENTOS;
	int p;
	char* imp;
	//escribir_log("pid|seg|base|offset|estado");
	for(p=0;p<cantSegMax;p++){
	imp = string_from_format("%d|%d|%d|%d|%d",tablaSegmentos[p].pid,tablaSegmentos[p].seg,tablaSegmentos[p].base,tablaSegmentos[p].limite,tablaSegmentos[p].estado);
	escribir_log(imp);
	free(imp);
	}
}

void mostrarLineas(void){
	int cantSegMax = SEGMENTOS;
	int p;
	char* imp;
	//escribir_log("seg|linea|tamanio");
	for(p=0;p<cantSegMax;p++){
	imp = string_from_format("%d|%d|%d|%d",tablaLineas[p].seg,tablaLineas[p].linea,tablaLineas[p].base,tablaLineas[p].tamanio);
	escribir_log(imp);
	free(imp);
	}
}

int hayEspacioEnMemoria(int tamanioArchivo){
int hayEspacio;
	if ( memoriaLibre- tamanioArchivo >= 0 ){
		hayEspacio = 1;
	}else{
		hayEspacio = 0;
	}
return hayEspacio;
}

int segmentoLibre(void){
	int i;
	int pos=-1;
	int cantSegMax= SEGMENTOS;
	for(i=0;i<cantSegMax;i++){
		if (tablaSegmentos[i].estado == 0) {
		pos = i;
		i=cantSegMax;
		}
	}
	char * imp2 = string_from_format("Segmento Libre %d", pos);
	escribir_log(imp2);
	free(imp2);

	return pos;
}

void cargartablaSegmentos( int pid, int seg, int tamanioArchivo){

if (seg == 0) {
		tablaSegmentos[seg].pid = pid;
		tablaSegmentos[seg].seg=seg;
		tablaSegmentos[seg].base=0;
		tablaSegmentos[seg].limite=tamanioArchivo; //-1
		tablaSegmentos[seg].estado = 1;
	} else {
		tablaSegmentos[seg].pid = pid;
		tablaSegmentos[seg].seg=seg;
		tablaSegmentos[seg].base=tablaSegmentos[seg-1].limite;
		tablaSegmentos[seg].limite=tablaSegmentos[seg-1].limite + tamanioArchivo; //TEST
		tablaSegmentos[seg].estado = 1;
	}
	puts("OK");
}

void cargarLinea( int seg, int linea, int tamanioLinea, char* datalinea){
	int cantSegMax= SEGMENTOS;
	int i=0;
	int pos=-1;
	for (i=0; i<cantSegMax;i++){
		if (tablaLineas[i].seg == seg) {
			pos = i;
			i=cantSegMax;
		}
	}
	if ( pos > -1) { //No es la primera linea
		tablaLineas[pos+linea-1].seg = seg;
		tablaLineas[pos+linea-1].linea = linea;
		//tablaLineas[pos+linea-1].base = tablaLineas[pos+linea-2].base +tablaLineas[pos+linea-2].tamanio;
		tablaLineas[pos+linea-1].base = (linea-1)*maxtamanioLinea;
		tablaLineas[pos+linea-1].tamanio = tamanioLinea;

		memcpy(Memoria+tablaSegmentos[seg].base+tablaLineas[pos+linea-1].base,datalinea,tamanioLinea);

	} else //Agregando primera linea
	{

		for(i=0;i<cantSegMax;i++){
			if (tablaLineas[i].seg == -1) {
			pos = i;
			i=cantSegMax;
			}
		}
		tablaLineas[pos].seg = seg;
		tablaLineas[pos].linea = linea;
		tablaLineas[pos].base=0;
		tablaLineas[pos].tamanio = tamanioLinea;

		memcpy(Memoria+tablaSegmentos[seg].base+tablaLineas[pos].base,datalinea,tamanioLinea);

	}
}

char * solicitarLinea ( int posMemoria, int linea){
	int cantSegMax= SEGMENTOS;
	int baseLinea,tamLinea,i;
		for(i=0;i<cantSegMax;i++){
		if (tablaLineas[i].seg == posMemoria && tablaLineas[i].linea == linea) {
		baseLinea = tablaLineas[i].base;
		tamLinea = tablaLineas[i].tamanio;
		i=cantSegMax;
		}
	}
		char * frame = malloc(tamLinea);
		memset(frame,'\0',tamLinea);
		memcpy(frame,Memoria+tablaSegmentos[posMemoria].base+baseLinea,tamLinea);
return frame;
}


int actualizarLinea( int seg, int linea, int tamanioLinea, char* datalinea){
	int cantSegMax= SEGMENTOS;
	int i=0;
	int pos=-1;
	for (i=0; i<cantSegMax;i++){
		if (tablaLineas[i].seg == seg && tablaLineas[i].linea == linea) {
			pos = i;
			i=cantSegMax;
		}
	}
	if ( pos > -1 ) { // La Linea existe
		tablaLineas[pos].tamanio = tamanioLinea;
		//memcpy(Memoria+tablaSegmentos[seg].base+tablaLineas[pos+linea-1].base,datalinea,tamanioLinea);
		memcpy(Memoria+tablaSegmentos[seg].base+tablaLineas[pos].base,datalinea,tamanioLinea);
	return 1;
	}
	else // La Linea NO existe
	{
		puts("LA LINEA NO EXISTE: SEG FAULT");
		return -1;
	}
}

int liberaRecursosMemoria( int pid, int referenciaMemoria){
	int cantSegMax= SEGMENTOS;
	int i=0;
	int existe= -1;
	for (i=0; i<cantSegMax;i++){
			if (tablaSegmentos[i].pid == pid && tablaSegmentos[i].seg == referenciaMemoria) {
				tablaSegmentos[i].pid=-1;
				tablaSegmentos[i].seg =-1;
				tablaSegmentos[i].base =-1;
				tablaSegmentos[i].limite=-1;
				tablaSegmentos[i].estado=0;
				existe = TRUE;
			}
		}

		if ( existe != TRUE) { //No existe la referencia SEGFAULT
			return -1;
		}
		else{
			for (i=0; i<cantSegMax;i++){
				if (tablaLineas[i].seg == referenciaMemoria) {
					tablaLineas[i].seg =-1;
					tablaLineas[i].linea=-1;
					tablaLineas[i].base=-1;
					tablaLineas[i].tamanio=-1;
					}
			}
			return TRUE;
		}
}




void handleConsolaFM9(){


	puts("-_____________________________________________________");
	puts("CONSOLA");
	puts("------ Escriba un comando ------");
	puts("1. - Dump  param1 param2");

	char* linea;

	while (1) {
		linea = readline("\nSharks: ");

		if (strcmp(linea, "exit")==0){
			free(linea);
			puts("EXIT.");
			break;
		}

		if (ejecutar_linea(linea)){
			add_history(linea);
		}
		free(linea);
	}
}


int ejecutar_linea (char * linea){
	char * linea_aux = string_duplicate(linea);
	int i = 0;
	char * funcion;

	if(string_contains(linea, " "))
	{
			while (linea_aux[i] != ' ') i++;

			funcion = malloc((sizeof(char) * i) + 1);
			strncpy(funcion, linea_aux, i);
			funcion[i] = '\0';
	}else{
			funcion = string_duplicate(linea_aux);
	}

	COMANDO * comando = buscar_comando(funcion);

	char** args = string_split(linea_aux, " ");
	if (!comando) {
		printf ("%s: el comando ingresado es incorrecto.", funcion);
		return (-1);
	}

	free(funcion);
	free(linea_aux);
	//Llamo a la función
	(*(comando->funcion)) (args);
	free(args[0]);
	free(args[1]);

	free(args);
	return 1;
}



COMANDO * buscar_comando(char* linea) {
	int i;

	for (i = 0; comandos[i].nombre; i++){
		// Paso a miniscula y comparo
		string_to_lower(linea);
		char* comando = string_duplicate(comandos[i].nombre);
		string_to_lower(comando);

		if (strcmp (linea, comando) == 0){
			free(comando);
			return (&comandos[i]);
		}
		free(comando);

	}
	return ((COMANDO *) NULL);
}




void Dump(char** arg){
	logInfo("Operacion Dump ");


}


















