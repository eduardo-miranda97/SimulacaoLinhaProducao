#include <time.h>
#include "./include/fila.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SM_queue_vasos = (Vaso***) malloc(sizeof(Vaso**)*QUEUE_WAIT);
    SM_time_simulation = 0;
    SM_final_time_simulation = 72*24*60;

    Artesao a0;
    Artesao a1;
    Artesao::add_art(&a0);
    Artesao::add_art(&a1);

    Especialista e0;
    Especialista::add_esp(&e0);

    init_simulation();
    while(SM_list_event_simulation){
        event_list_t act = SM_list_event_simulation[0];
        act.event.funct_event();
        printf("==============================================\n");
    }


    return 0;
}
