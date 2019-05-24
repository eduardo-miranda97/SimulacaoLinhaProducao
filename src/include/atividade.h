#ifndef ACT
#define  ACT

#include "vaso.h"
#include "especialista.h"
#include "artesao.h"
#include "triangular.h"
#include "simulacao.h"
#include "TAD.h"
#include "fila.h"

// SET == ACABAMENTO CARALHO.


void init_simulation();

void begin_requets(void* ptr);       // EVENTO := 'CHEGADA DE PEDIDOS'

void base_preparation(void* ptr);    // EVENTO := 'PREPARA_FORMA'
//
void base_set_init(void* ptr);
//
// void base_set_drying();
//
// void base_clearing();
//
// void base_finish();
//
// void mouth_preparation();
//
// void mouth_set_init();
//
// void mouth_set_drying();
//
// void mount_clearing();
//
// void mount_drying();
//
// void inter_waterpoofing();
//
// void inter_drying();
//
// void varnishing();
//
// void final_drying();

#endif
