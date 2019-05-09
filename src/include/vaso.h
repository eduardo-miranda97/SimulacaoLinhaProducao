#ifndef VASO
#define  VASO

#include <iostream>
#include <cstdlib>

#define QUEUE_WAIT 6
#define ERROR_ACESS_ARRAY 1

enum type_vaso{
    SMALL='S', MEDIUM='M', BIG='B'
};

enum name_queue{
    INICIO=0 , LIMP_ACAB_BASE, PREP_BOCA,  LIMP_ACAB_BOCA, IMP_INTER,
    ENV_GERAL,
};

u_int16_t last_id_vaso;

class Vaso{
  public:
    /* CONSTRUCTOR*/
    Vaso(u_int16_t id, type_vaso type, u_int32_t start_time);
    /* SETS METHODS */
    void set_id(u_int16_t id);
    void set_type(type_vaso type);
    void set_start_time(u_int32_t start_time);
    void set_end_time(u_int32_t end_time);
    void set_queue(u_int32_t queue[QUEUE_WAIT]);
    void set_queue(u_int32_t time, name_queue pos);
    /* GETS METHODS */
    u_int16_t  get_id();
    type_vaso  get_type();
    u_int32_t  get_start_time();
    u_int32_t  get_end_time();
    u_int32_t* get_queue();
    u_int32_t  get_queue(name_queue pos);

  private:
    u_int16_t id;
    type_vaso type;
    u_int32_t start_time;
    u_int32_t end_time;
    u_int32_t queue[QUEUE_WAIT];  /* Tempo acumulador em cada fila */
};

#endif
