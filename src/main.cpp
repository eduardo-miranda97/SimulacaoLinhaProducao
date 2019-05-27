#include <time.h>
#include "./include/fila.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SM_queue_vasos = (Vaso***) malloc(sizeof(Vaso**)*QUEUE_WAIT);
    SM_time_simulation = 0;
    SM_final_time_simulation = 78*24*60;

    Artesao a0;
    Artesao a1;
    Artesao a2;
    Artesao a3;

    a0.add_art();
    a1.add_art();
    a2.add_art();
    a3.add_art();

    Especialista e0;
    Especialista e1;
    Especialista e2;
    Especialista e3;

    e0.add_esp();
    e1.add_esp();
    e2.add_esp();
    e3.add_esp();

    init_simulation();
    while(SM_list_event_simulation){
        event_list_t act = SM_list_event_simulation[0];
        act.event.funct_event();
    }

    for (int i=0; i < SM_vaso_finish_length; i++){
      printf("Vaso ID: %ld\n", SM_vaso_finish[i]->get_id());
    }
    printf("\nUltimo ID vaso: %ld\n", last_id_vaso);
    return 0;
}
