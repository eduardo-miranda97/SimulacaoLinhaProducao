#include <time.h>
#include "./include/fila.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SM_queue_vasos = (Vaso***) malloc(sizeof(Vaso**)*QUEUE_WAIT);
    SM_time_simulation = 0;
    SM_final_time_simulation = 72*24*60;

    Artesao a0;
    Artesao a1;
    a0.add_art();
    a1.add_art();

    Especialista e0;
    e0.add_esp();

    init_simulation();
    while(SM_list_event_simulation){
        event_list_t act = SM_list_event_simulation[0];
        act.event.funct_event();
        printf("==============================================\n");
    }


    return 0;
}
