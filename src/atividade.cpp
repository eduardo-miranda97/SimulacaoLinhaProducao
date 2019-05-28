#include "./include/atividade.h"

#define DAYINMINUTES             (24*60)

#define NUM_PEDIDOS                   30
#define VAR_PEDIDOS                    7
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
        insert_list_event(event);
        u_int8_t quatd_pedidos = trand(times);
        start_time            += (quatd_pedidos*DAYINMINUTES);
    }while(start_time < SM_final_time_simulation);
}

void begin_requets(){
      SM_time_simulation = SM_list_event_simulation[0].event.time_event;
      remove_list_event(&(SM_list_event_simulation[0]));

      u_int16_t quatd_vasos = trand(SM_quatd_vasos);
      for (u_int16_t i=0; i < quatd_vasos; i++){
          Vaso* vaso = new Vaso(SM_time_simulation);
          if (!vaso){
              printf("\n================================================================================\n");
              printf("\n[ERRO] falha na alocação de memoria em begin_requets()\n\n");
              printf("\n================================================================================\n");
              exit(ERRO_MEMORY_ACESS);
          }
          if (Artesao::is_free() || Especialista::is_free())
             if (SM_massa >= vaso->get_quatd_massa())
               if (SM_espaco_secagem >= vaso->get_quatd_espace()){
                      SM_massa          -= vaso->get_quatd_massa();
                      SM_espaco_secagem -= vaso->get_quatd_espace();

                      times_triangular_t times;
                      event_t new_event;
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

                      if (Artesao::is_free()){
                          Artesao* artesao = Artesao::get_free();
                          artesao->set_situation(state_art::ACTIVE_ART);
                          artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                          new_event.time_event  = (SM_time_simulation+trand(times));
                          new_event.funct_event = &form_preparation;
                          new_event.uses.vaso   = vaso;
                          new_event.uses.art    = artesao;
                          new_event.uses.esp    = NULL;
                      }else{
                          Especialista* especialista = Especialista::get_free();
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
                          new_event.time_event  = (SM_time_simulation+trand(times));
                          new_event.funct_event = &form_preparation;
                          new_event.uses.vaso   = vaso;
                          new_event.uses.art    = NULL;
                          new_event.uses.esp    = especialista;
                      }
                      insert_list_event(new_event);
                      continue;
                }
          vaso->set_queue(SM_time_simulation, PREPARA_FORM);
          SM_queue_vasos[PREPARA_FORM].push_back(vaso);
      }
}

void form_preparation(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 1;
        times.time_mode = 2;
        times.time_max  = 4;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 2;
        times.time_mode = 4;
        times.time_max  = 6;
    }else{
        times.time_min  = 4;
        times.time_mode = 6;
        times.time_max  = 8;
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &base_preparation;
    insert_list_event(new_event);
}

void base_preparation(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 10;
        times.time_mode = 15;
        times.time_max  = 20;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 30;
        times.time_mode = 40;
        times.time_max  = 50;
    }else{
        times.time_min  = 100;
        times.time_mode = 120;
        times.time_max  = 140;
    }
    new_event.time_event += trand(times);
    new_event.funct_event = &base_set_init;
    insert_list_event(new_event);
}

#define  PORC_NIVEL_MASSA         (0.2*SM_massa_MAX)
#define  PORC_NIVEL_PEDRA         (0.2*SM_pedra_MAX)
void base_set_init(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= PORC_NIVEL_MASSA){
            flag = false;
            event_t prep_massa = new_event;
            /* Tempo para fazer massa */
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_massa.time_event += trand(times);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            goto SEC_ACAB_BASE_LABEL;

        }else if (SM_pedra <= PORC_NIVEL_PEDRA){
            flag = false;
            event_t prep_pedra = new_event;
            /* Tempo para fazer pedras*/
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_pedra.time_event += trand(times);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            goto SEC_ACAB_BASE_LABEL;

        }else if (SM_queue_vasos[ENV_GERAL].size()){
            flag = false;
            event_t fila_env_geral = new_event;
            Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
            SM_queue_vasos[ENV_GERAL].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
            if (vaso->get_type() == SMALL){
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 13;
                times.time_mode = 18;
                times.time_max  = 23;
            }else{
                times.time_min  = 20;
                times.time_mode = 25;
                times.time_max  = 30;
            }

            fila_env_geral.time_event += trand(times);
            fila_env_geral.funct_event = &varnishing;
            fila_env_geral.uses.vaso   = vaso;
            insert_list_event(fila_env_geral);
            goto SEC_ACAB_BASE_LABEL;

        }else if (SM_queue_vasos[IMP_INTER].size()){
            flag = false;
            event_t fila_imp_inter = new_event;
            Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
            SM_queue_vasos[IMP_INTER].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
            if (vaso->get_type() == SMALL){
                times.time_min  = 1;
                times.time_mode = 3;
                times.time_max  = 5;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 3;
                times.time_mode = 5;
                times.time_max  = 7;
            }else{
                times.time_min  = 5;
                times.time_mode = 8;
                times.time_max  = 11;
            }

            fila_imp_inter.time_event += trand(times);
            fila_imp_inter.funct_event = &inter_waterpoofing;
            fila_imp_inter.uses.vaso   = vaso;
            insert_list_event(fila_imp_inter);
            goto SEC_ACAB_BASE_LABEL;
        }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
            flag = false;
            event_t fila_limp_acab_boca = new_event;
            Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
            SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
            if (vaso->get_type() == SMALL){
                times.time_min  = 3;
                times.time_mode = 6;
                times.time_max  = 9;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 4;
                times.time_mode = 7;
                times.time_max  = 10;
            }else{
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }

            fila_limp_acab_boca.time_event += trand(times);
            fila_limp_acab_boca.funct_event = &mount_clearing;
            fila_limp_acab_boca.uses.vaso   = vaso;
            insert_list_event(fila_limp_acab_boca);
            goto SEC_ACAB_BASE_LABEL;
        }
    }
    if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);

    }else if (SM_queue_vasos[LIMP_ACAB_BASE].size()){
        flag = false;
        event_t fila_limp_acab_base = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BASE].front();
        SM_queue_vasos[LIMP_ACAB_BASE].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BASE)), LIMP_ACAB_BASE);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_limp_acab_base.time_event += trand(times);
        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }

SEC_ACAB_BASE_LABEL:

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 7;
        times.time_max  = 8;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 14;
        times.time_max  = 18;
    }

    if (flag){
        if (new_event.uses.art){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
        }
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &base_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void base_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &base_clearing;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &base_clearing;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        return;
    }
    new_event.uses.vaso->set_queue(SM_time_simulation, LIMP_ACAB_BASE);
    SM_queue_vasos[LIMP_ACAB_BASE].push_back(new_event.uses.vaso);
}

void base_clearing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= PORC_NIVEL_MASSA){
            flag = false;
            event_t prep_massa = new_event;
            /* Tempo para fazer massa */
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_massa.time_event += trand(times);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            goto LIMP_BASE;

        }else if (SM_pedra <= PORC_NIVEL_PEDRA){
            flag = false;
            event_t prep_pedra = new_event;
            /* Tempo para fazer pedras*/
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_pedra.time_event += trand(times);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            goto LIMP_BASE;

        }else if (SM_queue_vasos[ENV_GERAL].size()){
            flag = false;
            event_t fila_env_geral = new_event;
            Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
            SM_queue_vasos[ENV_GERAL].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
            if (vaso->get_type() == SMALL){
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 13;
                times.time_mode = 18;
                times.time_max  = 23;
            }else{
                times.time_min  = 20;
                times.time_mode = 25;
                times.time_max  = 30;
            }

            fila_env_geral.time_event += trand(times);
            fila_env_geral.funct_event = &varnishing;
            fila_env_geral.uses.vaso   = vaso;
            insert_list_event(fila_env_geral);
            goto LIMP_BASE;

        }else if (SM_queue_vasos[IMP_INTER].size()){
            flag = false;
            event_t fila_imp_inter = new_event;
            Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
            SM_queue_vasos[IMP_INTER].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
            if (vaso->get_type() == SMALL){
                times.time_min  = 1;
                times.time_mode = 3;
                times.time_max  = 5;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 3;
                times.time_mode = 5;
                times.time_max  = 7;
            }else{
                times.time_min  = 5;
                times.time_mode = 8;
                times.time_max  = 11;
            }

            fila_imp_inter.time_event += trand(times);
            fila_imp_inter.funct_event = &inter_waterpoofing;
            fila_imp_inter.uses.vaso   = vaso;
            insert_list_event(fila_imp_inter);
            goto LIMP_BASE;
        }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
            flag = false;
            event_t fila_limp_acab_boca = new_event;
            Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
            SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
            if (vaso->get_type() == SMALL){
                times.time_min  = 3;
                times.time_mode = 6;
                times.time_max  = 9;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 4;
                times.time_mode = 7;
                times.time_max  = 10;
            }else{
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }

            fila_limp_acab_boca.time_event += trand(times);
            fila_limp_acab_boca.funct_event = &mount_clearing;
            fila_limp_acab_boca.uses.vaso   = vaso;
            insert_list_event(fila_limp_acab_boca);
            goto LIMP_BASE;
        }
    }
    if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);

    }else if (SM_queue_vasos[LIMP_ACAB_BASE].size()){
        flag = false;
        event_t fila_limp_acab_base = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BASE].front();
        SM_queue_vasos[LIMP_ACAB_BASE].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BASE)), LIMP_ACAB_BASE);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_limp_acab_base.time_event += trand(times);
        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }

LIMP_BASE:

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 7;
        times.time_max  = 8;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 14;
        times.time_max  = 18;
    }

    if (flag){
        if (new_event.uses.art){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
        }
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &base_finish;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void base_finish(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &mouth_preparation;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 3;
            times.time_mode = 7;
            times.time_max  = 11;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &mouth_preparation;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        return;
    }
    new_event.uses.vaso->set_queue(SM_time_simulation, PREP_BOCA);
    SM_queue_vasos[PREP_BOCA].push_back(new_event.uses.vaso);
}

void mouth_preparation(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 1;
        times.time_mode = 3;
        times.time_max  = 5;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 2;
        times.time_mode = 5;
        times.time_max  = 8;
    }else{
        times.time_min  = 4;
        times.time_mode = 8;
        times.time_max  = 12;
    }
    new_event.time_event += trand(times);
    new_event.funct_event = &mouth_set_init;
    insert_list_event(new_event);
}

void mouth_set_init(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= PORC_NIVEL_MASSA){
            flag = false;
            event_t prep_massa = new_event;
            /* Tempo para fazer massa */
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_massa.time_event += trand(times);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            goto ACAB_INICIAL_BOCA;

        }else if (SM_pedra <= PORC_NIVEL_PEDRA){
            flag = false;
            event_t prep_pedra = new_event;
            /* Tempo para fazer pedras*/
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_pedra.time_event += trand(times);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            goto ACAB_INICIAL_BOCA;

        }else if (SM_queue_vasos[ENV_GERAL].size()){
            flag = false;
            event_t fila_env_geral = new_event;
            Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
            SM_queue_vasos[ENV_GERAL].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
            if (vaso->get_type() == SMALL){
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 13;
                times.time_mode = 18;
                times.time_max  = 23;
            }else{
                times.time_min  = 20;
                times.time_mode = 25;
                times.time_max  = 30;
            }

            fila_env_geral.time_event += trand(times);
            fila_env_geral.funct_event = &varnishing;
            fila_env_geral.uses.vaso   = vaso;
            insert_list_event(fila_env_geral);
            goto ACAB_INICIAL_BOCA;

        }else if (SM_queue_vasos[IMP_INTER].size()){
            flag = false;
            event_t fila_imp_inter = new_event;
            Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
            SM_queue_vasos[IMP_INTER].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
            if (vaso->get_type() == SMALL){
                times.time_min  = 1;
                times.time_mode = 3;
                times.time_max  = 5;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 3;
                times.time_mode = 5;
                times.time_max  = 7;
            }else{
                times.time_min  = 5;
                times.time_mode = 8;
                times.time_max  = 11;
            }

            fila_imp_inter.time_event += trand(times);
            fila_imp_inter.funct_event = &inter_waterpoofing;
            fila_imp_inter.uses.vaso   = vaso;
            insert_list_event(fila_imp_inter);
            goto ACAB_INICIAL_BOCA;
        }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
            flag = false;
            event_t fila_limp_acab_boca = new_event;
            Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
            SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
            if (vaso->get_type() == SMALL){
                times.time_min  = 3;
                times.time_mode = 6;
                times.time_max  = 9;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 4;
                times.time_mode = 7;
                times.time_max  = 10;
            }else{
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }

            fila_limp_acab_boca.time_event += trand(times);
            fila_limp_acab_boca.funct_event = &mount_clearing;
            fila_limp_acab_boca.uses.vaso   = vaso;
            insert_list_event(fila_limp_acab_boca);
            goto ACAB_INICIAL_BOCA;
        }
    }
    if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);

    }else if (SM_queue_vasos[LIMP_ACAB_BASE].size()){
        flag = false;
        event_t fila_limp_acab_base = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BASE].front();
        SM_queue_vasos[LIMP_ACAB_BASE].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BASE)), LIMP_ACAB_BASE);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_limp_acab_base.time_event += trand(times);
        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }

ACAB_INICIAL_BOCA:

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 8;
        times.time_max  = 13;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 13;
        times.time_max  = 16;
    }

    if (flag){
        if (new_event.uses.art){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
        }
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &mouth_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void mouth_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &mount_clearing;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        return;
    }
    new_event.uses.vaso->set_queue(SM_time_simulation, LIMP_ACAB_BOCA);
    SM_queue_vasos[LIMP_ACAB_BOCA].push_back(new_event.uses.vaso);
}

void mount_clearing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= PORC_NIVEL_MASSA){
            flag = false;
            event_t prep_massa = new_event;
            /* Tempo para fazer massa */
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_massa.time_event += trand(times);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            goto LIMP_BOCA;

        }else if (SM_pedra <= PORC_NIVEL_PEDRA){
            flag = false;
            event_t prep_pedra = new_event;
            /* Tempo para fazer pedras*/
            times.time_min  = 10;
            times.time_mode = 20;
            times.time_max  = 30;
            prep_pedra.time_event += trand(times);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            goto LIMP_BOCA;

        }else if (SM_queue_vasos[ENV_GERAL].size()){
            flag = false;
            event_t fila_env_geral = new_event;
            Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
            SM_queue_vasos[ENV_GERAL].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
            if (vaso->get_type() == SMALL){
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 13;
                times.time_mode = 18;
                times.time_max  = 23;
            }else{
                times.time_min  = 20;
                times.time_mode = 25;
                times.time_max  = 30;
            }

            fila_env_geral.time_event += trand(times);
            fila_env_geral.funct_event = &varnishing;
            fila_env_geral.uses.vaso   = vaso;
            insert_list_event(fila_env_geral);
            goto LIMP_BOCA;

        }else if (SM_queue_vasos[IMP_INTER].size()){
            flag = false;
            event_t fila_imp_inter = new_event;
            Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
            SM_queue_vasos[IMP_INTER].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
            if (vaso->get_type() == SMALL){
                times.time_min  = 1;
                times.time_mode = 3;
                times.time_max  = 5;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 3;
                times.time_mode = 5;
                times.time_max  = 7;
            }else{
                times.time_min  = 5;
                times.time_mode = 8;
                times.time_max  = 11;
            }

            fila_imp_inter.time_event += trand(times);
            fila_imp_inter.funct_event = &inter_waterpoofing;
            fila_imp_inter.uses.vaso   = vaso;
            insert_list_event(fila_imp_inter);
            goto LIMP_BOCA;
        }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
            flag = false;
            event_t fila_limp_acab_boca = new_event;
            Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
            SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
            vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
            if (vaso->get_type() == SMALL){
                times.time_min  = 3;
                times.time_mode = 6;
                times.time_max  = 9;
            }else if (vaso->get_type() == MEDIUM){
                times.time_min  = 4;
                times.time_mode = 7;
                times.time_max  = 10;
            }else{
                times.time_min  = 5;
                times.time_mode = 10;
                times.time_max  = 15;
            }

            fila_limp_acab_boca.time_event += trand(times);
            fila_limp_acab_boca.funct_event = &mount_clearing;
            fila_limp_acab_boca.uses.vaso   = vaso;
            insert_list_event(fila_limp_acab_boca);
            goto LIMP_BOCA;
        }
    }
    if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);

    }else if (SM_queue_vasos[LIMP_ACAB_BASE].size()){
        flag = false;
        event_t fila_limp_acab_base = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BASE].front();
        SM_queue_vasos[LIMP_ACAB_BASE].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BASE)), LIMP_ACAB_BASE);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_limp_acab_base.time_event += trand(times);
        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }
LIMP_BOCA:

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 7;
        times.time_max  = 8;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 14;
        times.time_max  = 18;
    }

    if (flag){
        if (new_event.uses.art){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
        }
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &mount_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void mount_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &inter_waterpoofing;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        return;
    }
    new_event.uses.vaso->set_queue(SM_time_simulation, IMP_INTER);
    SM_queue_vasos[IMP_INTER].push_back(new_event.uses.vaso);
}

void inter_waterpoofing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (SM_massa <= PORC_NIVEL_MASSA){
        flag = false;
        event_t prep_massa = new_event;
        /* Tempo para fazer massa */
        times.time_min  = 10;
        times.time_mode = 20;
        times.time_max  = 30;
        prep_massa.time_event += trand(times);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        insert_list_event(prep_massa);

    }else if (SM_pedra <= PORC_NIVEL_PEDRA){
        flag = false;
        event_t prep_pedra = new_event;
        /* Tempo para fazer pedras*/
        times.time_min  = 10;
        times.time_mode = 20;
        times.time_max  = 30;
        prep_pedra.time_event += trand(times);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        insert_list_event(prep_pedra);

    }else if (SM_queue_vasos[ENV_GERAL].size()){
        flag = false;
        event_t fila_env_geral = new_event;
        Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
        SM_queue_vasos[ENV_GERAL].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_env_geral.time_event += trand(times);
        fila_env_geral.funct_event = &varnishing;
        fila_env_geral.uses.vaso   = vaso;
        insert_list_event(fila_env_geral);

    }else if (SM_queue_vasos[IMP_INTER].size()){
        flag = false;
        event_t fila_imp_inter = new_event;
        Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
        SM_queue_vasos[IMP_INTER].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_imp_inter.time_event += trand(times);
        fila_imp_inter.funct_event = &inter_waterpoofing;
        fila_imp_inter.uses.vaso   = vaso;
        insert_list_event(fila_imp_inter);

    }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
        flag = false;
        event_t fila_limp_acab_boca = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
        SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
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

        fila_limp_acab_boca.time_event += trand(times);
        fila_limp_acab_boca.funct_event = &mount_clearing;
        fila_limp_acab_boca.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_boca);
    }else if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
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

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 7;
        times.time_max  = 8;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 14;
        times.time_max  = 18;
    }

    if (flag){
        new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
        new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &inter_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void inter_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
        if (new_event.uses.vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }
        new_event.time_event += trand(times);
        new_event.funct_event = &varnishing;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        return;
    }
    new_event.uses.vaso->set_queue(SM_time_simulation, ENV_GERAL);
    SM_queue_vasos[ENV_GERAL].push_back(new_event.uses.vaso);
}

void varnishing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    times_triangular_t times;
    bool flag = true;
    if (SM_massa <= PORC_NIVEL_MASSA){
        flag = false;
        event_t prep_massa = new_event;
        /* Tempo para fazer massa */
        times.time_min  = 10;
        times.time_mode = 20;
        times.time_max  = 30;
        prep_massa.time_event += trand(times);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        insert_list_event(prep_massa);

    }else if (SM_pedra <= PORC_NIVEL_PEDRA){
        flag = false;
        event_t prep_pedra = new_event;
        /* Tempo para fazer pedras*/
        times.time_min  = 10;
        times.time_mode = 20;
        times.time_max  = 30;
        prep_pedra.time_event += trand(times);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        insert_list_event(prep_pedra);

    }else if (SM_queue_vasos[ENV_GERAL].size()){
        flag = false;
        event_t fila_env_geral = new_event;
        Vaso* vaso = SM_queue_vasos[ENV_GERAL].front();
        SM_queue_vasos[ENV_GERAL].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(ENV_GERAL)), ENV_GERAL);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 7;
            times.time_max  = 12;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 10;
            times.time_max  = 15;
        }else{
            times.time_min  = 9;
            times.time_mode = 14;
            times.time_max  = 19;
        }

        fila_env_geral.time_event += trand(times);
        fila_env_geral.funct_event = &varnishing;
        fila_env_geral.uses.vaso   = vaso;
        insert_list_event(fila_env_geral);

    }else if (SM_queue_vasos[IMP_INTER].size()){
        flag = false;
        event_t fila_imp_inter = new_event;
        Vaso* vaso = SM_queue_vasos[IMP_INTER].front();
        SM_queue_vasos[IMP_INTER].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(IMP_INTER)), IMP_INTER);
        if (vaso->get_type() == SMALL){
            times.time_min  = 2;
            times.time_mode = 5;
            times.time_max  = 8;
        }else if (vaso->get_type() == MEDIUM){
            times.time_min  = 5;
            times.time_mode = 8;
            times.time_max  = 11;
        }else{
            times.time_min  = 8;
            times.time_mode = 11;
            times.time_max  = 14;
        }

        fila_imp_inter.time_event += trand(times);
        fila_imp_inter.funct_event = &inter_waterpoofing;
        fila_imp_inter.uses.vaso   = vaso;
        insert_list_event(fila_imp_inter);

    }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
        flag = false;
        event_t fila_limp_acab_boca = new_event;
        Vaso* vaso = SM_queue_vasos[LIMP_ACAB_BOCA].front();
        SM_queue_vasos[LIMP_ACAB_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(LIMP_ACAB_BOCA)), LIMP_ACAB_BOCA);
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

        fila_limp_acab_boca.time_event += trand(times);
        fila_limp_acab_boca.funct_event = &mount_clearing;
        fila_limp_acab_boca.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_boca);
    }else if (SM_queue_vasos[PREP_BOCA].size()){
        flag = false;
        event_t fila_prep_boca = new_event;
        Vaso* vaso = SM_queue_vasos[PREP_BOCA].front();
        SM_queue_vasos[PREP_BOCA].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREP_BOCA)), PREP_BOCA);
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

        fila_prep_boca.time_event += trand(times);
        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
        flag = false;
        event_t fila_prepara_form = new_event;
        Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
        SM_queue_vasos[PREPARA_FORM].pop_front();
        vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);
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

        fila_prepara_form.time_event += trand(times);
        fila_prepara_form.funct_event = &form_preparation;
        fila_prepara_form.uses.vaso   = vaso;
        insert_list_event(fila_prepara_form);
    }

    if (new_event.uses.vaso->get_type() == SMALL){
        times.time_min  = 5;
        times.time_mode = 7;
        times.time_max  = 8;
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        times.time_min  = 7;
        times.time_mode = 10;
        times.time_max  = 13;
    }else{
        times.time_min  = 10;
        times.time_mode = 14;
        times.time_max  = 18;
    }

    if (flag){
        new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
        new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }

    new_event.time_event += trand(times);
    new_event.funct_event = &final_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void final_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    SM_espaco_secagem++;
    SM_vaso_finish.push_back(new_event.uses.vaso);
}

void preparation_massa(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));
}

void preparation_pedras(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));
}
