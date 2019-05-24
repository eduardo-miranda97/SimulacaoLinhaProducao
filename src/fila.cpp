#include "./include/fila.h"


void put_vaso_fila(Vaso* vaso, u_int8_t fila){
      if (!SM_queue_vasos[fila]){
           SM_length_queue_vasos[fila] = 1;
           SM_queue_vasos[fila] = (Vaso**) malloc(sizeof(Vaso*));
           if (!SM_queue_vasos[fila]){
                printf("\n================================================================================\n");
                printf("\n[ERRO] falha na alocação de memoria em put_vaso_fila()\n\n");
                printf("\n================================================================================\n");
                exit(ERRO_MEMORY_ACESS);
          }
          SM_queue_vasos[fila][0] = vaso;
          return;
      }
      SM_queue_vasos[fila] = (Vaso**) realloc(SM_queue_vasos[fila], sizeof(Vaso*)*(++(SM_length_queue_vasos[fila])));
      SM_queue_vasos[fila][SM_length_queue_vasos[fila]-1] = vaso;
}

Vaso* pop_vaso_fila(u_int8_t fila){
      if (!SM_queue_vasos[fila]){
          return NULL;
      }
      u_int16_t pos = 0;
      Vaso*     aux = SM_queue_vasos[fila][pos];
      for (u_int16_t i=1; i < SM_length_queue_vasos[fila]; i++)
          if (SM_queue_vasos[fila][i]->get_queue(fila) > aux->get_queue(fila)){
              aux = SM_queue_vasos[fila][i];
              pos = i;
          }
      if (pos != SM_length_queue_vasos[fila]-1){
          SM_queue_vasos[fila][pos] = SM_queue_vasos[fila][SM_length_queue_vasos[fila]-1];
      }
      SM_queue_vasos[fila] = (Vaso**) realloc(SM_queue_vasos[fila], sizeof(Vaso*)*(--(SM_length_queue_vasos[fila])));
      if ((!SM_queue_vasos[fila])&&(SM_length_queue_vasos[fila])){
           printf("\n================================================================================\n");
           printf("\n[ERRO] falha na alocação de memoria em pop_vaso_fila()\n\n");
           printf("\n================================================================================\n");
           exit(ERRO_MEMORY_ACESS);
     }
      return aux;
}
