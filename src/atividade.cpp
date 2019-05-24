#include "./include/atividade.h"

#define NUM_PEDIDOS        30
#define VAR_PEDIDOS         7
void begin_requets(){
    times_triangular_t tring_pedidos = {(NUM_PEDIDOS-VAR_PEDIDOS), NUM_PEDIDOS, (NUM_PEDIDOS+VAR_PEDIDOS)};
    double             start_time    = 0;
    u_int8_t           quatd_pedidos, quatd_vasos;
    do{
        quatd_pedidos  = trand(tring_pedidos);
        quatd_vasos    = trand(SM_quatd_vasos);
        for (u_int16_t i=0; i < quatd_vasos; i++){
            Vaso *vaso = new Vaso(last_id_vaso++, Vaso::rand_type(), start_time);
            if (!vaso){
                printf("\n================================================================================\n");
      					printf("\n[ERRO] falha na alocação de memoria em begin_requets()\n\n");
      					printf("\n================================================================================\n");
      					exit(ERRO_MEMORY_ACESS);
            }

            if (ART_FREE() || ESP_FREE()){
               if (SM_massa >= vaso->get_quatd_massa())
                  if (SM_espaco_secagem >= vaso->get_quatd_espace()){
                      event_t event;
                      if (ART_FREE()){
                          Artesao* artesao = GET_ART_FREE();
                          artesao->set_situation(state_art::ACTIVE_ART);
                          artesao->set_time_ociosity(start_time-artesao->get_start_ociosity());
                          event_t event = {SM_time_simulation+start_time, &base_preparation, {vaso, artesao, NULL}};
                      }else{
                          Especialista* especialista = GET_ESP_FREE();
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          especialista->set_time_ociosity(start_time-especialista->get_start_ociosity());
                          event_t event = {SM_time_simulation+start_time, &base_preparation, {vaso, NULL, especialista}};
                      }
                      insert_list_event(event);
                  }
            }else{
              vaso->set_queue(start_time, PREPARA_BASE);
              put_vaso_fila(vaso, PREPARA_BASE);
            }
        }
        start_time    += (quatd_pedidos*(24*60));
    }while(start_time < SM_time_simulation);
}

void base_preparation(){
    return;
}
