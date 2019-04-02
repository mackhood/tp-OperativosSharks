#include <sharksCommons/protocoloSafaDAM.h>
#include "adminColasPlanificacion.h"

void desbloquear_proceso(t_dmv* estructura_dma );
void abortar_by_error(t_dmv* estructura_dma );
void finalizacion_dummy(t_dmv* estructura_dma );
void finalizacion_abrir(t_dmv* estructura_dma );

#define op_abrir "abrir"
#define op_flush "flush"
#define op_crear "crear"
#define op_borrar "borrar"
