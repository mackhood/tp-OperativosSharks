#include "sockets.h"

// *******************************
// *           Servidor			 *
// *******************************
int newSocket() {
	int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (fileDescriptor == FAIL){
		perror("Error : funcion socket");
		return(-1);
	}

	return fileDescriptor;
}

struct sockaddr_in asociarSocket(int sockfd, int puerto) {
	struct sockaddr_in miDireccion;
	miDireccion.sin_family = AF_INET;
	miDireccion.sin_port = htons(puerto);
	//miDireccion.sin_addr.s_addr = 0;
	miDireccion.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY); // Usa mi direccion IP
	memset(&(miDireccion.sin_zero), '\0', 8);

	if( bind(sockfd, (struct sockaddr *) &miDireccion,sizeof(struct sockaddr)) == -1){
		perror("Error :  funcion bind");
		return(miDireccion);
	};
	//char* ip=inet_ntoa(miDireccion.sin_addr);
	//printf("%s \n",ip);
	return miDireccion;
}

void escucharSocket(int sockfd, int conexionesEntrantesPermitidas) {
	int funcionListen = listen(sockfd, conexionesEntrantesPermitidas);

	if ( funcionListen == FAIL){
		perror("Error : funcion listen");
		//return(-1);
	}
}

/**
 * La secuencia para la configuracion es
 * 1 . socket()
 * 2 . bind()
 * 3 . listen()
 * 4 . accept()  ; // esta se utiliza en un while,
 */
int initSocketNConexiones(int puerto, int cant_max_conexiones){
	int socket;

	socket = newSocket();
	asociarSocket(socket, puerto);
	escucharSocket(socket, cant_max_conexiones);

	puts("socket a la espera de conexiones entrantes");

	return socket;
}

// *******************************
// *           Cliente			 *
// *******************************

int conectarCliente(char* ipDestino, int puerto){
	int socket;
	socket = newSocket();

	struct sockaddr_in datosServidor;
	datosServidor.sin_family = AF_INET;
	datosServidor.sin_port = htons(puerto);
	datosServidor.sin_addr.s_addr = inet_addr(ipDestino);
	memset(&(datosServidor.sin_zero), '\0', 8);

	int connectFunction = connect(socket,(struct sockaddr *)&datosServidor, sizeof(datosServidor));

	if ( connectFunction != 0){
		perror("Error al conectar el cliente");
		return(-1);
	}

	puts("Conexion establecida..");
	return socket;
}
// *******************************
// *       Enviar y Recibir		 *
// *******************************

int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio) {
	int bytes_enviados;
	int total = 0;

	while (total < tamanio) {
		bytes_enviados = send(fdCliente, mensaje + total, tamanio, 0);
		if (bytes_enviados == FAIL) {
			break;
		}
		total += bytes_enviados;
		tamanio -= bytes_enviados;
	}
	if (bytes_enviados == FAIL){
		puts("Error al enviar por socket");
	}

	return bytes_enviados;
}

int recibirPorSocket(int fdCliente, void * buffer, int tamanio) {
	int total = 0;
	int bytesRecibidos;

	while (total < tamanio) {
		bytesRecibidos = recv(fdCliente, buffer + total, tamanio, 0);
		if (bytesRecibidos == FAIL) {
			// Error
			puts("Funcion recv");
			break;
		}
		if (bytesRecibidos == 0) {
			// Desconexion
			puts("Desconexion");

			break;
		}
		total += bytesRecibidos;
		tamanio -= bytesRecibidos;
	}
	return bytesRecibidos;
}

// *******************************
// *    Cerrar File Descriptor	 *
// *******************************
void cerrarSocket(int sockfd) {
	if (close(sockfd) == FAIL){
		perror("Error :  funcion close");
	}
}

void selectSocket(int cantSocket, fd_set* listaSocket, int intentos) {
	int estado = select(cantSocket + 1, listaSocket, NULL, NULL, NULL);
	//Controlamos el estado
	if(estado==-1){
		if(intentos > 10){
			perror("Error selectSocket");
			exit(EXIT_FAILURE);
		}
		if(errno==EINTR){
			selectSocket(cantSocket,listaSocket,intentos+1);
		}else if(estado == ERROR){
			perror("Error selectSocket");
			exit(EXIT_FAILURE);

		}
	}
}


int aceptarConexionSocket(int sockfd) {
	struct sockaddr_in remoteaddr; // dirección del cliente
	unsigned int addres_size = sizeof(remoteaddr);

	int fdCliente = accept(sockfd, (struct sockaddr*) &remoteaddr, &addres_size);

	if(fdCliente == FAIL){
		perror("Error funcion accept\ns");
	}else{
		printf("Nueva conexion de %s \n", inet_ntoa(remoteaddr.sin_addr));
	}

	return fdCliente;
}
