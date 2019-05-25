#include "./include/atividade.h"

#define DAYINMINUTES      (24*60)

#define NUM_PEDIDOS          30
#define VAR_PEDIDOS           7
void init_simulation(){
    times_triangular_t times;
    times.time_min    = (NUM_PEDIDOS-VAR_PEDIDOS);
    times.time_mode   = NUM_PEDIDOS;
    times.time_max    = (NUM_PEDIDOS+VAR_PEDIDOS);
    double start_time = 0;
    event_t event;
    do{
        event.time_event  = start_time;
        event.funct_event = &begin_requets;
        event.uses.vaso   = NULL;
        event.uses.art    = NULL;
        event.uses.esp    = NULL;
        printf("Agenda CHEGADA PEDIDO  Time: %lf\n", start_time);
        insert_list_event(event);
        u_int8_t quatd_pedidos = trand(times);
        start_time            += (quatd_pedidos*DAYINMINUTES);
    }while(start_time < SM_final_time_simulation);
}

void begin_requets(){
      printf("CHEGADA PEDIDO  Time: %lf\n", SM_list_event_simulation[0].event.time_event);
      SM_time_simulation = SM_list_event_simulation[0].event.time_event;
      remove_list_event(&(SM_list_event_simulation[0]));
      u_int16_t quatd_vasos = trand(SM_quatd_vasos);
      for (u_int16_t i=0; i < quatd_vasos; i++){
          Vaso* vaso = new Vaso(last_id_vaso++, SM_time_simulation);
          printf("Instanciado vaso ID: %ld\n", vaso->get_id());
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
                          printf("Agendando PREPARA_BASE vaso ID: %ld com artesao ID: %d\n", vaso->get_id(), artesao->get_id());
                          artesao->set_situation(state_art::ACTIVE_ART);
                          artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                          event.time_event  = (SM_time_simulation+trand(times));
                          event.funct_event = &base_preparation;
                          event.uses.vaso   = vaso;
                          event.uses.art    = artesao;
                          event.uses.esp    = NULL;
                      }else{
                          Especialista* especialista = GET_ESP_FREE();
                          printf("Agendando PREPARA_BASE vaso ID: %ld com Especialista ID: %d\n", vaso->get_id(), especialista->get_id());
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
                          event.time_event  = (SM_time_simulation+trand(times));
                          event.funct_event = &base_preparation;
                          event.uses.vaso   = vaso;
                          event.uses.art    = NULL;
                          event.uses.esp    = especialista;
                      }
                      insert_list_event(event);
                      continue;
            }
            printf("coloca na fila PREPARA_BASE vaso  ID: %ld\n", vaso->get_id());
            vaso->set_queue(SM_time_simulation, PREPARA_BASE);
            put_vaso_fila(vaso, PREPARA_BASE);
      }
}

void base_preparation(){
    times_triangular_t times;
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    if (SM_list_event_simulation[0].event.uses.vaso){
        event_t evento = SM_list_event_simulation[0].event;
        printf("FIM PREPARA_BASE vaso: ID %ld ", evento.uses.vaso->get_id());
        if (evento.uses.art){
           printf(" artesao ID: %d  Time: %lf\n", evento.uses.art->get_id(), SM_list_event_simulation[0].event.time_event);
        }else{
           printf(" especialista ID: %d  Time: %lf\n", evento.uses.esp->get_id(), SM_list_event_simulation[0].event.time_event);
        }
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
        /* TESTES*/
        if (evento.uses.art){
            evento.uses.art->set_situation(OCIOSITY_ART);
        }else{
            evento.uses.esp->set_situation(OCIOSITY_ESP);
        }
        /* FIM testes*/
        event_t new_event;
        new_event.time_event  = (SM_time_simulation+trand(times));
        new_event.funct_event = &base_set_init;
        new_event.uses        = evento.uses;
        remove_list_event(&(SM_list_event_simulation[0]));
        insert_list_event(new_event);
        return;
    }
    remove_list_event(&(SM_list_event_simulation[0]));
    /* Verificar filas em ordem a ser definida, REUNIR PARA DISCUTIR*/
}


void base_set_init(){
    printf("SEM IMPLEMENTAR AINDA\n");
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    remove_list_event(&(SM_list_event_simulation[0]));
}
