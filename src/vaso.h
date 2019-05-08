#ifndef VASO
#define  VASO

#include <ccstdlib>

#define QUEUE_WAIT 5

enum type_vaso{
    SMALL='S', MEDIUM='M', BIG='B'
};

enum name_queue{
    INICIO=0 , LIMP_ACAB_BASE, PREP_BOCA,  LIMP_ACAB_BOCA, IMP_INTER,
    ENV_GERAL,
};

class Vaso{
public:

private:
  u_int8_t  id;
  type_vaso type;
  u_int32_t start_time;
  u_int32_t end_time;
  u_int32_t queue[QUEUE_WAIT];
};

#endif
