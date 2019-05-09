#include "./include/artesao.h"

u_int16_t last_id_arts = 0;

/* CONSTRUCTOR */
Artesao::Artesao(u_int8_t id, state_art situation,
        u_int32_t time_ociosity, u_int32_t start_ociosity){
    this->id = id;
    this->situation = situation;
    this->time_ociosity = time_ociosity;
    this->start_ociosity = start_ociosity;
}
/* SET METHODS */
void Artesao::set_id(u_int8_t id){
    this->id = id;
}
void Artesao::set_situation(state_art situation){
    this->situation = situation;
}
void Artesao::set_time_ociosity (u_int32_t time_ociosity){
    this->time_ociosity = time_ociosity;
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
