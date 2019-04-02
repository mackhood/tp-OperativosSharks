 /*============================================================================
 Name        : conect.h
 Author      : Cam Palomino
 Version     :
 Copyright   : Todos los derechos reservados (c) 2018 CamPalomino
 Description : Hello World in C
 ============================================================================+*/


#ifndef CONECT_H_
#define CONECT_H_

//Solicitar Linea C07|PID|POSMEMORIA|NROLINEA
typedef struct {
	char* cabecera;
	int cod;
	int id;
	int posMemoria;
	int tamanio;
	int lineas;
	char* path;
	void* data;

} t_paqueteCPU;

char* solicitarSentencia(int unaLinea);
t_paqueteCPU* deszerializarCPU(char* buffer);
char* serializarCPU(t_paqueteCPU* object);
t_paqueteCPU* crearCPU(char* cabecera, int codigo, int idGDT, int posMemoria,
		int cantidad_linea);
t_paqueteCPU* recibirCPU(int socket);
void conectar_Safa();
void conectar_DMA();
int conectar_fm9();
void enviar_handshake_cpu(int socket_cpu, int cod);

#endif /* CONECT_H_ */
