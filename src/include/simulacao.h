#ifndef SIMULACAO
#define  SIMULACAO

#include <map>
#include <fstream>
#include "atividade.h"

#define LOG  false

extern std::map<std::string, times_triangular_t> SM_times_events;
extern std::list<Vaso*>                          SM_vaso_finish;
extern std::ofstream                             file_log;
extern double                                    SM_time_simulation;
extern double                                    SM_final_time_simulation;
extern double                                    SM_massa_MIN;
extern double                                    SM_pedra_MIN;
extern double                                    SM_massa;
extern double                                    SM_massa_MAX;
extern double                                    SM_massa_usado;
extern double                                    SM_pedra;
extern double                                    SM_pedra_MAX;
extern double                                    SM_pedra_usado;
extern double                                    SM_espaco_secagem;
extern double                                    SM_quatd_art;
extern double                                    SM_quatd_esp;
extern times_triangular_t                        SM_quatd_pedidos;
extern times_triangular_t                        SM_quatd_vasos;

void start_simulation();

#endif
