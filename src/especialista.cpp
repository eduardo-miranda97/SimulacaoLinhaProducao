#include "./include/especialista.h"

 u_int8_t      SM_quatd_Esp    = 0;
 Especialista* SM_Especialista = NULL;
 u_int16_t     last_id_espec   = 0;

bool ESP_FREE(){
    for (u_int8_t i=0; i < SM_quatd_Esp; i++)
        if (SM_Especialista[i].get_situation() == OCIOSITY_ESP)
           return true;
    return false;
}

Especialista* GET_ESP_FREE(){
    for (u_int8_t i=0; i < SM_quatd_Esp; i++)
        if (SM_Especialista[i].get_situation() == OCIOSITY_ESP)
           return &(SM_Especialista[i]);
    return NULL;
}

/* CONSTRUCTOR*/
Especialista::Especialista(u_int8_t  id, state_esp situation,
            u_int32_t time_ociosity, u_int32_t start_ociosity){
    this->id             = id;
    this->situation      = situation;
    this->time_ociosity  = time_ociosity;
    this->start_ociosity = start_ociosity;
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
