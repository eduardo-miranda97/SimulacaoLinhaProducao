#include "./include/input.h"

const vector<string> split(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if((n != c)&&(n != EOF)) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}

void input_args(const string& path){
     ifstream arq;
     arq.open(path);
     vector<string> tokens;
     char         line[256];

     if (arq.is_open()){
        while (!arq.eof()){
            arq.getline(line,255);
            tokens = split(line, ' ');
						if (tokens.size()!=0)
            		set_parameters(tokens);
        }
     }else{
				 printf("\n================================================================================\n");
				 printf("\n[ERRO] Falha ao tentar abrir o arquivo '%s'\n\n", path.c_str());
				 printf("\n================================================================================\n");
				 exit(ERRO_INPUT_FILE);
     }
     arq.close();
}

void set_parameters(const vector<string>& tokens){
      if (tokens[0].compare("num_art")==0){
          SM_quatd_esp = stoul(tokens[1]);
      }else if (tokens[0].compare("num_esp")==0){
          SM_quatd_esp = stoul(tokens[1]);
      }else if (tokens[0].compare("esp_sec")==0){
          SM_espaco_secagem = stoul(tokens[1]);
      }else if (tokens[0].compare("prob_ped_g")==0){
          Vaso::prob_big = stoul(tokens[1]);
      }else if (tokens[0].compare("prob_ped_m")==0){
          Vaso::prob_medium = stoul(tokens[1]);
      }else if (tokens[0].compare("prob_ped_p")==0){
          Vaso::prob_small = stoul(tokens[1]);
      }else if (tokens[0].compare("tam_ped")==0){
					if (tokens.size()==4){
							SM_quatd_vasos.time_min  = stoul(tokens[1]);
							SM_quatd_vasos.time_mode = stoul(tokens[2]);
							SM_quatd_vasos.time_max  = stoul(tokens[3]);
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("freq_ped")==0){
					if (tokens.size()==4){
							SM_quatd_pedidos.time_min  = stoul(tokens[1]);
							SM_quatd_pedidos.time_mode = stoul(tokens[2]);
							SM_quatd_pedidos.time_max  = stoul(tokens[3]);
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("prep_form")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["prep_form-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["prep_form-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["prep_form-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("prep_base")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["prep_base-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["prep_base-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["prep_base-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("acab_ini_base")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["acab_ini_base-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["acab_ini_base-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["acab_ini_base-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("sec_acab_base")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["sec_acab_base-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["sec_acab_base-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["sec_acab_base-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("limp_acab_base")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["limp_acab_base-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["limp_acab_base-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["limp_acab_base-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("secagem_base")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["secagem_base-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["secagem_base-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["secagem_base-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("imp_interna")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["imp_interna-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["imp_interna-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["imp_interna-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("prep_boca")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["prep_boca-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["prep_boca-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["prep_boca-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("acab_ini_boca")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["acab_ini_boca-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["acab_ini_boca-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["acab_ini_boca-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("sec_acab_boca")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["sec_acab_boca-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["sec_acab_boca-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["sec_acab_boca-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("limp_acab_boca")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["limp_acab_boca-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["limp_acab_boca-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["limp_acab_boca-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("secagem_boca")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["secagem_boca-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["secagem_boca-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["secagem_boca-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("imp_interna")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["imp_interna-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["imp_interna-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["imp_interna-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("sec_interna")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["sec_interna-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["sec_interna-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["sec_interna-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("env_geral")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["env_geral-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["env_geral-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["env_geral-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else if (tokens[0].compare("sec_final")==0){
					if (tokens.size()==10){
							times_triangular_t times;
							times.time_min  = stoul(tokens[1]);
							times.time_mode = stoul(tokens[2]);
							times.time_max  = stoul(tokens[3]);
							SM_times_events["sec_final-s"] = times;//{tokens[1], tokens[2], tokens[3]};
							times.time_min  = stoul(tokens[4]);
							times.time_mode = stoul(tokens[5]);
							times.time_max  = stoul(tokens[6]);
							SM_times_events["sec_final-m"] = times;//{tokens[4], tokens[5], tokens[6]};
							times.time_min  = stoul(tokens[7]);
							times.time_mode = stoul(tokens[8]);
							times.time_max  = stoul(tokens[9]);
							SM_times_events["sec_final-b"] = times;//{tokens[7], tokens[8], tokens[9]};
					}else{
							printf("\n================================================================================\n");
							printf("\n[ERRO] Entrada de argumentos via arquivo, token %s\n\n", tokens[0].c_str());
							printf("\n================================================================================\n");
							exit(ERRO_INPUT_ARGS);
					}
      }else{
					printf("\n================================================================================\n");
					printf("\n[ERRO] Numero de parametro menor que um, token %s\n\n", tokens[0].c_str());
					printf("\n================================================================================\n");
					exit(ERRO_INPUT_ARGS);
			}
}
