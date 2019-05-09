#include "./include/TAD.h"

event_list_t* queue_event_list = NULL;
Vaso* filas[QUEUE_WAIT];

void insert_list_event(event_t event){
    if (!queue_event_list){
       /* (Julio) LISTA VAZIA, PRIMEIRA ADD*/
       queue_event_list = (event_list_t*) malloc(sizeof(event_list_t));
       if (!queue_event_list){
          std::cerr << "\nERROR: Error Alloc Memory for \"queue_event_list\"" << '\n';
          exit(ERROR_MEMORY_ALLOC);
       }
       queue_event_list->event      = event;
       queue_event_list->next_event = NULL;
       queue_event_list->prev_event = NULL;
       return;
    }
    event_list_t* new_event = (event_list_t*) malloc(sizeof(event_list_t));

    if (!new_event){
       std::cerr << "\nERROR: Error Alloc Memory for \"new_event\"" << '\n';
       exit(ERROR_MEMORY_ALLOC);
    }

    new_event->event = event;
    event_list_t* ptr_event = queue_event_list;

    do{
      if (event.time_event < ptr_event->event.time_event){
         new_event->next_event = ptr_event;
         new_event->prev_event = ptr_event->prev_event;
         /*(Julio) TRATAMENTO ADD NA FIRST(CABEÇA)*/
         if (new_event->prev_event){
            new_event->prev_event->next_event = new_event;
         }else{
            queue_event_list = new_event;
         }
         ptr_event->prev_event = new_event;
         return;
      }
      /*(Julio) CASO ONDE ELE É COLOCADO NO FIM DA LISTA*/
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
    /*(Julio) CASO ONDE ESTA REMOVENDO A FIRST (CABEÇA)*/
    if (!event->prev_event){
        queue_event_list = event->next_event;
    }else{
        event->prev_event->next_event = event->next_event;
    }
    /*(Julio) CASO ONDE NÃO É O ULTIMO A SER REMOVIDO*/
    if (event->next_event){
        event->next_event->prev_event = event->prev_event;
    }
    free(event);
}
