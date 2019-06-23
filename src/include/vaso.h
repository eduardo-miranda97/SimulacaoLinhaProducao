#ifndef VASO
#define  VASO

#include <iostream>
#include <cstdlib>
#include <list>

#define ERRO_ARRAY_ACESS        1
#define ERRO_MEMORY_ACESS       2
#define ERRO_EXEC               3
#define ERRO_INPUT_ARGS         4
#define ERRO_INPUT_FILE         5
#define ERRO_INPUT_FILE_ARGS    6
/* #define  ERRO_CALC           7 in triangular.h */
#define ERRO_MEMORY_ALLOC       8

#define QUEUE_WAIT              6

extern double massa_small;
extern double massa_median;
extern double massa_big;

extern double pedra_small;
extern double pedra_median;
extern double pedra_big;

extern double space_small;
extern double space_median;
extern double space_big;

class Vaso;

extern std::list<Vaso*>  SM_queue_vasos[QUEUE_WAIT];

enum name_queue{
    PREPARA_FORM=0, LIMP_ACAB_BASE, PREP_BOCA,  LIMP_ACAB_BOCA, IMP_INTER,
    ENV_GERAL
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
    Vaso (u_int32_t start_time);
    static u_int8_t rand_type();

    /* SETS METHODS */
    void       set_id         (u_int32_t id);
    void       set_type       (u_int8_t  type);
    void       set_start_time (u_int32_t start_time);
    void       set_end_time   (u_int32_t end_time);
    void       set_queue      (u_int32_t queue[QUEUE_WAIT]);
    void       set_queue      (u_int32_t time, u_int8_t pos);

    /* GETS METHODS */
    u_int32_t  get_id           ();
    u_int8_t   get_type         ();
    u_int8_t   get_quatd_massa  ();
    u_int8_t   get_quatd_pedra  ();
    u_int8_t   get_quatd_espace ();
    u_int32_t  get_start_time   ();
    u_int32_t  get_end_time     ();
    u_int32_t* get_queue        ();
    u_int32_t  get_queue        (u_int8_t pos);

  private:
    u_int16_t id;
    u_int32_t start_time;
    u_int32_t end_time;
    u_int32_t queue[QUEUE_WAIT];  /* Tempo acumulador em cada fila */
    u_int8_t  type;
};

#endif
