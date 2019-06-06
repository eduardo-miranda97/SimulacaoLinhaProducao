#ifndef SIMULACAO
#define  SIMULACAO

#include <iostream>
#include <cstdlib>
#include <list>
#include <map>
#include "triangular.h"
#include "vaso.h"

extern std::map<std::string, times_triangular_t> SM_times_events;
extern double               SM_time_simulation;
extern double               SM_final_time_simulation;
extern u_int16_t            SM_massa;
extern u_int16_t            SM_massa_MAX;
extern u_int16_t            SM_pedra;
extern u_int16_t            SM_pedra_MAX;
extern u_int16_t            SM_espaco_secagem;
extern std::list<Vaso*>     SM_vaso_finish;
extern times_triangular_t   SM_quatd_vasos;
extern u_int16_t            SM_quatd_art;
extern times_triangular_t   SM_quatd_pedidos;
extern u_int16_t            SM_quatd_esp;

#endif
