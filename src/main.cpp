#include <time.h>
#include "./include/fila.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SM_queue_vasos = (Vaso***) malloc(sizeof(Vaso**)*QUEUE_WAIT);
    SM_time_simulation = 0;
    SM_final_time_simulation = 72*24*60;
    /*
    for (int i=0; i<QUEUE_WAIT;i++){
      Vaso* v1 = new Vaso(i, SMALL, i);
      v1->set_queue(QUEUE_WAIT-i, PREPARA_BASE);
      put_vaso_fila(v1, PREPARA_BASE);
    }

    for (int i=0; i<QUEUE_WAIT;i++){
      std::cout << "ID:\t" << SM_queue_vasos[PREPARA_BASE][i]->get_id() << "\tTIME:\t" <<  SM_queue_vasos[PREPARA_BASE][i]->get_queue(PREPARA_BASE) << '\n';
    }
    for (int i=0; i<QUEUE_WAIT;i++){
      pop_vaso_fila(PREPARA_BASE);
    }
    */
    SM_Artesao = (Artesao*)malloc(sizeof(Artesao)*2);
    SM_Especialista = (Especialista*)malloc(sizeof(Especialista)*1);
    SM_quatd_Art = 2;
    SM_quatd_Esp = 1;
    Artesao a0(0,OCIOSITY_ART,0,0);
    SM_Artesao[0] = a0;
    Artesao a1(1,OCIOSITY_ART,0,0);
    SM_Artesao[1] = a1;
    Especialista e1(0,OCIOSITY_ESP,0,0);
    SM_Especialista[0] = e1;
    init_simulation();
    while(SM_list_event_simulation){
        event_list_t act = SM_list_event_simulation[0];
        act.event.funct_event();
        printf("==============================================\n");
    }


    return 0;
}
