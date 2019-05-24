#include "./include/triangular.h"

float trand(times_triangular_t times){
    float y, range, prob;
    do{
        range = ((rand()%((int)(times.time_max-times.time_min+1)))+times.time_min);
        prob  = ((float) rand()/RAND_MAX);
        if (range <= times.time_mode){
            y = tcresc(times.time_min, times.time_mode, range);
        }else{
            y = tdecre(times.time_mode, times.time_max, range);
        }
    }while(prob > y);
    return range;
}

float tcresc(float start, float end, float ponto){
    if (start == end){
        printf("\n\n[ERROR] Entrada para tcresc(), devem ser diferentes !\n\n");
        exit(ERRO_CALC);
    }
    float a = ((AMPLITUDE_MAX-AMPLITUDE_MIN)/(end-start));
    float b = (((AMPLITUDE_MIN*end)-(AMPLITUDE_MAX*start))/(end-start));
    return (a*ponto+b);
}

float tdecre(float start, float end, float ponto){
    if (start == end){
        printf("\n\n[ERROR] Entrada para tdecre(), devem ser diferentes !\n\n");
        exit(ERRO_CALC);
    }
    float a = ((AMPLITUDE_MIN-AMPLITUDE_MAX)/(end-start));
    float b = (((AMPLITUDE_MAX*end)-(AMPLITUDE_MIN*start))/(end-start));
    return (a*ponto+b);
}
