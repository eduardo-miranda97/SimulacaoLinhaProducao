#include "./include/especialista.h"

 std::list<Especialista*> SM_Especialista;
 u_int16_t                last_id_espec = 0;

/* STATIC METHODS */
void Especialista::add_esp(){
    SM_Especialista.push_back(this);
}

bool Especialista::is_free(){
    for (Especialista* especialista : SM_Especialista)
        if (especialista->get_situation() == OCIOSITY_ESP)
           return true;
    return false;
}

Especialista* Especialista::get_free(){
    for (Especialista* especialista : SM_Especialista)
        if (especialista->get_situation() == OCIOSITY_ESP)
           return especialista;
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
