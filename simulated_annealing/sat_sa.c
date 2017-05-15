/*

    Nadyan Suriel Pscheidt
    Inteligência Artificial
    
    Utilizando Simulated Annealing para
    resolver o problema 3-SAT
*/

#include <stdio.h>

/* Defines */
#define T0          100     // Temperatura inicial
#define TN          0       // Temperatura final
#define N           500000  // Quantidade de iterações
#define QTDVARS     20      // Quantidade de variaveis
#define QTDCLAUS    91      // Quantidade de clausulas

/* Funcoes */
void monta_vetor();
int resfriamento(int i);
void gera_solucao_inicial();

/* Vars */
int tam = 3 * QTDCLAUS;
int vet[tam];
int solucao[tam];


int main(){
    monta_vetor();


    return 0;
}

void monta_vetor(){
    FILE *f = fopen("instancias/uf20_01.txt", "r");
    int tam = 3 * QTDCLAUS;    
    int i, qtdVars, qtdClaus;

    if(f == NULL){
        printf("ERRO AO LER ARQUIVO!\n");        
    }else{
        fscanf(f, "%d", &qtdVars);
        fscanf(f, "%d", &qtdClaus);    

        for(i = 0; i < tam; i++){
            fscanf(f, "%d", &vet[i]);
        }
    }
}

void gera_solucao_inicial(){
    int chance, i;

    srand((unsigned)time(NULL));
    
    for(i = 0; i < tam; i++){
        chance = rand() % 100; 
        
        if(chance < 50){
            solucao[i] = 1; // 50% de chance de ser 1    
        }else{
            solucao[i] = 0; // 50& de chance de ser 0
        }
    }
}
