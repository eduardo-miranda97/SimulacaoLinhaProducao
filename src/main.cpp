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

    init_simulation();
    begin_requets(NULL);

    for (int i=0; i<SM_length_queue_vasos[PREPARA_BASE];i++){
      std::cout << "ID:\t" << SM_queue_vasos[PREPARA_BASE][i]->get_id() << "\tTIME:\t" <<  SM_queue_vasos[PREPARA_BASE][i]->get_queue(PREPARA_BASE) << '\n';
    }


    return 0;
}
