#include "dam.h"




void desbloquear_proceso(t_dmv* estructura_dma ){

	DTB_SAFA* dtb =	get_elem_block_by_id( estructura_dma->id );
	dtb->cant_usaron_diego++; //uso al diego
	if(dtb==NULL){
		puts("Este dtb ya no esta en cola de bloqueado..");
	}else {
		moverBlockToReady(dtb);
	}


}

void finalizacion_abrir(t_dmv* estructura_dma  ){
	DTB_SAFA* dtb =	get_elem_block_by_id( estructura_dma->id );

	if(dtb==NULL){
		puts("Este dtb ya no esta en cola de bloqueado..");
	}else {
		list_add(dtb->tablaDeArchivosAbiertos,  crear_archivo_abierto(estructura_dma->path, estructura_dma->posMemoria) );
		moverBlockToReady(dtb);
	}


}


void finalizacion_dummy(t_dmv* estructura_dma  ){
	DTB_SAFA* dtb =	get_elem_penready_by_id( estructura_dma->id );

	if(dtb!=NULL){

		dtb->posMemoria = estructura_dma->posMemoria;
		dtb->sentencias = malloc( string_length(estructura_dma->lineas) + 1 );
		memcpy(dtb->sentencias, estructura_dma->lineas,  string_length(estructura_dma->lineas) + 1);
		dtb->total_sentencias = estructura_dma->cant_lineas;

		char** line_by_line = string_split( estructura_dma->lineas, "\n");

		for(int i=0; line_by_line[i]!=NULL;i++){

			if( string_contains(line_by_line[i], op_flush) || string_contains(line_by_line[i], op_crear) ||
			   string_contains(line_by_line[i], op_borrar) || string_contains(line_by_line[i], op_abrir) ){
				queue_push(dtb->prox_io, (int*)i);
			}

			free( line_by_line[i] );
		}
		free(line_by_line);
		dtb->siguiente_io =  (int)queue_pop(dtb->prox_io);
		moverPendienteReadyToReady( dtb );

	}else {
		logError("Este dtb ya no esta en cola de pendiente de ready..");
	}



}

void abortar_by_error(t_dmv* estructura_dma ){

	DTB_SAFA* dtb =	get_elem_block_by_id( estructura_dma->id );
	moverBlockToExit( dtb );
	logError("Error: %s",  estructura_dma->lineas);

}
