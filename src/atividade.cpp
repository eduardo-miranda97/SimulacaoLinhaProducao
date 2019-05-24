#include "./include/atividade.h"

#define DAYINMINUTES      (24*60)

#define NUM_PEDIDOS          30
#define VAR_PEDIDOS           7
void init_simulation(){
    times_triangular_t tring_pedidos = {(NUM_PEDIDOS-VAR_PEDIDOS), NUM_PEDIDOS, (NUM_PEDIDOS+VAR_PEDIDOS)};
    double             start_time    = 0;
    do{
        event_t event = {start_time, &begin_requets, {NULL, NULL, NULL}};
        insert_list_event(event);
        u_int8_t quatd_pedidos = trand(tring_pedidos);
        start_time            += (quatd_pedidos*DAYINMINUTES);
    }while(start_time < SM_final_time_simulation); //final
}

void begin_requets(void* ptr){
      u_int16_t quatd_vasos = trand(SM_quatd_vasos);
      for (u_int16_t i=0; i < quatd_vasos; i++){
          Vaso *vaso = new Vaso(last_id_vaso++, Vaso::rand_type(), SM_time_simulation);
          if (!vaso){
              printf("\n================================================================================\n");
              printf("\n[ERRO] falha na alocação de memoria em begin_requets()\n\n");
              printf("\n================================================================================\n");
              exit(ERRO_MEMORY_ACESS);
          }
          if (ART_FREE() || ESP_FREE()){
             if (SM_massa >= vaso->get_quatd_massa())
                if (SM_espaco_secagem >= vaso->get_quatd_espace()){
                    SM_massa          -= vaso->get_quatd_massa();
                    SM_espaco_secagem -= vaso->get_quatd_espace();
                    times_triangular_t times;

                    if (vaso->get_type() == SMALL){
                        times_triangular_t times = {1,2,3};
                    }else if (vaso->get_type() == MEDIUM){
                        times_triangular_t times = {2,4,6};
                    }else{
                        times_triangular_t times = {4,6,8};
                    }

                    event_t event;
                    if (ART_FREE()){
                        Artesao* artesao = GET_ART_FREE();
                        artesao->set_situation(state_art::ACTIVE_ART);
                        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                        event_t event = {SM_time_simulation+trand(times), &base_preparation, {vaso, artesao, NULL}};
                    }else{
                        Especialista* especialista = GET_ESP_FREE();
                        especialista->set_situation(state_esp::ACTIVE_ESP);
                        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
                        event_t event = {SM_time_simulation+trand(times), &base_preparation, {vaso, NULL, especialista}};
                    }
                    insert_list_event(event);
                }
          }else{
            vaso->set_queue(SM_time_simulation, PREPARA_BASE);
            put_vaso_fila(vaso, PREPARA_BASE);
          }
      }
}

void base_preparation(void* ptr){
    /*verifica filas*/
    times_triangular_t times;
    if (ptr){
        event_t* event = (event_t*) ptr;
        if (event->uses.vaso->get_type() == SMALL){
            times_triangular_t times = {10,15,20};
        }else if (event->uses.vaso->get_type() == MEDIUM){
            times_triangular_t times = {30,40,50};
        }else{
            times_triangular_t times = {100,120,140};
        }
        event_t new_event = {SM_time_simulation+trand(times), &base_set_init, event->uses};
        // remove_list_event(event);
        insert_list_event(new_event);
        return;
    }
    /*agora busca pelas filas*/
}


void base_set_init(void* ptr){

}
