#include "./include/atividade.h"

#define DAYINMINUTES             (24*60)

void init_simulation(){
    double  start_time;
    event_t event;
    event.uses.vaso = NULL;
    event.uses.art  = NULL;
    event.uses.esp  = NULL;
    // for (start_time = 8;start_time < SM_final_time_simulation; start_time += 24){
    //     event.time_event  = start_time;
    //     event.funct_event = &new_day;
    //     insert_list_event(event);
    // }
    start_time = 0;
    event.funct_event = &begin_requets;
    #if LOG
        file_log.open("Log.txt");
    #endif
    do{
        #if LOG
            file_log << "Agendado CHEGADA_PEDIDO TIME: " << start_time << std::endl;
        #endif
        event.time_event = start_time;
        insert_list_event(event);
        u_int8_t quatd_pedidos = trand(SM_quatd_pedidos);
        start_time            += (quatd_pedidos*DAYINMINUTES);
    }while(start_time < SM_final_time_simulation);
}

void begin_requets(){
      SM_time_simulation = SM_list_event_simulation[0].event.time_event;
      remove_list_event(&(SM_list_event_simulation[0]));

      u_int16_t quatd_vasos = trand(SM_quatd_vasos);
      #if LOG
          file_log << std::endl << "CHEGADA_PEDIDO num_vasos: " << quatd_vasos << " TIME: " << SM_time_simulation << std::endl;
      #endif
      for (u_int16_t i=0; i < quatd_vasos; i++){
          Vaso* vaso = new Vaso(SM_time_simulation);
          if (!vaso){
              printf("\n================================================================================\n");
              printf("\n[ERRO] falha na alocação de memoria em begin_requets()\n\n");
              printf("\n================================================================================\n");
              exit(ERRO_MEMORY_ACESS);
          }
          #if LOG
              file_log << "Instanciado vaso ID: " << vaso->get_id() << std::endl;
          #endif
          if (Artesao::is_free() || Especialista::is_free())
             if (SM_massa  >=  vaso->get_quatd_massa()){
              if (SM_pedra >= vaso->get_quatd_pedra()){
               if (SM_espaco_secagem >= vaso->get_quatd_espace()){
                      SM_massa          -= vaso->get_quatd_massa();
                      SM_pedra_usado    += vaso->get_quatd_pedra();
                      SM_massa_usado    += vaso->get_quatd_massa();
                      SM_pedra          -= vaso->get_quatd_pedra();
                      SM_espaco_secagem -= vaso->get_quatd_espace();

                      event_t prep_form;
                      if (vaso->get_type() == SMALL){
                          prep_form.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-s"]));
                      }else if (vaso->get_type() == MEDIUM){
                          prep_form.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-m"]));
                      }else{
                          prep_form.time_event  = (SM_time_simulation+trand(SM_times_events["prep_form-b"]));
                      }

                      if (Especialista::is_free()){
                          Especialista* especialista = Especialista::get_free();
                          especialista->set_situation(state_esp::ACTIVE_ESP);
                          // especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());
                          especialista->set_start_ociosity(SM_time_simulation);

                          prep_form.funct_event = &form_preparation;
                          prep_form.uses.vaso   = vaso;
                          prep_form.uses.art    = NULL;
                          prep_form.uses.esp    = especialista;
                          #if LOG
                              file_log << "Agendado PREPARA_FORMA vaso ID: " << vaso->get_id()
                                      << " Especialista ID: "               << especialista->get_id()
                                      << " TIME: "                          << prep_form.time_event
                                      << std::endl;
                          #endif
                      }else{
                          Artesao* artesao = Artesao::get_free();
                          artesao->set_situation(state_art::ACTIVE_ART);
                          // artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());
                          artesao->set_start_ociosity(SM_time_simulation);
                          prep_form.funct_event = &form_preparation;
                          prep_form.uses.vaso   = vaso;
                          prep_form.uses.art    = artesao;
                          prep_form.uses.esp    = NULL;
                          #if LOG
                              file_log << "Agendado PREPARA_FORMA vaso ID: " << vaso->get_id()
                                      << " Artesao ID: "                    << artesao->get_id()
                                      << " TIME: "                          << prep_form.time_event
                                      << std::endl;
                          #endif
                      }
                      insert_list_event(prep_form);
                      continue;
               }
              }
             }
          #if LOG
              file_log << "Colocando em FILA da PREPARA_FORMA vaso ID: " << vaso->get_id()
                      << std::endl;
          #endif
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

    #if LOG
        file_log << std::endl << "PREPARA_FORMA TIME: " << SM_time_simulation << std::endl
                << "Agendado PREPARA_BASE vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event   << std::endl;
    #endif

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

    #if LOG
        file_log << std::endl << "PREPARA_BASE TIME: " << SM_time_simulation << std::endl
                << "Agendado PREPARA_ACAB_BASE vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event  << std::endl;
    #endif

    new_event.funct_event = &base_set_init;
    insert_list_event(new_event);
}

void base_set_init(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "PREPARA_ACAB_BASE TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= SM_massa_MIN){
            flag = false;
            event_t prep_massa = new_event;
            prep_massa.time_event += trand(SM_times_events["prep_massa"]);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            #if LOG
                file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_massa.time_event << std::endl;
            #endif
            goto SEC_ACAB_BASE_LABEL;

        }else if (SM_pedra <= SM_pedra_MIN){
            flag = false;
            event_t prep_pedra = new_event;
            prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            #if LOG
                file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_pedra.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_env_geral.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_imp_inter.time_event << std::endl;
            #endif
            goto SEC_ACAB_BASE_LABEL;
        }
    }
    if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
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

        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif

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

        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif

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

        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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

          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif

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
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso especialista  ID: " << new_event.uses.esp->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }
    }

    #if LOG
        file_log << "Agendado SECAGEM_ACABAMENTO_BASE vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif

    new_event.funct_event = &base_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void base_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "SECAGEM_ACABAMENTO_BASE TIME: " << SM_time_simulation << std::endl;
    #endif

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["sec_acab_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["sec_acab_base-b"]);
    }
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_situation(state_esp::ACTIVE_ESP);
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

        new_event.funct_event = &base_clearing;
        new_event.uses.esp    = especialista;
        #if LOG
            file_log << "Agendado LIMP_BASE vaso ID: " << new_event.uses.vaso->get_id()
                    << " Especialista ID: "               << especialista->get_id()
                    << " TIME: "                          << new_event.time_event
                    << std::endl;
        #endif
        insert_list_event(new_event);
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_situation(state_art::ACTIVE_ART);
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        new_event.funct_event = &base_clearing;
        new_event.uses.art    = artesao;

        #if LOG
            file_log << "Agendado LIMP_BASE vaso ID: " << new_event.uses.vaso->get_id()
                    << " Artesao ID: "                   << artesao->get_id()
                    << " TIME: "                         << new_event.time_event
                    << std::endl;
        #endif

        insert_list_event(new_event);
        return;
    }
    #if LOG
        file_log << "Colocando na FILA LIMP_ACAB_BASE vaso ID: " << new_event.uses.vaso->get_id() << std::endl;
    #endif
    new_event.uses.vaso->set_queue(SM_time_simulation, LIMP_ACAB_BASE);
    SM_queue_vasos[LIMP_ACAB_BASE].push_back(new_event.uses.vaso);
}

void base_clearing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));
    #if LOG
        file_log << std::endl << "LIMP_BASE TIME: " << SM_time_simulation << std::endl;
    #endif
    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= SM_massa_MIN){
            flag = false;
            event_t prep_massa = new_event;
            prep_massa.time_event += trand(SM_times_events["prep_massa"]);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            #if LOG
                file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_massa.time_event << std::endl;
            #endif
            goto LIMP_BASE;

        }else if (SM_pedra <= SM_pedra_MIN){
            flag = false;
            event_t prep_pedra = new_event;
            prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            #if LOG
                file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_pedra.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_env_geral.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_imp_inter.time_event << std::endl;
            #endif
            goto LIMP_BASE;
        }
    }
    if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
            SM_espaco_secagem -= vaso->get_quatd_espace();
            SM_massa          -= vaso->get_quatd_massa();
            SM_pedra          -= vaso->get_quatd_pedra();
            SM_pedra_usado    += vaso->get_quatd_pedra();
            SM_massa_usado    += vaso->get_quatd_massa();
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
            #if LOG
                file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado PREPARA_FORMA ";
                if (new_event.uses.art)
                    file_log << "artesao ID: " << new_event.uses.art->get_id();
                else
                    file_log << "especialista ID: " << new_event.uses.esp->get_id();
                file_log << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_prepara_form.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso especialista  ID: " << new_event.uses.esp->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }
    }

    new_event.funct_event = &base_finish;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    #if LOG
        file_log << "Agendado ACABAMENTO_BASE vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif
    insert_list_event(new_event);
}

void base_finish(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "ACABAMENTO_BASE TIME: " << SM_time_simulation << std::endl;
    #endif

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["secagem_base-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["secagem_base-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["secagem_base-b"]);
    }
    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_situation(state_esp::ACTIVE_ESP);
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

        new_event.funct_event = &mouth_preparation;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        #if LOG
            file_log << "Agendado PREPARA_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                    << " Especialista ID: "               << especialista->get_id()
                    << " TIME: "                          << new_event.time_event
                    << std::endl;
        #endif
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_situation(state_art::ACTIVE_ART);
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        new_event.funct_event = &mouth_preparation;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        #if LOG
            file_log << "Agendado PREPARA_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                    << " Artesao ID: "                   << artesao->get_id()
                    << " TIME: "                         << new_event.time_event
                    << std::endl;
        #endif
        return;
    }
    #if LOG
        file_log << "Colocando em FILA da PREPARA_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                << std::endl;
    #endif
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
    #if LOG
        file_log << std::endl << "PREPARA_BOCA TIME: " << SM_time_simulation << std::endl
                << "Agendado ACAB_INICIAL_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event   << std::endl;
    #endif
    insert_list_event(new_event);
}

void mouth_set_init(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "PREPARA_ACAB_BOCA TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= SM_massa_MIN){
            flag = false;
            event_t prep_massa = new_event;
            prep_massa.time_event += trand(SM_times_events["prep_massa"]);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            #if LOG
                file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_massa.time_event << std::endl;
            #endif
            goto ACAB_INICIAL_BOCA;

        }else if (SM_pedra <= SM_pedra_MIN){
            flag = false;
            event_t prep_pedra = new_event;
            prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            #if LOG
                file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_pedra.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_env_geral.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_imp_inter.time_event << std::endl;
            #endif
            goto ACAB_INICIAL_BOCA;
        }
    }
    if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
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
          #if LOG

              file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado LIMP_ACAB_BOCA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
          #endif
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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
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
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso especialista  ID: " << new_event.uses.esp->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }
    }

    #if LOG
        file_log << "Agendado SECAGEGEM_ACAB_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif

    new_event.funct_event = &mouth_set_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void mouth_set_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "SECAGEGEM_ACAB_BOCA TIME: " << SM_time_simulation << std::endl;
    #endif

    if (new_event.uses.vaso->get_type() == SMALL){
        new_event.time_event  += trand(SM_times_events["sec_acab_boca-s"]);
    }else if (new_event.uses.vaso->get_type() == MEDIUM){
        new_event.time_event  += trand(SM_times_events["sec_acab_boca-m"]);
    }else{
        new_event.time_event  += trand(SM_times_events["sec_acab_boca-b"]);
    }

    if (Especialista::is_free()){
        Especialista* especialista  = Especialista::get_free();
        especialista->set_situation(state_esp::ACTIVE_ESP);
        especialista->set_time_ociosity(SM_time_simulation-especialista->get_start_ociosity());

        new_event.funct_event = &mount_clearing;
        new_event.uses.esp    = especialista;
        insert_list_event(new_event);
        #if LOG
            file_log << "Agendado LIMP_BOCA especialista ID: " << especialista->get_id()
                    << " vaso ID: " << new_event.uses.vaso->get_id()
                    << " TIME: " << new_event.time_event << std::endl;
        #endif
        return;
    }
    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_situation(state_art::ACTIVE_ART);
        artesao->set_time_ociosity(SM_time_simulation-artesao->get_start_ociosity());

        new_event.funct_event = &mount_clearing;
        new_event.uses.art    = artesao;
        insert_list_event(new_event);
        #if LOG
            file_log << "Agendado LIMP_BOCA artesao ID: " << artesao->get_id()
                    << " vaso ID: " << new_event.uses.vaso->get_id()
                    << " TIME: " << new_event.time_event << std::endl;
        #endif
        return;
    }
    #if LOG
        file_log << "Colocando na FILA LIMP_BOCA vaso ID: " << new_event.uses.vaso->get_id() << std::endl;
    #endif
    new_event.uses.vaso->set_queue(SM_time_simulation, LIMP_ACAB_BOCA);
    SM_queue_vasos[LIMP_ACAB_BOCA].push_back(new_event.uses.vaso);
}

void mount_clearing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "LIMP_BOCA TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    if (new_event.uses.art){
        if (SM_massa <= SM_massa_MIN){
            flag = false;
            event_t prep_massa = new_event;
            prep_massa.time_event += trand(SM_times_events["prep_massa"]);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            insert_list_event(prep_massa);
            #if LOG
                file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_massa.time_event << std::endl;
            #endif
            goto LIMP_BOCA;

        }else if (SM_pedra <= SM_pedra_MIN){
            flag = false;
            event_t prep_pedra = new_event;
            prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            insert_list_event(prep_pedra);
            #if LOG
                file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_pedra.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_env_geral.time_event << std::endl;
            #endif
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
            #if LOG
                file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_imp_inter.time_event << std::endl;
            #endif
            goto LIMP_BOCA;
        }
    }
    if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif
        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
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
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }else{
            new_event.uses.esp->set_situation(state_esp::OCIOSITY_ESP);
            new_event.uses.esp->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso especialista  ID: " << new_event.uses.esp->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
        }
    }


    new_event.funct_event = &mount_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    #if LOG
        file_log << "Agendado SECAGEM_BOCA vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif
    insert_list_event(new_event);
}

void mount_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "SECAGEM_BOCA TIME: " << SM_time_simulation << std::endl;
    #endif

    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_situation(state_art::ACTIVE_ART);
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
        #if LOG
            file_log << "Agendado IMPERMEABILIZAÇÃO vaso ID: " << new_event.uses.vaso->get_id()
                    << " Artesao ID: "                   << artesao->get_id()
                    << " TIME: "                         << new_event.time_event
                    << std::endl;
        #endif
        return;
    }
    #if LOG
        file_log << "Colocando em FILA da IMPERMEABILIZAÇÃO vaso ID: " << new_event.uses.vaso->get_id()
                << std::endl;
    #endif
    new_event.uses.vaso->set_queue(SM_time_simulation, IMP_INTER);
    SM_queue_vasos[IMP_INTER].push_back(new_event.uses.vaso);
}

void inter_waterpoofing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "PREPARA_ACAB_BASE TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    if (SM_massa <= SM_massa_MIN){
        flag = false;
        event_t prep_massa = new_event;
        prep_massa.time_event += trand(SM_times_events["prep_massa"]);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        #if LOG
            file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_massa.time_event << std::endl;
        #endif
        insert_list_event(prep_massa);

    }else if (SM_pedra <= SM_pedra_MIN){
        flag = false;
        event_t prep_pedra = new_event;
        prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        #if LOG
            file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_pedra.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_env_geral.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_imp_inter.time_event << std::endl;
        #endif
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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif
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

        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
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
          #if LOG
              file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                      << " TIME: " << new_event.time_event << std::endl;
          #endif
    }

    #if LOG
        file_log << "Agendado SECAGEM_INTERNA vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif

    new_event.funct_event = &inter_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    insert_list_event(new_event);
}

void inter_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "SECAGEM_INTERNA TIME: " << SM_time_simulation << std::endl;
    #endif

    if (Artesao::is_free()){
        Artesao* artesao  = Artesao::get_free();
        artesao->set_situation(state_art::ACTIVE_ART);
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
        #if LOG
            file_log << "Agendado ENVERNIZAÇÂO artesao ID: " << artesao->get_id()
                    << " vaso ID: " << new_event.uses.vaso->get_id()
                    << " TIME: " << new_event.time_event << std::endl;
        #endif
        return;
    }
    #if LOG
        file_log << "Colocando na FILA ENVERNIZAÇÂO vaso ID: " << new_event.uses.vaso->get_id() << std::endl;
    #endif
    new_event.uses.vaso->set_queue(SM_time_simulation, ENV_GERAL);
    SM_queue_vasos[ENV_GERAL].push_back(new_event.uses.vaso);
}

void varnishing(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    bool flag = true;
    if (SM_massa <= SM_massa_MIN){
        flag = false;
        event_t prep_massa = new_event;
        prep_massa.time_event += trand(SM_times_events["prep_massa"]);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        #if LOG
            file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_massa.time_event << std::endl;
        #endif
        insert_list_event(prep_massa);

    }else if (SM_pedra <= SM_pedra_MIN){
        flag = false;
        event_t prep_pedra = new_event;
        prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        #if LOG
            file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_pedra.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_env_geral.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_imp_inter.time_event << std::endl;
        #endif
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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif
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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif
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

        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
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
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
    }


    new_event.funct_event = &final_drying;
    new_event.uses.art    = NULL;
    new_event.uses.esp    = NULL;
    #if LOG
        file_log << "Agendado SECAGEM_FINAL vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << new_event.time_event << std::endl;
    #endif
    insert_list_event(new_event);
}

void final_drying(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "SECAGEM_FINAL TIME: " << SM_time_simulation << std::endl;
    #endif

    SM_espaco_secagem += new_event.uses.vaso->get_quatd_espace();

    if (Artesao::is_free()){
        if (SM_massa <= SM_massa_MIN){
            event_t prep_massa = new_event;
            prep_massa.time_event += trand(SM_times_events["prep_massa"]);
            prep_massa.funct_event = &preparation_massa;
            prep_massa.uses.vaso   = NULL;
            prep_massa.uses.art    = Artesao::get_free();
            prep_massa.uses.art->set_situation(state_art::ACTIVE_ART);
            prep_massa.uses.art->set_time_ociosity(SM_time_simulation-prep_massa.uses.art->get_start_ociosity());
            insert_list_event(prep_massa);
            #if LOG
                file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_massa.time_event << std::endl;
            #endif
            goto SECAGEM_FINAL;

        }else if (SM_pedra <= SM_pedra_MIN){
            event_t prep_pedra = new_event;
            prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
            prep_pedra.funct_event = &preparation_pedras;
            prep_pedra.uses.vaso   = NULL;
            prep_pedra.uses.art    = Artesao::get_free();
            prep_pedra.uses.art->set_situation(state_art::ACTIVE_ART);
            prep_pedra.uses.art->set_time_ociosity(SM_time_simulation-prep_pedra.uses.art->get_start_ociosity());
            insert_list_event(prep_pedra);
            #if LOG
                file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << prep_pedra.time_event << std::endl;
            #endif
            goto SECAGEM_FINAL;

        }else if (SM_queue_vasos[ENV_GERAL].size()){
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
            fila_env_geral.uses.art    = Artesao::get_free();
            fila_env_geral.uses.art->set_situation(state_art::ACTIVE_ART);
            fila_env_geral.uses.art->set_time_ociosity(SM_time_simulation-fila_env_geral.uses.art->get_start_ociosity());
            insert_list_event(fila_env_geral);
            #if LOG
                file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_env_geral.time_event << std::endl;
            #endif
            goto SECAGEM_FINAL;

        }else if (SM_queue_vasos[IMP_INTER].size()){
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
            fila_imp_inter.uses.art    = Artesao::get_free();
            fila_imp_inter.uses.art->set_situation(state_art::ACTIVE_ART);
            fila_imp_inter.uses.art->set_time_ociosity(SM_time_simulation-fila_imp_inter.uses.art->get_start_ociosity());
            insert_list_event(fila_imp_inter);
            #if LOG
                file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                        << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_imp_inter.time_event << std::endl;
            #endif
            goto SECAGEM_FINAL;

        }else if (SM_queue_vasos[LIMP_ACAB_BOCA].size()){
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
            fila_limp_acab_boca.uses.art    = Artesao::get_free();
            fila_limp_acab_boca.uses.art->set_situation(state_art::ACTIVE_ART);
            fila_limp_acab_boca.uses.art->set_time_ociosity(SM_time_simulation-fila_limp_acab_boca.uses.art->get_start_ociosity());
            insert_list_event(fila_limp_acab_boca);
            #if LOG

                file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                        << "Agendado LIMP_ACAB_BOCA ";
                if (new_event.uses.art)
                    file_log << "artesao ID: " << new_event.uses.art->get_id();
                else
                    file_log << "especialista ID: " << new_event.uses.esp->get_id();
                file_log << " vaso ID: " << vaso->get_id()
                        << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
            #endif
            goto SECAGEM_FINAL;
        }
    }
    if (Artesao::is_free() || Especialista::is_free())
      if (SM_queue_vasos[PREP_BOCA].size()){
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
        if (Especialista::is_free()){
            fila_prep_boca.uses.esp = Especialista::get_free();
            fila_prep_boca.uses.esp->set_situation(state_esp::ACTIVE_ESP);
            fila_prep_boca.uses.esp->set_time_ociosity(SM_time_simulation-fila_prep_boca.uses.esp->get_start_ociosity());
        }else{
            fila_prep_boca.uses.art = Artesao::get_free();
            fila_prep_boca.uses.art->set_situation(state_art::ACTIVE_ART);
            fila_prep_boca.uses.art->set_time_ociosity(SM_time_simulation-fila_prep_boca.uses.art->get_start_ociosity());
        }
        insert_list_event(fila_prep_boca);
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif

    }else if (SM_queue_vasos[LIMP_ACAB_BASE].size()){
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
        if (Especialista::is_free()){
            fila_limp_acab_base.uses.esp = Especialista::get_free();
            fila_limp_acab_base.uses.esp->set_situation(state_esp::ACTIVE_ESP);
            fila_limp_acab_base.uses.esp->set_time_ociosity(SM_time_simulation-fila_limp_acab_base.uses.esp->get_start_ociosity());
        }else{
            fila_limp_acab_base.uses.art = Artesao::get_free();
            fila_limp_acab_base.uses.art->set_situation(state_art::ACTIVE_ART);
            fila_limp_acab_base.uses.art->set_time_ociosity(SM_time_simulation-fila_limp_acab_base.uses.art->get_start_ociosity());
        }
        insert_list_event(fila_limp_acab_base);
        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          if (Especialista::is_free()){
              fila_prepara_form.uses.esp = Especialista::get_free();
              fila_prepara_form.uses.esp->set_situation(state_esp::ACTIVE_ESP);
              fila_prepara_form.uses.esp->set_time_ociosity(SM_time_simulation-fila_prepara_form.uses.esp->get_start_ociosity());
          }else{
              fila_prepara_form.uses.art = Artesao::get_free();
              fila_prepara_form.uses.art->set_situation(state_art::ACTIVE_ART);
              fila_prepara_form.uses.art->set_time_ociosity(SM_time_simulation-fila_prepara_form.uses.art->get_start_ociosity());
          }
          insert_list_event(fila_prepara_form);
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (fila_prepara_form.uses.art)
                  file_log << "artesao ID: " << fila_prepara_form.uses.art->get_id();
              else
                  file_log << "especialista ID: " << fila_prepara_form.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
       }
    }

SECAGEM_FINAL:
    #if LOG
        file_log << "Vaso Finalizado vaso ID: " << new_event.uses.vaso->get_id()
                << " TIME: " << SM_time_simulation << std::endl;
    #endif
    new_event.uses.vaso->set_end_time(SM_time_simulation);
    SM_vaso_finish.push_back(new_event.uses.vaso);
}

void preparation_massa(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "PREPARA_MASSA TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    SM_massa = SM_massa_MAX;
    if (SM_massa <= SM_massa_MIN){
        flag = false;
        event_t prep_massa = new_event;
        prep_massa.time_event += trand(SM_times_events["prep_massa"]);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        insert_list_event(prep_massa);
        #if LOG
            file_log << std::endl << "SECAGEM_ACABAMENTO_BASE TIME: " << SM_time_simulation << std::endl;
        #endif

    }else if (SM_pedra <= SM_pedra_MIN){
        flag = false;
        event_t prep_pedra = new_event;
        prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        insert_list_event(prep_pedra);
        #if LOG
            file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_pedra.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_env_geral.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_imp_inter.time_event << std::endl;
        #endif
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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif

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

        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
       }
    }
    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
                new_event.uses.art->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
    }
}

void preparation_pedras(){
    SM_time_simulation = SM_list_event_simulation[0].event.time_event;
    event_t new_event  = SM_list_event_simulation[0].event;
    remove_list_event(&(SM_list_event_simulation[0]));

    #if LOG
        file_log << std::endl << "PREPARA_PEDRA TIME: " << SM_time_simulation << std::endl;
    #endif

    bool flag = true;
    SM_pedra = SM_pedra_MAX;
    if (SM_massa <= SM_massa_MIN){
        flag = false;
        event_t prep_massa = new_event;
        prep_massa.time_event += trand(SM_times_events["prep_massa"]);
        prep_massa.funct_event = &preparation_massa;
        prep_massa.uses.vaso   = NULL;
        insert_list_event(prep_massa);
        #if LOG
            file_log << "Agendado PREPARA_MASSA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_massa.time_event << std::endl;
        #endif

    }else if (SM_pedra <= SM_pedra_MIN){
        flag = false;
        event_t prep_pedra = new_event;
        prep_pedra.time_event += trand(SM_times_events["prep_pedra"]);
        prep_pedra.funct_event = &preparation_pedras;
        prep_pedra.uses.vaso   = NULL;
        insert_list_event(prep_pedra);
        #if LOG
            file_log << "Agendado PREPARA_PEDRA artesao ID: " << new_event.uses.art->get_id()
                    << " TIME: " << prep_pedra.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA ENVERNIZAÇÂO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado ENVERNIZAÇÂO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_env_geral.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA IMPERMEABILIZAÇÃO vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado IMPERMEABILIZAÇÃO artesao ID: " << new_event.uses.art->get_id()
                    << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_imp_inter.time_event << std::endl;
        #endif

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
        #if LOG

            file_log << "Remove FILA LIMP_ACAB_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_boca.time_event << std::endl;
        #endif

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
        #if LOG
            file_log << "Remove FILA PREPARA_BOCA vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado PREPARA_BOCA ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_prep_boca.time_event << std::endl;
        #endif

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

        #if LOG
            file_log << "Remove FILA LIMP_ACAB_BASE vaso ID: " << vaso->get_id() << std::endl
                    << "Agendado LIMP_ACAB_BASE ";
            if (new_event.uses.art)
                file_log << "artesao ID: " << new_event.uses.art->get_id();
            else
                file_log << "especialista ID: " << new_event.uses.esp->get_id();
            file_log << " vaso ID: " << vaso->get_id()
                    << " TIME: " << fila_limp_acab_base.time_event << std::endl;
        #endif

        insert_list_event(fila_limp_acab_base);

    }else if (SM_queue_vasos[PREPARA_FORM].size()){
      Vaso* vaso = SM_queue_vasos[PREPARA_FORM].front();
      if (SM_massa   >= vaso->get_quatd_massa())
       if (SM_pedra >= vaso->get_quatd_pedra())
        if (SM_espaco_secagem >= vaso->get_quatd_espace()){
          SM_espaco_secagem -= vaso->get_quatd_espace();
          SM_massa          -= vaso->get_quatd_massa();
          SM_pedra          -= vaso->get_quatd_pedra();
          SM_pedra_usado    += vaso->get_quatd_pedra();
          SM_massa_usado    += vaso->get_quatd_massa();
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
          #if LOG
              file_log << "Remove FILA PREPARA_FORMA vaso ID: " << vaso->get_id() << std::endl
                      << "Agendado PREPARA_FORMA ";
              if (new_event.uses.art)
                  file_log << "artesao ID: " << new_event.uses.art->get_id();
              else
                  file_log << "especialista ID: " << new_event.uses.esp->get_id();
              file_log << " vaso ID: " << vaso->get_id()
                      << " TIME: " << fila_prepara_form.time_event << std::endl;
          #endif
       }
    }

    if (flag){
            new_event.uses.art->set_situation(state_art::OCIOSITY_ART);
                new_event.uses.art->set_start_ociosity(SM_time_simulation);
            #if LOG
                file_log << "Estado Ocioso artesao ID: " << new_event.uses.art->get_id()
                        << " TIME: " << new_event.time_event << std::endl;
            #endif
    }
}
