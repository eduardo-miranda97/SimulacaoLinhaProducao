#include "./include/atividade.h"
#include "./include/input.h"
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
    srand(time(NULL));

    SM_time_simulation = 0;
    SM_final_time_simulation = 150*24*60;

    Artesao a0;
    Artesao a1;
    Artesao a2;
    Artesao a3;
    Artesao a4;
    Artesao a5;

    a0.add_art();
    a1.add_art();
    a2.add_art();
    a3.add_art();
    a4.add_art();
    a5.add_art();

    Especialista e0;
    Especialista e1;
    Especialista e2;
    Especialista e3;
    Especialista e4;
    Especialista e5;

    e0.add_esp();
    e1.add_esp();
    e2.add_esp();
    e3.add_esp();
    e4.add_esp();
    e5.add_esp();

    input_args("config.cfg");
    init_simulation();
    while ((SM_list_event_simulation)&&(SM_time_simulation <= SM_final_time_simulation)){
        SM_list_event_simulation[0].event.funct_event();
    }

    for(int i=0;i<QUEUE_WAIT;i++){
        std::cout << "FILA ["<< i << "] = " << SM_queue_vasos[i].size() << '\n';
    }

    std::cout << '\n';

    std::cout << "Time atual: " << SM_time_simulation << '\n';
    std::cout << "Time Final: " << SM_final_time_simulation << '\n';
    std::cout << "Vasos Gerados: " << last_id_vaso << '\n';
    std::cout << "Vasos Finalizados: " << SM_vaso_finish.size() << '\n';
    printf("Massa: %ld, Pedra: %ld, Espaço: %ld\n", SM_massa, SM_pedra, SM_espaco_secagem);
    return 0;
}
