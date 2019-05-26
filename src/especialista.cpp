#include "./include/especialista.h"

 u_int8_t       SM_quatd_Esp    = 0;
 Especialista** SM_Especialista = NULL;
 u_int16_t      last_id_espec   = 0;

/* STATIC METHODS */
void Especialista::add_esp(){
    if (!SM_Especialista){
        SM_Especialista    = (Especialista**) malloc(sizeof(Especialista*));
        if (!SM_Especialista){
            printf("\n================================================================================\n");
            printf("\n[ERRO] falha na alocação de memoria em Especialista::add_esp()\n\n");
            printf("\n================================================================================\n");
            exit(ERRO_MEMORY_ACESS);
        }
        SM_quatd_Esp  = 1;
        SM_Especialista[0] = this;
        return;
    }
    SM_Especialista = (Especialista**) realloc(SM_Especialista, sizeof(Especialista*)*(++SM_quatd_Esp));
    if (!SM_Especialista){
        printf("\n================================================================================\n");
        printf("\n[ERRO] falha na alocação de memoria em Especialista::add_esp()\n\n");
        printf("\n================================================================================\n");
        exit(ERRO_MEMORY_ACESS);
    }
    SM_Especialista[SM_quatd_Esp-1] = this;
}

bool Especialista::is_free(){
    for (u_int8_t i=0; i < SM_quatd_Esp; i++)
        if (SM_Especialista[i]->get_situation() == OCIOSITY_ESP)
           return true;
    return false;
}

Especialista* Especialista::get_free(){
    for (u_int8_t i=0; i < SM_quatd_Esp; i++)
        if (SM_Especialista[i]->get_situation() == OCIOSITY_ESP)
           return SM_Especialista[i];
    return NULL;
}

/* CONSTRUCTOR*/
Especialista::Especialista(){
    this->id             = last_id_espec++;
    this->situation      = state_esp::OCIOSITY_ESP;
    this->time_ociosity  = 0;
    this->start_ociosity = 0;
}
/* SET METHODS */
void Especialista::set_id(u_int8_t id){
    this->id = id;
}
void Especialista::set_situation(state_esp situation){
    this->situation = situation;
}
void Especialista::set_time_ociosity (u_int32_t time_ociosity){
    this->time_ociosity += time_ociosity;
}
void Especialista::set_start_ociosity(u_int32_t start_ociosity){
    this->start_ociosity = start_ociosity;
}
/* GET METHODS */
u_int8_t  Especialista::get_id(){
    return this->id;
}
state_esp Especialista::get_situation(){
    return this->situation;
}
u_int32_t Especialista::get_time_ociosity (){
    return this->time_ociosity;
}
u_int32_t Especialista::get_start_ociosity(){
    return this->start_ociosity;
}
