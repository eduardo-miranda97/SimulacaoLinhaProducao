#include "./include/simulacao.h"

double               SM_final_time_simulation = 60;
double               SM_time_simulation      = 0;
u_int16_t            SM_massa                = 300;
u_int16_t            SM_pedra                = 300;
u_int16_t            SM_espaco_secagem       = 300;
u_int16_t            SM_vaso_finish_length    = 0;
times_triangular_t   SM_quatd_vasos          = {100, 150, 200};
Vaso**               SM_vaso_finish           = NULL;
