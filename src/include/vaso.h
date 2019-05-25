#ifndef VASO
#define  VASO

#include <iostream>
#include <cstdlib>

#define ERRO_ARRAY_ACESS        1
#define ERRO_MEMORY_ACESS       2
/* #define  ERRO_CALC          7 in triangular.h */

#define QUEUE_WAIT              6

#define MASSA_SMALL             1
#define MASSA_MEDIAM            2
#define MASSA_BIG               4

#define SPACE_SMALL             1
#define SPACE_MEDIAM            2
#define SPACE_BIG               4


class Vaso;
extern Vaso***     SM_queue_vasos;
extern u_int16_t   SM_length_queue_vasos[QUEUE_WAIT];

enum name_queue{
    PREPARA_BASE=0 , LIMP_ACAB_BASE, PREP_BOCA,  LIMP_ACAB_BOCA, IMP_INTER,
    ENV_GERAL,
};

enum type_vaso{
    SMALL='S', MEDIUM='M', BIG='B'
};

extern u_int32_t last_id_vaso;

class Vaso{
  public:
    /* VAR STATIC PUBLIC */
    static float prob_small;
    static float prob_medium;
    static float prob_big;
    /* CONSTRUCTOR AND FUNCTIONS*/
    Vaso(u_int32_t id, u_int32_t start_time);
    static u_int8_t rand_type();
    /* SETS METHODS */
    void       set_id(u_int32_t id);
    void       set_type(u_int8_t type);
    void       set_start_time(u_int32_t start_time);
    void       set_end_time(u_int32_t end_time);
    void       set_queue(u_int32_t queue[QUEUE_WAIT]);
    void       set_queue(u_int32_t time, u_int8_t pos);
    /* GETS METHODS */
    u_int32_t  get_id();
    u_int8_t   get_type();
    u_int8_t   get_quatd_massa();
    u_int8_t   get_quatd_espace();
    u_int32_t  get_start_time();
    u_int32_t  get_end_time();
    u_int32_t* get_queue();
    u_int32_t  get_queue(u_int8_t pos);

  private:
    u_int16_t id;
    u_int8_t  type;
    u_int32_t start_time;
    u_int32_t end_time;
    u_int32_t queue[QUEUE_WAIT];  /* Tempo acumulador em cada fila */
};

#endif
