#include "./include/vaso.h"

 u_int32_t last_id_vaso = 0;

float Vaso::prob_small   = 0.3;
float Vaso::prob_medium  = 0.3;
float Vaso::prob_big     = 0.4;
Vaso*** SM_queue_vasos  = NULL;
u_int16_t SM_length_queue_vasos[QUEUE_WAIT];

u_int8_t rand_type();


/* CONSTRUCTOR AND FUNCTIONS*/
Vaso::Vaso(u_int32_t id, u_int32_t start_time){
    this->id         = id;
    this->type       = type;
    this->start_time = start_time;
    this->end_time   = 0;
    this->type       = Vaso::rand_type();

    for(int i=0; i < QUEUE_WAIT; i++)
      this->queue[i] = 0;
}

u_int8_t Vaso::rand_type(){
    float prob = ((float)rand()/RAND_MAX);
    if (prob <= prob_small)
       return SMALL;
    else if (prob <= (prob_medium+prob_small))
       return MEDIUM;
    return BIG;
}

u_int8_t Vaso::get_quatd_espace(){
    if (this->type == SMALL)
        return SPACE_SMALL;
    if (this->type == MEDIUM)
        return SPACE_MEDIAM;
    if (this->type == BIG)
        return SPACE_BIG;
    return SPACE_BIG;  //somente para não da warning, não é necessario.
}

/* SETS METHODS */
void Vaso::set_id(u_int32_t id){
    this->id = id;
}
void Vaso::set_type(u_int8_t type){
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
void Vaso::set_queue(u_int32_t time, u_int8_t pos){
    this->queue[pos] = time;
}
/* GETS METHODS */
u_int32_t  Vaso::get_id(){
    return this->id;
}
u_int8_t  Vaso::get_type(){
    return this->type;
}
u_int8_t Vaso::get_quatd_massa(){
    if (this->type == SMALL)
        return MASSA_SMALL;
    if (this->type == MEDIUM)
        return MASSA_MEDIAM;
    if (this->type == BIG)
        return MASSA_BIG;
    return MASSA_BIG;  //somente para não da warning, não é necessario.
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
u_int32_t  Vaso::get_queue(u_int8_t pos){
    if (pos < 0 || pos >= QUEUE_WAIT){
        std::cerr << "Error: Posição de acesso invalida, array queue (VASO)" << '\n';
        exit(ERRO_ARRAY_ACESS);
    }
    return this->queue[pos];
}
