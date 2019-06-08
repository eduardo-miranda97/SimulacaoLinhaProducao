#include "./include/atividade.h"

#define DAYINMINUTES             (24*60)

void init_simulation(){
    double start_time = 0;
    event_t event;
    do{
        event.time_event  = start_time;
        event.funct_event = &begin_requets;
        event.uses.vaso   = NULL;
        event.uses.art    = NULL;
        event.uses.esp    = NULL;
        insert_list_event(event);
        u_int8_t quatd_pedidos = trand(SM_quatd_pedidos);
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

                      event_t new_event;
                      if (vaso->get_type() == SMALL){
                          new_event.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-s"]));
                      }else if (vaso->get_type() == MEDIUM){
                          new_event.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-m"]));
                      }else{
                          new_event.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-b"]));
                      }

                      if (Especialista::is_free()){
                          Especialista* especialista = Especialista::get_free();
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

                          new_event.funct_event = &form_preparation;
                          new_event.uses.vaso   = vaso;
                          new_event.uses.art    = NULL;
                          new_event.uses.esp    = especialista;
                      }else{
                          Artesao* artesao = Artesao::get_free();
                          artesao->set_situation(state_art::ACTIVE_ART);
                          artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                          new_event.funct_event = &form_preparation;
                          new_event.uses.vaso   = vaso;
                          new_event.uses.art    = artesao;
                          new_event.uses.esp    = NULL;
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

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["prep_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["prep_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["prep_base-b"]);
    }

    new_event.funct_event = &base_preparation;
    insert_list_event(new_event);
}

void base_preparation(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["acab_ini_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["acab_ini_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["acab_ini_base-b"]);
    }

    new_event.funct_event = &base_set_init;
    insert_list_event(new_event);
}

#define  PORC_NIVEL_MASSA         (0.35*SM_massa_MAX)
#define  PORC_NIVEL_PEDRA         (0.35*SM_pedra_MAX)
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
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
            }else{
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
            }

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
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
            }else{
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
            }

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
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
            }else{
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
            }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

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
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

SEC_ACAB_BASE_LABEL:

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["sec_acab_base-b"]);
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

    new_event.funct_event = &base_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void base_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["sec_acab_base-b"]);
    }
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

        new_event.funct_event = &base_clearing;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

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
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
            }else{
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
            }

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
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
            }else{
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
            }

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
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
            }else{
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
            }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

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
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

LIMP_BASE:

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["limp_acab_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["limp_acab_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["limp_acab_base-b"]);
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

    new_event.funct_event = &base_finish;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void base_finish(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["secagem_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["secagem_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["secagem_base-b"]);
    }
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

        new_event.funct_event = &mouth_preparation;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

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


    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["prep_boca-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["prep_boca-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["prep_boca-b"]);
    }
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
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
            }else{
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
            }

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
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
            }else{
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
            }

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
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
            }else{
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
            }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

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
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

ACAB_INICIAL_BOCA:

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["acab_ini_boca-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["acab_ini_boca-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["acab_ini_boca-b"]);
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


    new_event.funct_event = &mouth_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void mouth_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        if (new_event.uses.vaso->get_type() == SMALL){
            new_event.time_event  += trand(SM_times_events["sec_acab_boca-s"]);
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            new_event.time_event  += trand(SM_times_events["sec_acab_boca-m"]);
        }else{
            new_event.time_event  += trand(SM_times_events["sec_acab_boca-b"]);
        }


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
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
            }else{
                fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
            }

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
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
            }else{
                fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
            }

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
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
            }else if (vaso->get_type() == MEDIUM){
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
            }else{
                fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
            }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

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
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_base.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

        fila_limp_acab_base.funct_event = &base_clearing;
        fila_limp_acab_base.uses.vaso   = vaso;
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

LIMP_BOCA:

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
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


    new_event.funct_event = &mount_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void mount_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        if (new_event.uses.vaso->get_type() == SMALL){
            new_event.time_event  += trand(SM_times_events["secagem_boca-s"]);
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            new_event.time_event  += trand(SM_times_events["secagem_boca-m"]);
        }else{
            new_event.time_event  += trand(SM_times_events["secagem_boca-b"]);
        }

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
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
        }else{
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
        }

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
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
        }else{
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
        }

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
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["sec_acab_base-b"]);
    }

    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }


    new_event.funct_event = &inter_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void inter_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        if (new_event.uses.vaso->get_type() == SMALL){
            new_event.time_event  += trand(SM_times_events["sec_interna-s"]);
        }else if (new_event.uses.vaso->get_type() == MEDIUM){
            new_event.time_event  += trand(SM_times_events["sec_interna-m"]);
        }else{
            new_event.time_event  += trand(SM_times_events["sec_interna-b"]);
        }


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
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
        }else{
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
        }

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
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
        }else{
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
        }

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
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["env_geral-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["env_geral-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["env_geral-b"]);
    }

    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }


    new_event.funct_event = &final_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void final_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));


    SM_espaco_secagem += new_event.uses.vaso->get_quatd_espace();
    if (Especialista::is_free() || Artesao::is_free())
      if (SM_queue_vasos[PREPARA_FORM].size()){
         Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
         if (SM_massa  >= vaso->get_quatd_massa())
           if (SM_pedra >= vaso->get_quatd_pedra())
            if (SM_espaco_secagem >= vaso->get_quatd_espace()){
              SM_espaco_secagem -= vaso->get_quatd_espace();
              SM_massa          -= vaso->get_quatd_massa();
              SM_pedra          -= vaso->get_quatd_pedra();
              event_t fila_prepara_form = new_event;
              SM_queue_vasos[PREPARA_FORM].pop_front();
              vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

              if (vaso->get_type() == SMALL){
                  fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
              }else if (vaso->get_type() == MEDIUM){
                  fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
              }else{
                  fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
              }

              if (Especialista::is_free()){
                  fila_prepara_form.uses.esp = Especialista::get_free();
              }else{
                  fila_prepara_form.uses.art = Artesao::get_free();
              }

              fila_prepara_form.funct_event = &form_preparation;
              fila_prepara_form.uses.vaso   = vaso;
              insert_list_event(fila_prepara_form);
           }
      }
    new_event.uses.vaso->set_end_time(SM_time_simulation);
    SM_vaso_finish.push_back(new_event.uses.vaso);
}

void preparation_massa(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    bool flag = true;
    times_triangular_t times;
    SM_massa = SM_massa_MAX;
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
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
        }else{
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
        }

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
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
        }else{
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
        }

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
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }
    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }
}

void preparation_pedras(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    bool flag = true;
    times_triangular_t times;
    SM_pedra = SM_pedra_MAX;
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
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-m"]);
        }else{
            fila_env_geral.time_event  += trand(SM_times_events["env_geral-b"]);
        }

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
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-m"]);
        }else{
            fila_imp_inter.time_event  += trand(SM_times_events["imp_interna-b"]);
        }

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
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-m"]);
        }else{
            fila_limp_acab_boca.time_event  += trand(SM_times_events["limp_acab_boca-b"]);
        }

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
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-s"]);
        }else if (vaso->get_type() == MEDIUM){
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-m"]);
        }else{
            fila_prep_boca.time_event  += trand(SM_times_events["prep_boca-b"]);
        }

        fila_prep_boca.funct_event = &mouth_preparation;
        fila_prep_boca.uses.vaso   = vaso;
        insert_list_event(fila_prep_boca);
    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          flag = false;
          event_t fila_prepara_form = new_event;
          SM_queue_vasos[PREPARA_FORM].pop_front();
          vaso->set_queue((SM_time_simulation-vaso->get_queue(PREPARA_FORM)), PREPARA_FORM);

          if (vaso->get_type() == SMALL){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-s"]);
          }else if (vaso->get_type() == MEDIUM){
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-m"]);
          }else{
              fila_prepara_form.time_event  += trand(SM_times_events["prep_form-b"]);
          }

          fila_prepara_form.funct_event = &form_preparation;
          fila_prepara_form.uses.vaso   = vaso;
          insert_list_event(fila_prepara_form);
       }
    }

    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
            new_event.uses.art->set_start_ociosity(SM_time_simulation);
    }
}
