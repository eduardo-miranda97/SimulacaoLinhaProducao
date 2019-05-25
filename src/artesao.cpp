#include "./include/artesao.h"

u_int8_t  SM_quatd_Art = 0;
Artesao** SM_Artesao   = NULL;
u_int16_t last_id_arts = 0;


/* STATIC METHODS */
void Artesao::add_art(Artesao* artesao){
    if (!SM_Artesao){
        SM_Artesao    = (Artesao**) malloc(sizeof(Artesao*));
        if (!SM_Artesao){
            printf("\n================================================================================\n");
            printf("\n[ERRO] falha na alocação de memoria em Vaso::add_art()\n\n");
            printf("\n================================================================================\n");
            exit(ERRO_MEMORY_ACESS);
        }
        SM_quatd_Art  = 1;
        SM_Artesao[0] = artesao;
        return;
    }
    SM_Artesao = (Artesao**) realloc(SM_Artesao, sizeof(Artesao*)*(++SM_quatd_Art));
    if (!SM_Artesao){
        printf("\n================================================================================\n");
        printf("\n[ERRO] falha na alocação de memoria em Vaso::add_art()\n\n");
        printf("\n================================================================================\n");
        exit(ERRO_MEMORY_ACESS);
    }
    SM_Artesao[SM_quatd_Art-1] = artesao;
}

bool Artesao::is_free(){
    for (u_int8_t i=0; i < SM_quatd_Art; i++)
        if (SM_Artesao[i]->get_situation() == OCIOSITY_ART)
            return true;
    return false;
}

Artesao* Artesao::get_free(){
  for (u_int8_t i=0; i < SM_quatd_Art; i++)
      if (SM_Artesao[i]->get_situation() == OCIOSITY_ART)
          return SM_Artesao[i];
  return NULL;
}

/* CONSTRUCTOR */
Artesao::Artesao(){
    this->id             = last_id_arts;
    this->situation      = state_art::OCIOSITY_ART;
    this->time_ociosity  = 0;
    this->start_ociosity = 0;
}
/* SET METHODS */
void Artesao::set_id(u_int8_t id){
    this->id = id;
}
void Artesao::set_situation(state_art situation){
    this->situation = situation;
}
void Artesao::set_time_ociosity (u_int32_t time_ociosity){
    this->time_ociosity += time_ociosity;
}
void Artesao::set_start_ociosity(u_int32_t start_ociosity){
    this->start_ociosity = start_ociosity;
}
/* GET METHODS */
u_int8_t  Artesao::get_id(){
    return this->id;
}
state_art Artesao::get_situation(){
    return this->situation;
}
u_int32_t Artesao::get_time_ociosity (){
    return this->time_ociosity;
}
u_int32_t Artesao::get_start_ociosity(){
    return this->start_ociosity;
}
