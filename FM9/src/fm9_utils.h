#ifndef FM9_SOCKETS_
#define FM9_SOCKETS_

int iniciar_socket_cliente(char*, int, int*);
int iniciar_socket_server(char*, int, int*);
int escuchar_conexiones(int, int*);
int aceptar_conexion(int, int*);
int enviar_f(int, char*, int*);
char *recibir_f(int, int*);
void cerrar_conexion(int);

#endif /* FM9_SOCKETS_ */
