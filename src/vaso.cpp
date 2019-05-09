#include "./include/vaso.h"
/* CONSTRUCTOR*/
Vaso::Vaso(u_int16_t id, type_vaso type, u_int32_t start_time){
    this->id         = id;
    this->type       = type;
    this->start_time = start_time;
    this->end_time   = 0;

    for(int i=0; i < QUEUE_WAIT; i++)
      this->queue[i] = 0;
}
/* SETS METHODS */
void Vaso::set_id(u_int16_t id){
    this->id = id;
}
void Vaso::set_type(type_vaso type){
    this->type = type;
}
void Vaso::set_start_time(u_int32_t start_time){
    this->start_time = start_time;
}
void Vaso::set_end_time(u_int32_t end_time){
    this->end_time = end_time;
}
void Vaso::set_queue(u_int32_t queue[QUEUE_WAIT]){
    for(int i=0; i < QUEUE_WAIT; i++)
        this->queue[i] = queue[i];
}
void Vaso::set_queue(u_int32_t time, name_queue pos){
    this->queue[pos] = time;
}
/* GETS METHODS */
u_int16_t  Vaso::get_id(){
    return this->id;
}
type_vaso  Vaso::get_type(){
    return this->type;
}
u_int32_t  Vaso::get_start_time(){
    return this->start_time;
}
u_int32_t  Vaso::get_end_time(){
    return this->end_time;
}
u_int32_t* Vaso::get_queue(){
    return this->queue;
}
u_int32_t  Vaso::get_queue(name_queue pos){
    if (pos < 0 || pos >= QUEUE_WAIT){
        std::cerr << "Error: Posição de acesso invalida, array queue (VASO)" << '\n';
        exit(ERROR_ACESS_ARRAY);
    }
    return this->queue[pos];
}
