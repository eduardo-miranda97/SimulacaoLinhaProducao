#include "./include/simulacao.h"

double               SM_final_time_simulation =   0;
double               SM_time_simulation      =   0;
double               SM_massa_MIN            =   4;
double               SM_pedra_MIN            =   4;
double               SM_quatd_esp            =   2;
double               SM_quatd_art            =   2;
double               SM_massa                =  10;
double               SM_massa_MAX            =  10;
double               SM_pedra                =  10;
double               SM_pedra_MAX            =  10;
double               SM_espaco_secagem       =  10;
double               SM_pedra_usado          =   0;
double               SM_massa_usado          =   0;
times_triangular_t   SM_quatd_vasos          = {20, 40, 60};
times_triangular_t   SM_quatd_pedidos        = {21, 30, 37};
std::map<std::string, times_triangular_t>    SM_times_events;
std::list<Vaso*>                             SM_vaso_finish;
std::ofstream                                file_log;

using namespace std;

void start_simulation(){

  for (u_int16_t i = 0; i < SM_quatd_art; i++) {
      Artesao* artesao = new Artesao;
      artesao->add_art();
  }

  for (u_int16_t i = 0; i < SM_quatd_esp; i++) {
      Especialista* especialista = new Especialista;
      especialista->add_esp();
  }

  init_simulation();
  while ((SM_list_event_simulation)&&(SM_time_simulation <= SM_final_time_simulation)){
      SM_list_event_simulation[0].event.funct_event();
  }

  #if LOG
      file_log.close();
  #endif

  for (Artesao* artesao: SM_Artesao){
      if (artesao->get_situation()==state_art::OCIOSITY_ART)
          artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
  }

  for (Especialista* especialista: SM_Especialista){
      if (especialista->get_situation()==state_esp::OCIOSITY_ESP)
          especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
  }

}
