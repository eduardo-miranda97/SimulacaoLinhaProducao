#include "./include/simulacao.h"

double               SM_final_time_simulation = 0;
double               SM_time_simulation      = 0;
u_int16_t            SM_quatd_esp            = 2;
u_int16_t            SM_quatd_art            = 2;
u_int16_t            SM_massa                = 10;
u_int16_t            SM_massa_MAX            = 10;
u_int16_t            SM_pedra                = 10;
u_int16_t            SM_pedra_MAX            = 10;
u_int16_t            SM_espaco_secagem       = 10;
times_triangular_t   SM_quatd_vasos          = {20, 40, 60};
times_triangular_t   SM_quatd_pedidos        = {21, 30, 37};
std::map<std::string, times_triangular_t>    SM_times_events;
std::list<Vaso*>                             SM_vaso_finish;

using namespace std;

void start_simulation(){

  for (u_int16_t i = 0; i < SM_quatd_art; i++) {
      Artesao artesao;
      artesao.add_art();
  }

  for (u_int16_t i = 0; i < SM_quatd_esp; i++) {
      Especialista especialista;
      especialista.add_esp();
  }

  init_simulation();
  while ((SM_list_event_simulation)&&(SM_time_simulation <= SM_final_time_simulation)){
      SM_list_event_simulation[0].event.funct_event();
  }
}
