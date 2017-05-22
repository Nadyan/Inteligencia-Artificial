/*
    Nadyan Suriel Pscheidt
    Inteligência Artificial
    
    Utilizando Simulated Annealing para
    resolver o problema 3-SAT

    Na análise, verificar se o algoritmo convergiu,
    ou seja, o espaço de busca se afunila com o tempo,
    diferente da busca aleatoria

    gcc -o sat_sa sat_sa.c -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Defines */
#define T0                  4.0          // Temperatura inicial
#define TN                  0.0          // Temperatura final
#define N                   500000       // Quantidade de iterações
#define CHANCE_PERTURBACAO  3            // % de chance de bit flip da solucao

/* instancia 1 */
#define QTDVARS             20          // Quantidade de variaveis
#define QTDCLAUS            91          // Quantidade de clausulas

/* instancia 2 */
//#define QTDVARS           100   
//#define QTDCLAUS          430

/* instancia 3 */
//#define QTDVARS           250
//#define QTDCLAUS          1065

/* Funcoes */
void monta_vetores();
void gera_solucao_inicial();
float resfriamento(int i);                
int avalia_solucao();			// fitness
void perturba();				// crossover
void avalia_print();

/* Vars */
int tam = 3 * QTDCLAUS;
int vet[3 * QTDCLAUS];
int negs[3 * QTDCLAUS];
int solucao[QTDVARS];
int candidato_sol[QTDVARS];
int qtdAnterior = 0;
float delta;

int main(){
    int j, k, aux, oi = 0, oi2 = 0;
    float temperatura = T0;
    float chance, exp;

    srand((unsigned)time(NULL));
    
    monta_vetores();
    gera_solucao_inicial();

    for(j = 0; j < N; j++){
      
        aux = avalia_solucao();                 // avalia a solucao contida no candidato_sol[]
        //printf("%d\n", aux);

              
        if(aux == 0){                     // se a solucao canditada for pior
            
            chance = ((float)rand()/(float)(RAND_MAX)) * 100; // gera um float aleatorio
            //printf("%f\n", chance);

            exp = delta/temperatura;
            //printf("%f\n", exp);
            
            //printf("%f\n", (pow(2.7, exp)*100.0));

            if(chance < (pow(2.7, exp)*100.0)){
                for(k = 0; k < tam; k++)
                    solucao[k] = candidato_sol[k];

                oi++;
                //avalia_print();
            }
        }else if(aux == 1){                           // se a solucao candidata for melhor
            for(k = 0; k < tam; k++)
                solucao[k] = candidato_sol[k];  // atribui como solucao

            oi2++;
            //avalia_print();
        }

        perturba();
        temperatura = resfriamento(j);
    }

    printf("piorias: %d\nmelhorias: %d\n", oi, oi2);    

    return 0;
}

void monta_vetores(){
    
    int ignore;

    /* instancia 1 */
    FILE *f = fopen("instancias/uf20_01.txt", "r");
    /* instancia 2 */
    //FILE *f = fopen("instancias/uf100_01.txt", "r");
    /* instancia 3 */       
    //FILE *f = fopen("isntancias/uf250_01.txt", "r");
    
    int i, qtdVars, qtdClaus;

    if(f == NULL){
        printf("ERRO AO LER ARQUIVO!\n");
    }else{
        fscanf(f, "%d", &qtdVars);
        fscanf(f, "%d", &qtdClaus); 

        for(i = 0; i < QTDVARS; i++)        // inicia o vetor de negacao
            negs[i] = 0;   

        for(i = 0; i < tam; i++){
            fscanf(f, "%d", &vet[i]);       // le a var na posicao i

            if(vet[i] < 0){
                negs[i] = 1;                // se for negação        
            }

            vet[i] = abs(vet[i]);           // armazena apenas a variavel no vetor vet, e nao se está negada ou nao
                                            // a negacao fica armazenada no vetor negs

            ignore++;

            if(ignore == 3){
                fscanf(f, "%d", &ignore); 
                //printf("%d\n", ignore);
            }
        }
    }
}

void gera_solucao_inicial(){
    int chance, i;
    
    for(i = 0; i < tam; i++){
        chance = rand() % 100; 
        
        if(chance < 50){
            solucao[i] = 1; // 50% de chance de ser 1
            candidato_sol[i] = 1;
        }else{
            solucao[i] = 0; // 50& de chance de ser 0
            candidato_sol[i] = 0;
        }
    }
}

void perturba(){
    int chance, i;

    /* existe uma chance de x% de bit flip */
    /*for(i = 0; i < QTDVARS; i++){
        chance = rand() % 100;
        if(chance <= CHANCE_PERTURBACAO){
            if(candidato_sol[i] == 0)
                candidato_sol[i] = 1;
            else if(candidato_sol[i] == 1)
                candidato_sol[i] = 0;
        }
    }*/

    for(i = 0; i < QTDVARS; i++)
        candidato_sol[i] = solucao[i];	
    
    /* Muda um bit */
    chance = rand() % QTDVARS;
    if(candidato_sol[chance] == 0)
        candidato_sol[chance] = 1;
    else if(candidato_sol[chance] == 1)
        candidato_sol[chance] = 0;
    
}

int avalia_solucao(){
    int qtdSatisfeitas = 0, n, aux; // var contadora de clausulas satisfeitas
    int var1, var2, var3;
            
    // 3-CNF -> (A || B || C) && (D || E || F) && ...
    for(n = 0; n < tam; n += 3){
        var1 = candidato_sol[vet[n]];
        var2 = candidato_sol[vet[n+1]];
        var3 = candidato_sol[vet[n+2]];
        
        if(negs[n] == 1){
            if(var1 == 1)
                var1 = 0;
            else if(var1 == 0)
                var1 = 1;
        }
        if(negs[n+1] == 1){
            if(var2 == 1)
                var2 = 0;
            else if(var2 == 0)
                var2 = 1;
        }
        if(negs[n+2] == 1){
            if(var3 == 1)
                var3 = 0;
            else if(var3 == 0)
                var3 = 1;
        }
        
        if(var1 || var2 || var3)
            qtdSatisfeitas++;
    }
    
    //printf("%d\n", qtdSatisfeitas);

    delta = (float)(qtdSatisfeitas - qtdAnterior);

    aux = qtdAnterior;
    qtdAnterior = qtdSatisfeitas;
    
    if(qtdSatisfeitas >= aux){
        return 1;
    }else{
        return 0;
    }
}

void avalia_print(){
    /* Funcao utilziada para imprimir a quantidade
       de clausulas satisfeitas pela solucao escolhida */

    int var1, var2, var3, qtdSatisfeitas = 0, n;

    FILE *escreve = fopen("resultados.txt", "a+");
            
    // 3-CNF -> (A || B || C) && (D || E || F) && ...
    for(n = 0; n < tam; n += 3){
        var1 = solucao[vet[n]];
        var2 = solucao[vet[n+1]];
        var3 = solucao[vet[n+2]];
        
        if(negs[n] == 1){
            if(var1 == 1)
                var1 = 0;
            else if(var1 == 0)
                var1 = 1;
        }
        if(negs[n+1] == 1){
            if(var2 == 1)
                var2 = 0;
            else if(var2 == 0)
                var2 = 1;
        }
        if(negs[n+2] == 1){
            if(var3 == 1)
                var3 = 0;
            else if(var3 == 0)
                var3 = 1;
        }
        
        if(var1 || var2 || var3)
            qtdSatisfeitas++;
    }

    fprintf(escreve, "%d\n", qtdSatisfeitas);
    printf("%d\n", qtdSatisfeitas);
}

float resfriamento(int i){
    float novaTemp;
        
    /* Fórmula de resfriamento linear */
    novaTemp = T0 - (float)i * ((T0 - TN) / (float)N);

    /* Fórmula de resfriamento n5 */
    //novaTemp = ((T0 - TN)/(1.0 + pow(2.7, 3.0*((float)i - ((float)N)/2.0))));

    /* busca aleatoria */
    //novaTemp = 100.0;

    //printf("%f\n", novaTemp);
        
    return novaTemp;
}


