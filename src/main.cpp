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
    SM_quatd_Art = 2;
    Artesao a0(0,OCIOSITY_ART,0,0);
    SM_Artesao[0] = a0;
    Artesao a1(1,OCIOSITY_ART,0,0);
    SM_Artesao[1] = a1;
    init_simulation();
    // begin_requets();
    event_list_t* aux = SM_list_event_simulation;
    aux->event.funct_event();
    remove_list_event(SM_list_event_simulation);
    aux = SM_list_event_simulation;
    aux->event.funct_event();
    remove_list_event(SM_list_event_simulation);
    // aux = SM_list_event_simulation;
    // aux->event.funct_event();
    // remove_list_event(SM_list_event_simulation);
    // for (int i=0; i<SM_length_queue_vasos[PREPARA_BASE];i++){
    //   std::cout << "ID:\t" << SM_queue_vasos[PREPARA_BASE][i]->get_id() << "\tTIME:\t" <<  SM_queue_vasos[PREPARA_BASE][i]->get_queue(PREPARA_BASE) << '\n';
    // }
    for(event_list_t* aux=SM_list_event_simulation;aux;aux=aux->next_event){
        if ((aux->event.funct_event)==&begin_requets){
            std::cout << "begin_requets ";
            std::cout << "time: " << aux->event.time_event << '\n';
        }else if ((aux->event.funct_event)==&base_preparation){
            std::cout << "base_preparation ";
            std::cout << "time: " << aux->event.time_event << '\n';
        }else if ((aux->event.funct_event)==&base_set_init){
            std::cout << "base_set_init ";
            std::cout << "time: " << aux->event.time_event << '\n';
        }
    }

    return 0;
}
