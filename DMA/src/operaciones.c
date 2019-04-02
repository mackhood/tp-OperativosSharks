#include "operaciones.h"

void operacion_abrir(char* datos_cpu, int accion_to_safa){

	DTB_DAM* e_dam = deszerializarDTBDAM(datos_cpu);
	int pid = e_dam->id;
	int base = 0;
	E_MDJ* i_to_mdj = crearE_MDJ(CABECERA_DMA , 0, base, dma_struct->t_size, e_dam->path, "", ABRIR_ARCHIVO, 0, 0);
	char* i_datos_to_mdj = serializar_E_MDJ(i_to_mdj);
	destruir_E_MDJ(i_to_mdj);

	pthread_mutex_lock(&mutexMdj);//ningun hilo se puede hacer el vivo


	if(enviar(fd_mdj, i_datos_to_mdj) ==-1){
		logError("Error al enviar datos a mdj");
		free(i_datos_to_mdj);
		return;
	}
	free(i_datos_to_mdj);


	char* buffer_total = strdup("");

	/**
	 * datos primer mensaje enviado de mdj
	 */
	char* mensaje_i_mdj = recibir(fd_mdj, TAM_RECIBIR);
	pthread_mutex_unlock(&mutexMdj);

	E_MDJ* i_from_mdj= deserializar_E_MDJ(mensaje_i_mdj);
	free( mensaje_i_mdj );

	int tam_buffer_inicial = (sizeof(char)*string_length( i_from_mdj->buffer )) + 1;
	char* buffer_inicial = malloc( tam_buffer_inicial );
	memcpy(buffer_inicial, i_from_mdj->buffer, tam_buffer_inicial );

	int tam_archivo = i_from_mdj->tamArchivo;
	int cod_mdj = i_from_mdj->clave;
	int cant = tam_archivo/dma_struct->t_size;
	int resto = (tam_archivo%dma_struct->t_size?1:0);
	int cantidadSolicitudes = cant + resto -1 ; //le resto uno, debido a que el primer pedido ya se hizo

	if( cod_mdj != PATH_INEXISTENTE){
		string_append(&buffer_total, buffer_inicial);

		pthread_mutex_lock(&mutexMdj);
		for(int i = cantidadSolicitudes; cantidadSolicitudes!=0; i++ ){

			E_MDJ* to_mdj = crearE_MDJ(CABECERA_DMA, 0, base, dma_struct->t_size, e_dam->path, "", ABRIR_ARCHIVO, 0, 0);
			char* datos_to_mdj = serializar_E_MDJ(to_mdj);
			destruir_E_MDJ(to_mdj);
			if(enviar(fd_mdj, datos_to_mdj) ==-1){
				logError("Error al enviar datos a mdj");
				free(datos_to_mdj);
				break;
			}
			free(datos_to_mdj);

			char* datos_from_mdj = recibir(fd_mdj, TAM_RECIBIR);
			E_MDJ* from_mdj= deserializar_E_MDJ(datos_from_mdj);
			free( datos_from_mdj );

			string_append(&buffer_total, from_mdj->buffer);
			destruir_E_MDJ( from_mdj );
			base += dma_struct->t_size;


		}
		pthread_mutex_unlock(&mutexMdj);
	}

	printf("\n mensaje recibido %s \n", buffer_total);


	/***********divide en lineas los datos para memoria *********************/
	char b[1];
		b[0] = '\n';
	char** lineas =  string_split(buffer_total, "\n");
	int i=0;
	for(i=0; lineas[i]!=NULL;i++){
		char* linea = malloc(string_length( lineas[0] )+2);
		memcpy(linea, lineas[0], string_length( lineas[0] ) + 1 );
		string_append(&linea, b);
		lineas[i] = malloc(string_length( linea )+1);
		memcpy(lineas[i], linea, string_length( linea ) + 1 );
		free(linea);
	}

	int cant_lineas = i;
	char* datos_from_fm9;
	E_FM9* e_from_fm9;

	t_dmv* mv;
	char* datos_to_safa;

	int pos = 0;

	//errores
	char* error_path = string_itoa(PATH_INEXISTENTE);
	char* error_no_espacio = string_itoa(ESPACIO_INSUFICIENTE);

	switch( cod_mdj ){
		case OPERACION_MDJ_VALIDA:
			pthread_mutex_lock(&mutexFm9);
			for(int j=cant_lineas; cant_lineas!=0 ;j--){
				char* linea_to_fm9 = lineas[pos];
				E_FM9* e_to_fm9 =  crear_e_fm9(CABECERA_DMA, GUARDAR_DATOS_MEMORIA, pid,
						e_dam->path, 0, cant_lineas, linea_to_fm9);

				char* each_to_fm9 = serializar_e_fm9( e_to_fm9 );
				destroy_e_fm9( e_to_fm9 );
				if( enviar(fd_fm9, each_to_fm9)==-1){
					logError("Error al enviar datos a fm9");
					free( linea_to_fm9 );
					free( each_to_fm9 );
					break;
				}

				free( linea_to_fm9 );
				free( each_to_fm9 );
				pos++;
			}

			datos_from_fm9 =  recibir(fd_fm9,TAM_RECIBIR);
			e_from_fm9 = deserializar_e_fm9(datos_from_fm9);
			free(datos_from_fm9);
			pthread_mutex_unlock(&mutexFm9);

			switch( e_from_fm9->cod ){
				case GUARDAR_DATOS_OK:
					mv = create_t_dmv(CABECERA_DMA, accion_to_safa, pid,
							e_from_fm9->posMemoria, cant_lineas, buffer_total, e_dam->path);
					datos_to_safa = serializar_t_dmv(mv);
					pthread_mutex_lock(&mutexSafa);
					if( enviar(fd_safa, datos_to_safa)==-1)
						logError("Error al enviar datos a SAFA");

					pthread_mutex_unlock(&mutexSafa);
					break;
				case GENERIC_ERROR:
				mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, error_no_espacio, e_dam->path);
				datos_to_safa = serializar_t_dmv(mv);
				pthread_mutex_lock(&mutexSafa);
				if(enviar(fd_safa, datos_to_safa)==-1)
					logError("Error al enviar datos a SAFA");

				pthread_mutex_unlock(&mutexSafa);
				break;

			}
			break;
		case PATH_INEXISTENTE:
			mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, error_path,e_dam->path);
			datos_to_safa = serializar_t_dmv(mv);
			pthread_mutex_lock(&mutexSafa);
			if(enviar(fd_safa, datos_to_safa)==-1)
				logError("Error path inexistente");
			pthread_mutex_unlock(&mutexSafa);
			break;
	}

	free( error_no_espacio );
	free(error_path);
	free( datos_to_safa );
	destruir_E_MDJ(i_from_mdj);
	destroy_t_dmv(mv);
	destroy_e_fm9(e_from_fm9);
	destruir_DTBDAM(e_dam);
}
void operacion_flush(char* mensaje_recibido_cpu){
	DTB_DAM* e_dam = deszerializarDTBDAM(mensaje_recibido_cpu);
	//bool errors = false;
	//estructura de pedidos
	E_FM9* initial_struct_to_fm9 = crear_e_fm9(CABECERA_DMA , PEDIR_DATOS_MEMORIA,
			e_dam->id, e_dam->path,e_dam->posMemoria ,0, "");

	char* initial_data_to_fm9 =  serializar_e_fm9(initial_struct_to_fm9);

	pthread_mutex_lock(&mutexFm9);
	if(enviar(fd_fm9, initial_data_to_fm9) ==-1){
		logError("Error al enviar datos a fm9");
		free(initial_data_to_fm9);
		destroy_e_fm9(initial_struct_to_fm9);
	}
	free(initial_data_to_fm9);
	destroy_e_fm9(initial_struct_to_fm9);

	char* total_buffer_linea = strdup("");

	char* mensaje_from_fm9 = recibir(fd_fm9, TAM_RECIBIR);
	pthread_mutex_unlock(&mutexFm9);
	E_FM9* initial_struct_from_fm9 = deserializar_e_fm9(mensaje_from_fm9);
	free(mensaje_from_fm9);

	if( initial_struct_from_fm9->cod !=GENERIC_ERROR ){
		int line_amount = initial_struct_from_fm9->cant_lineas;
		int initial_line_size = (sizeof(char)*string_length( initial_struct_from_fm9->linea )) + 1;
		char* initial_line = malloc( initial_line_size );
		memcpy(initial_line, initial_struct_from_fm9->linea, initial_line_size );
		string_append(&total_buffer_linea, initial_line);

		pthread_mutex_lock(&mutexFm9);
		for(int i=0; line_amount > i ;i++){

			E_FM9* each_request = crear_e_fm9(CABECERA_DMA ,
					PEDIR_DATOS_MEMORIA, e_dam->id, e_dam->path,e_dam->posMemoria ,0, "");

			char* serialize_resquest = serializar_e_fm9(each_request);
			if(enviar(fd_mdj, serialize_resquest) ==-1)
				logError("Error al enviar datos a fm9");

			free( serialize_resquest );
			destroy_e_fm9(each_request);

			char* each_fm9_response = recibir(fd_fm9, TAM_RECIBIR);
			E_FM9* each_struct_response = deserializar_e_fm9( each_fm9_response );
			free( each_fm9_response );

			if( each_struct_response->cod==GENERIC_ERROR ){
				destroy_e_fm9(each_struct_response);
				break;
			}
			string_append(&total_buffer_linea, each_struct_response->linea);
			destroy_e_fm9(each_struct_response);
		   }
		pthread_mutex_unlock(&mutexFm9);
	}


		logInfo("Mensaje recibido de fm9 %s", total_buffer_linea);

	//	variables necesarias
		char* datos_from_mdj;
		char* datos_to_safa;
		int base = 0;
		int buffer_size = string_length(total_buffer_linea);
		int cant_sends = buffer_size/dma_struct->t_size + ( buffer_size%dma_struct->t_size?1:0);
		t_dmv* mv;

		/**
		 * errores operacion flush
		 */
		char* arch_no_open = string_itoa(ARCH_NO_ABIERTO);
		char* sin_espacio = string_itoa(SIN_ESPACIO_MDJ);
		char* sin_arch = string_itoa(SIN_ARCHIVO);
		char* fallo_memoria = string_itoa(FALLO_MEMORIA);

		switch( initial_struct_from_fm9->cod){
			case PEDIR_DATOS_MEMORIA:
				pthread_mutex_lock(&mutexMdj);
				for(int i=cant_sends; i!=0; i-- ){

					char* each_buffer = string_substring(total_buffer_linea, base, dma_struct->t_size);
					E_MDJ* each_struct_to_mdj = crearE_MDJ(CABECERA_DMA ,0, base, dma_struct->t_size,
					e_dam->path, each_buffer, GUARDAR_DATOS, 0, 0);
					char* datos_to_mdj = serializar_E_MDJ( each_struct_to_mdj );

					if( enviar(fd_mdj, datos_to_mdj)==-1){
						logError("Error al enviar datos a mdj");
						break;
					}
					base+=dma_struct->t_size;

				}

				datos_from_mdj =  recibir(fd_mdj,TAM_RECIBIR);
				pthread_mutex_unlock(&mutexMdj);
				switch(atoi(get_codigo(datos_from_mdj))){
					case OPERACION_MDJ_VALIDA:
						mv = create_t_dmv(CABECERA_DMA ,VALID, e_dam->id, initial_struct_from_fm9->posMemoria, 0,"",e_dam->path); //ese dos sera tempora la posta la tiene que enviar memoria
						datos_to_safa = serializar_t_dmv(mv);
						pthread_mutex_lock(&mutexSafa);
						if( enviar(fd_safa, datos_to_safa)==-1)
							logError("Error al enviar datos a SAFA");
						pthread_mutex_unlock(&mutexSafa);
						break;
					case ARCH_NO_ABIERTO:
						mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, arch_no_open,e_dam->path);
						datos_to_safa = serializar_t_dmv(mv);
						pthread_mutex_lock(&mutexSafa);
						if(enviar(fd_safa, datos_to_safa)==-1)
							logError("Error al enviar datos a SAFA");
						pthread_mutex_unlock(&mutexSafa);

					break;

					case SIN_ESPACIO_MDJ:
						mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, sin_espacio, e_dam->path);
						datos_to_safa = serializar_t_dmv(mv);
						pthread_mutex_lock(&mutexSafa);
						if(enviar(fd_safa, datos_to_safa)==-1)
							logError("Error al enviar datos a SAFA");
						pthread_mutex_unlock(&mutexSafa);
					break;

					case SIN_ARCHIVO:
						mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, sin_arch, e_dam->path);
						datos_to_safa = serializar_t_dmv(mv);
						pthread_mutex_lock(&mutexSafa);
						if(enviar(fd_safa, datos_to_safa)==-1)
							logError("Error al enviar datos a SAFA");
						pthread_mutex_unlock(&mutexSafa);
						break;

				}
				break;
			case ARCH_NO_ABIERTO:
				mv = create_t_dmv(CABECERA_DMA ,ERROR, e_dam->id, 0, 0, fallo_memoria, e_dam->path);
				datos_to_safa = serializar_t_dmv(mv);
				pthread_mutex_lock(&mutexSafa);
				if(enviar(fd_safa, datos_to_safa)==-1)
					logError("fallo segmento memoria");
				pthread_mutex_unlock(&mutexSafa);
				break;

		}
}

void operacion_crear(char* mensaje_recibido_cpu){


	DTB_DAM* e_dam = deszerializarDTBDAM(mensaje_recibido_cpu);
	E_MDJ* crear_t = crearE_MDJ(CABECERA_DMA ,0, 0, 0, e_dam->path, "", CREAR_ARCHIVO, e_dam->lineas, 0);
	char* datos_to_mdj = serializar_E_MDJ(crear_t);

	pthread_mutex_lock(&mutexMdj);
	if( enviar(fd_mdj, datos_to_mdj)==-1){
		logError("Error al enviar datos a mdj");
		return;
	}
	char* datos_recibidos = recibir(fd_mdj,TAM_RECIBIR);
	E_MDJ* mdj_recibido = deserializar_E_MDJ(datos_recibidos);
	free( datos_recibidos );
	pthread_mutex_unlock(&mutexMdj);

	//errores
	char* arch_exist = string_itoa(ARCHIVO_EXISTENTE);
	char* sin_espacio = string_itoa(SIN_ESPACIO_CREAR);

	pthread_mutex_lock(&mutexSafa);
	switch( mdj_recibido->clave ){
			//caso feliz
			case OPERACION_MDJ_VALIDA: {
				t_dmv* mv = create_t_dmv(CABECERA_DMA, CREAR_ARCHIVO_SAFA, e_dam->id, 0, 0, "",e_dam->path);//aqui
				char* datos_to_safa = serializar_t_dmv(mv);
				if( enviar(fd_safa, datos_to_safa)==-1)
					logError("Error al enviar datos a SAFA");
				destroy_t_dmv( mv );
				free( datos_to_safa );
				}
				break;
			//archivo ya existe
			case ARCHIVO_EXISTENTE:{
					t_dmv* mv = create_t_dmv(CABECERA_DMA, GENERIC_ERROR, e_dam->id, 0, 0, arch_exist, "");//aqui
					char* datos_to_safa = serializar_t_dmv(mv);
					if( enviar(fd_safa, datos_to_safa)==-1)
						logError("Error al enviar datos a SAFA");
					destroy_t_dmv( mv );
					free( datos_to_safa );
				}
				break;
			//espacio insuficiente
			case SIN_ESPACIO_CREAR:{
					t_dmv* mv = create_t_dmv(CABECERA_DMA, GENERIC_ERROR, e_dam->id, 0, 0, sin_espacio, "");//aqui
					char* datos_to_safa = serializar_t_dmv(mv);
					if( enviar(fd_safa, datos_to_safa)==-1)
						logError("Error al enviar datos a SAFA");
					destroy_t_dmv( mv );
					free( datos_to_safa );
			  	}
				break;
	}
	pthread_mutex_unlock(&mutexSafa);

	free( datos_to_mdj );
	free(arch_exist);
	free(sin_espacio);
	destruir_E_MDJ(crear_t);

	destruir_DTBDAM(e_dam);
}

void operacion_borrar(char* mensaje_recibido_cpu){

	DTB_DAM* e_dam = deszerializarDTBDAM(mensaje_recibido_cpu);
	E_MDJ* borrar_t = crearE_MDJ(CABECERA_DMA, 0, 0, 0, e_dam->path, "", BORRAR_ARCHIVO,0,0);
	char* datos_to_mdj = serializar_E_MDJ(borrar_t);

	pthread_mutex_lock(&mutexMdj);
	if( enviar(fd_mdj, datos_to_mdj)==-1){
		logError("Error al enviar datos a mdj");
		return;
	}
	char* datos_recibidos =  recibir(fd_mdj, TAM_RECIBIR);
	E_MDJ* mdj_recibido = deserializar_E_MDJ(datos_recibidos);

	pthread_mutex_unlock(&mutexMdj);

	//error
	char* file_inexist = string_itoa(FILE_INEXISTENTE);

	pthread_mutex_lock(&mutexSafa);
	switch( mdj_recibido->clave ){
			//caso feliz
			case OPERACION_MDJ_VALIDA:{
				t_dmv* mv = create_t_dmv(CABECERA_DMA, BORRAR_ARCHIVO_SAFA, e_dam->id, 0, 0,"", e_dam->path);//aqui
				char* datos_to_safa = serializar_t_dmv(mv);
				if( enviar(fd_safa, datos_to_safa)==-1)
					logError("Error al enviar datos a SAFA");
				free( datos_to_safa );
				destroy_t_dmv(mv);
			}
				break;
			case FILE_INEXISTENTE:{
				//archivo ya existe
				t_dmv* mv = create_t_dmv(CABECERA_DMA, GENERIC_ERROR, e_dam->id, 0, 0, file_inexist, "");//aqui
				char* datos_to_safa = serializar_t_dmv(mv);
				if( enviar(fd_safa, datos_to_safa)==-1 )
					logError("Error al enviar datos a SAFA");
				free( datos_to_safa );
				destroy_t_dmv(mv);
			}

				break;
	}
	pthread_mutex_unlock(&mutexSafa);
	free(datos_recibidos);
	free(file_inexist);
	free(datos_to_mdj);
	destruir_DTBDAM(e_dam);
	destruir_E_MDJ(borrar_t);
	destruir_E_MDJ(mdj_recibido);

}

