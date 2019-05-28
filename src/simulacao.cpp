#include "./include/simulacao.h"

double               SM_final_time_simulation = 0;
double               SM_time_simulation      = 0;
u_int16_t            SM_massa                = 300;
u_int16_t            SM_massa_MAX            = 300;
u_int16_t            SM_pedra                = 300;
u_int16_t            SM_pedra_MAX            = 300;
u_int16_t            SM_espaco_secagem       = 300;
u_int16_t            SM_vaso_finish_length    = 0;
times_triangular_t   SM_quatd_vasos          = {300, 350, 400};
// Vaso**               SM_vaso_finish           = NULL;
std::list<Vaso*>     SM_vaso_finish;
