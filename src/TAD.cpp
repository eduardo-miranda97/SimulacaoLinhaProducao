#include "./include/TAD.h"

event_list_t* SM_list_event_simulation = NULL;
Vaso*         filas[QUEUE_WAIT];

void insert_list_event(event_t event){
    if (!SM_list_event_simulation){
       /* LISTA VAZIA, PRIMEIRA ADD*/
       SM_list_event_simulation = new event_list_t();
       if (!SM_list_event_simulation){
           printf("\n================================================================================\n");
           printf("\n[ERRO] falha na alocação de memoria em insert_list_event()\n");
           printf("\n================================================================================\n");
           exit(ERRO_MEMORY_ALLOC);
       }
       SM_list_event_simulation->event      = event;
       SM_list_event_simulation->next_event = NULL;
       SM_list_event_simulation->prev_event = NULL;
       return;
    }
    event_list_t* new_event = new event_list_t ();

    if (!new_event){
        printf("\n================================================================================\n");
        printf("\n[ERRO] falha na alocação de memoria em insert_list_event()\n");
        printf("\n================================================================================\n");
        exit(ERRO_MEMORY_ALLOC);
    }

    new_event->event        = event;
    event_list_t* ptr_event = SM_list_event_simulation;

    do{
      if (event.time_event < ptr_event->event.time_event){
         new_event->next_event = ptr_event;
         new_event->prev_event = ptr_event->prev_event;
         /* TRATAMENTO ADD NA FIRST(CABEÇA)*/
         if (new_event->prev_event){
            new_event->prev_event->next_event = new_event;
         }else{
            SM_list_event_simulation = new_event;
         }
         ptr_event->prev_event = new_event;
         return;
      }
      /* CASO ONDE ELE É COLOCADO NO FIM DA LISTA*/
      if (!ptr_event->next_event){
         new_event->next_event = ptr_event->next_event;
         new_event->prev_event = ptr_event;
         ptr_event->next_event = new_event;
         return;
      }
      ptr_event = ptr_event->next_event;
    }while(1);
}

void remove_list_event(event_list_t* event){
    /* CASO ONDE ESTA REMOVENDO A FIRST (CABEÇA)*/
    if (!event->prev_event){
        SM_list_event_simulation      = event->next_event;
    }else{
        event->prev_event->next_event = event->next_event;
    }
    /* CASO ONDE NÃO É O ULTIMO A SER REMOVIDO*/
    if (event->next_event){
        event->next_event->prev_event = event->prev_event;
    }
    delete event;
}
