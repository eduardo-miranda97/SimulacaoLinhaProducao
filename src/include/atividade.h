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

void begin_requets();       // EVENTO := 'CHEGADA DE PEDIDOS'

void form_preparation();    // EVENTO := 'PREPARA_FORMA'

void base_preparation();    // EVENTO := 'PREPARA_BASE'
//
void base_set_init();       // EVENTO := 'ACABAMENTO_INCIAL_BASE'
//
void base_set_drying();    // EVENTO  := 'SECAGEM_ACABAMENTO_BASE'
//
void base_clearing();

void base_finish();

void mouth_preparation();

void mouth_set_init();

void mouth_set_drying();

void mount_clearing();

void mount_drying();

void inter_waterpoofing();

void inter_drying();

void varnishing();

void final_drying();


void preparation_massa();

void preparation_pedras();

#endif
