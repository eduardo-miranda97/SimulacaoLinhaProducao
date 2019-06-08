#ifndef TRIANGULAR
#define  TRIANGULAR

#include <iostream>
#include <cstdlib>

/* codigo erro semantico em calculo matematico */
#define  ERRO_CALC          7

/* Amplitudes para envelopagem da função triangular (Accept-Reject Method)*/
#define  AMPLITUDE_MAX      1
#define  AMPLITUDE_MIN      0

/* Struct para armazenar os numeros da distribuição aleatoria triangular */
typedef struct{
    double  time_min;
    double  time_mode;
    double  time_max;
}times_triangular_t;

/* Função que gera numeros aleatorios com distribuição triangular */
double trand(times_triangular_t times);

/* Calcula parte crescente da triangular*/
double tcresc(double start, double end, double ponto);

/* Calcula parte decrescente da triangular*/
double tdecre(double start, double end, double ponto);

#endif //TRIANGULAR
