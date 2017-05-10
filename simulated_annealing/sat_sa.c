/*

    Nadyan Suriel Pscheidt
    Inteligência Artificial
    
    Utilizando Simulated Annealing para
    resolver o problema 3-SAT
*/

#include <stdio.h>

/* Defines */
#define T0  100     // Temperatura inicial
#define TN  0       // Temperatura final
#define N   500000  // Quantidade de iterações

/* Funcoes */
int resfriamento(int i);
int gera_solucao_inicial();
