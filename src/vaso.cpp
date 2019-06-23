#include "./include/vaso.h"

double massa_small      = 1;
double massa_median     = 2;
double massa_big        = 4;

double pedra_small      = 1;
double pedra_median     = 2;
double pedra_big        = 4;

double space_small      = 1;
double space_median     = 2;
double space_big        = 4;

float Vaso::prob_small   = 0.3;
float Vaso::prob_medium  = 0.3;
float Vaso::prob_big     = 0.4;

std::list<Vaso*>  SM_queue_vasos[QUEUE_WAIT];
u_int32_t last_id_vaso  = 0;

/* CONSTRUCTOR AND FUNCTIONS*/
Vaso::Vaso(u_int32_t start_time){
    this->id         = last_id_vaso++;
    this->start_time = start_time;
    this->end_time   = 0;
    this->type       = Vaso::rand_type();

    for (int i=0; i < QUEUE_WAIT; i++)
        this->queue[i] = 0;
}

u_int8_t Vaso::rand_type(){
    double prob = ((double)rand()/RAND_MAX);
    if (prob <= prob_small)
       return SMALL;
    else if (prob <= (prob_medium+prob_small))
       return MEDIUM;
    return    BIG;
}

u_int8_t Vaso::get_quatd_espace(){
    if (this->type == SMALL)
        return space_small;
    if (this->type == MEDIUM)
        return space_median;
    return     space_big;
}

u_int8_t Vaso::get_quatd_massa(){
    if (this->type == SMALL)
        return massa_small;
    if (this->type == MEDIUM)
        return massa_median;
    return     massa_big;
}

u_int8_t   Vaso::get_quatd_pedra(){
    if (this->type == SMALL)
        return pedra_small;
    if (this->type == MEDIUM)
        return pedra_median;
    return     pedra_big;
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
    if (pos < 0 || pos >= QUEUE_WAIT){
        std::cerr << '\n' << "[Erro] Posição de acesso invalida, array queue (VASO)" << '\n';
        exit(ERRO_ARRAY_ACESS);
    }
    this->queue[pos] = time;
}
/* GETS METHODS */
u_int32_t  Vaso::get_id(){
    return this->id;
}
u_int8_t  Vaso::get_type(){
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
u_int32_t  Vaso::get_queue(u_int8_t pos){
    if (pos < 0 || pos >= QUEUE_WAIT){
        std::cerr << '\n' << "[Erro] Posição de acesso invalida, array queue (VASO)" << '\n';
        exit(ERRO_ARRAY_ACESS);
    }
    return this->queue[pos];
}
