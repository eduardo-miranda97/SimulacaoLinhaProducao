#ifndef TAD
#define  TAD

#include "vaso.h"
#include "especialista.h"
#include "artesao.h"

/* IDs dos objetos usados no evento */
typedef struct{
    Vaso*          vaso;
    Artesao*       art;
    Especialista*  esp;
}utils;

/* Descrição do evento */
typedef struct{
    double  time_event;
    void    (*funct_event)();
    utils   uses;
}event_t;


/* Estrutura de dados de uma fila, celula definida a baixo */
typedef struct queue event_list_t;
struct queue{
    event_t       event;
    event_list_t* next_event;
    event_list_t* prev_event;
};

/*==========================================================================*/

extern event_list_t* SM_list_event_simulation; /* Fila de eventos */
extern Vaso*         filas[QUEUE_WAIT];

/*==========================================================================*/

void insert_list_event (event_t       event);
void remove_list_event (event_list_t* event);

#endif
