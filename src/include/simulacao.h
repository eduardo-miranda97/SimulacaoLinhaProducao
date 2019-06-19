#ifndef SIMULACAO
#define  SIMULACAO

#include <map>
#include <fstream>
#include "atividade.h"

#define LOG  true

extern std::map<std::string, times_triangular_t> SM_times_events;
extern std::list<Vaso*>                          SM_vaso_finish;
extern std::ofstream                             file_log;
extern double                                    SM_time_simulation;
extern double                                    SM_final_time_simulation;
extern double                                    SM_massa_MIN;
extern double                                    SM_pedra_MIN;
extern u_int16_t                                 SM_massa;
extern u_int16_t                                 SM_massa_MAX;
extern u_int16_t                                 SM_massa_usado;
extern u_int16_t                                 SM_pedra;
extern u_int16_t                                 SM_pedra_MAX;
extern u_int16_t                                 SM_pedra_usado;
extern u_int16_t                                 SM_espaco_secagem;
extern u_int16_t                                 SM_quatd_art;
extern u_int16_t                                 SM_quatd_esp;
extern times_triangular_t                        SM_quatd_pedidos;
extern times_triangular_t                        SM_quatd_vasos;

void start_simulation();

#endif
