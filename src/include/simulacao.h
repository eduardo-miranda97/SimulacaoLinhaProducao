#ifndef SIMULACAO
#define  SIMULACAO

#include <iostream>
#include "triangular.h"
#include "vaso.h"

extern double               SM_time_simulation;
extern double               SM_final_time_simulation;
extern u_int16_t            SM_massa;
extern u_int16_t            SM_pedra;
extern u_int16_t            SM_espaco_secagem;
extern times_triangular_t   SM_quatd_vasos;
extern Vaso**               SM_vaso_finish;
extern u_int16_t            SM_vaso_finish_length;

#endif
