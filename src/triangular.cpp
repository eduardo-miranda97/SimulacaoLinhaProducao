#include "./include/triangular.h"

double trand(times_triangular_t times){
    if ((times.time_min>times.time_mode)||(times.time_max<times.time_mode)){
        std::cout << '\n' << "[ERRO] Entrada para trand() com valores invalidos" << '\n';
        exit(ERRO_CALC);
    }
    if (times.time_min==times.time_max){
        return times.time_min;
    }
    double y, range, prob;
    do{
        range = ((rand()%((int)(times.time_max-times.time_min+1)))+times.time_min);
        prob  = ((double) rand()/RAND_MAX);
        if (range <= times.time_mode){
            y = tcresc(times.time_min, times.time_mode, range);
        }else{
            y = tdecre(times.time_mode, times.time_max, range);
        }
    }while(prob > y);
    return range;
}

double tcresc(double start, double end, double ponto){
    if (start == end){
        return start;
    }
    double a = ((AMPLITUDE_MAX-AMPLITUDE_MIN)/(end-start));
    double b = (((AMPLITUDE_MIN*end)-(AMPLITUDE_MAX*start))/(end-start));
    return (a*ponto+b);
}

double tdecre(double start, double end, double ponto){
    if (start == end){
        return start;
    }
    double a = ((AMPLITUDE_MIN-AMPLITUDE_MAX)/(end-start));
    double b = (((AMPLITUDE_MAX*end)-(AMPLITUDE_MIN*start))/(end-start));
    return (a*ponto+b);
}
