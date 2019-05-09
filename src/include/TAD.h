#ifndef TAD
#define  TAD

#include <ccstdlib>
#include "vaso.h"

typedef struct{
    int8_t vaso;
    int8_t espc;
    int8_t arte;
}utils;

typedef struct{
  u_int32_t time_event;
  void (*funct_event)();
  utils uses;
}event_t;

typedef struct queue queue_time_priority_t;

struct queue{
  event_t event;
  queue_time_priority_t* next_event;
  queue_time_priority_t* prev_event;
};

queue_time_priority_t* event_list;

/*==========================================================================*/

Vaso *filas[QUEUE_WAIT];

#endif
