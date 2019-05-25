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
    }while(start_time < SM_final_time_simulation);
}

void begin_requets(){
      u_int16_t quatd_vasos = trand(SM_quatd_vasos);
      for (u_int16_t i=0; i < quatd_vasos; i++){
          Vaso *vaso = new Vaso(last_id_vaso++, Vaso::rand_type(), SM_time_simulation);
          if (!vaso){
              printf("\n================================================================================\n");
              printf("\n[ERRO] falha na alocação de memoria em begin_requets()\n\n");
              printf("\n================================================================================\n");
              exit(ERRO_MEMORY_ACESS);
          }
          if (ART_FREE() || ESP_FREE())
             if (SM_massa >= vaso->get_quatd_massa())
                if (SM_espaco_secagem >= vaso->get_quatd_espace()){
                      SM_massa          -= vaso->get_quatd_massa();
                      SM_espaco_secagem -= vaso->get_quatd_espace();

                      times_triangular_t times;
                      if (vaso->get_type() == SMALL){
                          times.time_min  = 1;
                          times.time_mode = 2;
                          times.time_max  = 3;
                      }else if (vaso->get_type() == MEDIUM){
                          times.time_min  = 2;
                          times.time_mode = 4;
                          times.time_max  = 6;
                      }else{
                          times.time_min  = 4;
                          times.time_mode = 6;
                          times.time_max  = 8;
                      }

                      event_t event;
                      if (ART_FREE()){
                          Artesao* artesao = GET_ART_FREE();
                          artesao->set_situation(state_art::ACTIVE_ART);
                          artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                          event.time_event  = (SM_time_simulation+trand(times));
                          event.funct_event = &base_preparation;
                          event.uses->vaso  = vaso;
                          event.uses->art   = artesao;
                          event.uses->esp   = NULL;
                      }else{
                          Especialista* especialista = GET_ESP_FREE();
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
                          event.time_event  = (SM_time_simulation+trand(times));
                          event.funct_event = &base_preparation;
                          event.uses->vaso  = vaso;
                          event.uses->art   = NULL;
                          event.uses->esp   = especialista;
                      }
                      insert_list_event(event);
                      continue;
            }
            vaso->set_queue(SM_time_simulation, PREPARA_BASE);
            put_vaso_fila(vaso, PREPARA_BASE);
      }
}

void base_preparation(){
    times_triangular_t times;
    if (SM_list_event_simulation[0].event.uses->vaso){
        event_t evento = SM_list_event_simulation[0].event;
        if (evento.uses.vaso->get_type() == SMALL){
            times.time_min  = 10;
            times.time_mode = 15;
            times.time_max  = 20;
        }else if (evento.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 20;
            times.time_mode = 25;
            times.time_max  = 30;
        }else{
            times.time_min  = 40;
            times.time_mode = 50;
            times.time_max  = 60;
        }
        event_t new_event = {SM_time_simulation+trand(times), &base_set_init, evento.uses};
        insert_list_event(new_event);
        return;
    }
    /* Verificar filas em ordem a ser definida, REUNIR PARA DISCUTIR*/
}


void base_set_init(){

}
