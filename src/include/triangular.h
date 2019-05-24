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
    float  time_min;
    float  time_mode;
    float  time_max;
}times_triangular_t;

/* Função que gera numeros aleatorios com distribuição triangular */
float trand(times_triangular_t times);

/* Calcula parte crescente da triangular*/
float tcresc(float start, float end, float ponto);

/* Calcula parte decrescente da triangular*/
float tdecre(float start, float end, float ponto);

#endif //TRIANGULAR
