#include "./include/simulacao.h"

double               SM_final_time_simulation = 0;
double               SM_time_simulation      = 0;
u_int16_t            SM_massa                = 30;
u_int16_t            SM_massa_MAX            = 300;
u_int16_t            SM_pedra                = 30;
u_int16_t            SM_pedra_MAX            = 300;
u_int16_t            SM_espaco_secagem       = 80;
times_triangular_t   SM_quatd_vasos          = {20, 40, 60};
times_triangular_t   SM_quatd_pedidos        = {21, 30, 37};
u_int16_t            SM_quatd_esp            = 1;
u_int16_t            SM_quatd_art            = 1;
std::list<Vaso*>     SM_vaso_finish;
std::map<std::string, times_triangular_t> SM_times_events;
