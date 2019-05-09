#ifndef TAD
#define  TAD

#include <cstdlib>
#include <iostream>

#include "vaso.h"

#define ERROR_MEMORY_ALLOC 2

/* IDs dos objetos usados no evento */

typedef struct{
    int16_t vaso;
    int8_t  espc;
    int8_t  arte;
}utils;

/* Descrição do evento */

typedef struct{
    u_int32_t time_event;
    void      (*funct_event)();
    utils     uses;
}event_t;


/* Estrutura de dados de uma fila, celula definida a baixo */

typedef struct queue event_list_t;
struct queue{
    event_t       event;
    event_list_t* next_event;
    event_list_t* prev_event;
};

extern event_list_t* queue_event_list; /* Fila de eventos */

/*==========================================================================*/

extern Vaso *filas[QUEUE_WAIT];

/*==========================================================================*/

 void insert_list_event(event_t event);
 void remove_list_event(event_list_t* event);

#endif
