#ifndef ACT
#define  ACT

#include <list>
#include "vaso.h"
#include "especialista.h"
#include "atividade.h"
#include "artesao.h"
#include "triangular.h"
#include "simulacao.h"
#include "TAD.h"
// #include "fila.h"


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
void base_clearing();      // EVENTO  := 'LIMPEZA_ACABAMENTO_BASE'

void base_finish();         // EVENTO  := 'SECAGEM_DA_BASE'

void mouth_preparation();  // EVENTO  := 'PREPARA_BOCA'

void mouth_set_init();     // EVENTO  := 'ACABAMENTO_INCIAL_BOCA'

void mouth_set_drying();   // EVENTO  := 'SECAGEM_ACABAMENTO_BOCA'

void mount_clearing();     // EVENTO  := 'LIMPEZA_ACABAMENTO_BOCA'

void mount_drying();       // EVENTO  := 'SECAGEM_DA_BOCA'

void inter_waterpoofing();  // EVENTO  := 'IMPERMEABILIZIÇÃO_INTERNA'

void inter_drying();       // EVENTO  := 'ENVERNIZAÇÃO_GERAL'

void varnishing();

void final_drying();


void preparation_massa();

void preparation_pedras();

#endif
