/*
 * protocolo_FM9.h
 *
 *  Created on: 10/11/2018
 *      Author: utnso
 */

#ifndef PROTOCOLO_FM9_H_
#define PROTOCOLO_FM9_H_

/* Boolean*/

#define TRUE 1

/** cabeceras**/
#define CABECERA_CPU "C"
#define CABECERA_FM9 "F"
#define CABECERA_DMA "D"



/* Handshakes entre procesos */
#define HANDSHAKE_FM9	"F00"

/*---------------ERRORES ESPERADOS ---------------*/
#define ESPACIO_INSUFICIENTE	10002	// Abrir:  Espacio insuficiente en FM9
#define FALLO_SEGMENTO			20002	// Asignar:Fallo de segmento/memoria.
#define SIN_ESPACIO_FM9			20003	// Asignar:Espacio insuficiente en FM9.
#define FALLO_MEMORIA			30002	// Flush:  Fallo de segmento/memoria.
#define FAIL_MEMORIA			40002	// Close:  Fallo de segmento/memoria.

/*---------------OPERACIONES DE FM9---------------*/
#define ABRIR_ARCHIVO		03
#define	ASIGNAR_LINEA		04
#define CERRAR_ARCHIVO		05
#define LEER_LINEA_ARCHIVO	06
#define SOLICITAR_LINEA		07

#endif /* PROTOCOLO_FM9_H_ */
