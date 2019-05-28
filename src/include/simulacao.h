#ifndef SIMULACAO
#define  SIMULACAO

#include <iostream>
#include <list>
#include "triangular.h"
#include "vaso.h"

extern double               SM_time_simulation;
extern double               SM_final_time_simulation;
extern u_int16_t            SM_massa;
extern u_int16_t            SM_massa_MAX;
extern u_int16_t            SM_pedra;
extern u_int16_t            SM_pedra_MAX;
extern u_int16_t            SM_espaco_secagem;
// extern u_int16_t            SM_vaso_finish_length;
// extern Vaso**               SM_vaso_finish;
extern std::list<Vaso*>            SM_vaso_finish;
extern times_triangular_t   SM_quatd_vasos;

#endif
