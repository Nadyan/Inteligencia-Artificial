/*
    Nadyan Suriel Pscheidt
    Inteligência Artificial
    
    Utilizando Simulated Annealing para
    resolver o problema 3-SAT

    É algoritmo de melhoria, diferente do de construção

    -> Na análise, verificar se o algoritmo convergiu,
    ou seja, o espaço de busca se afunila com o tempo,
    diferente da busca aleatoria

    -> Executar 10 vezes para cada instancia, tirar um grafico de media
    para cada, resultando em um grafico de convergencia para 
    cada instancia

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
//#define QTDVARS             20          // Quantidade de variaveis
//#define QTDCLAUS            91          // Quantidade de clausulas

/* instancia 2 */
//#define QTDVARS           100   
//#define QTDCLAUS          430

/* instancia 3 */
#define QTDVARS           250
#define QTDCLAUS          1065


/* Funcoes */
void monta_vetores();
void gera_solucao_inicial();
float resfriamento(int i);                
int avalia_solucao();			// fitness
void perturba();				// crossover
void avalia_print();            // montar vetor final[]
void salva_final();             // salvar no txt


/* Vars */
int tam = 3 * QTDCLAUS;
int vet[3 * QTDCLAUS];
int negs[3 * QTDCLAUS];
int solucao[QTDVARS];
int candidato_sol[QTDVARS];
int qtdAnterior = 0;
int final[N], qtdFinal = 0;     // vetor de qtdSatisfeitas a cada iteração
float delta;


int main(){
    int j, k, aux, pioras = 0, melhoras = 0;
    float temperatura = T0;
    float chance, exp;
    clock_t t;

    for(j = 0; j < N; j++)
        final[j] = 0;

    srand((unsigned)time(NULL));
    
    monta_vetores();
    gera_solucao_inicial();

    t = clock();
    for(j = 0; j < N; j++){
      
        aux = avalia_solucao();                 // avalia a solucao contida no candidato_sol[]
        //printf("%d\n", aux);

        if(aux == 0){                     // se a solucao canditada for pior
            
            chance = ((float)rand()/(float)(RAND_MAX)) * 1.0; // gera um float aleatorio
            //printf("%f\n", chance);

            exp = delta/temperatura;
            //printf("%f\n", exp);
            
            //printf("%f\n", (pow(2.7, exp))*1.0);

            if(chance < (pow(2.718281, exp)*1.0)){
                for(k = 0; k < QTDVARS; k++)
                    solucao[k] = candidato_sol[k];

                pioras++;
                avalia_print();
            }
        }else if(aux == 1){                           // se a solucao candidata for melhor
            for(k = 0; k < QTDVARS; k++)
                solucao[k] = candidato_sol[k];  // atribui como solucao

            melhoras++;
            avalia_print();
        }
        
        perturba();
        temperatura = resfriamento(j);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;

    printf("Tempo de execução: %lf\n", time_taken);

    printf("pioras: %d\nmelhoras: %d\n", pioras, melhoras);   

    salva_final(); 

    return 0;
}


void monta_vetores(){
    int ignore = 0;

    /* instancia 1 */
    //FILE *f = fopen("instancias/uf20_01.txt", "r");
    /* instancia 2 */
    //FILE *f = fopen("instancias/uf100_01.txt", "r");
    /* instancia 3 */       
    FILE *f = fopen("instancias/uf250_01.txt", "r");
    
    int i, qtdVars, qtdClaus;

    if(f == NULL){
        printf("ERRO AO LER ARQUIVO!\n");
    }else{
        fscanf(f, "%d", &qtdVars);
        fscanf(f, "%d", &qtdClaus);  

        for(i = 0; i < tam; i++){

            if(ignore == 3){
                fscanf(f, "%d", &ignore);   // ignorar o 0 no final de cada clausula
            }

            fscanf(f, "%d", &vet[i]);       // le a var na posicao i

            if(vet[i] < 0){
                negs[i] = 1;                // se for negação        
            }else{
                negs[i] = 0;
            }

            vet[i] = abs(vet[i]);           // armazena apenas a variavel no vetor vet, e nao se está negada ou nao
                                            // a negacao fica armazenada no vetor negs

            ignore++;

            //printf("%d\n", negs[i]);
        }
    }
}


void gera_solucao_inicial(){
    int chance, i;
    
    for(i = 0; i < QTDVARS; i++){
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
        var1 = candidato_sol[vet[n]-1];
        var2 = candidato_sol[vet[n+1]-1];
        var3 = candidato_sol[vet[n+2]-1];
        
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
        
        if(var1 || var2 || var3){
            qtdSatisfeitas++;
        }    
    }

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

    int var1, var2, var3;
    int qtdSatisfeitas = 0;
    int n;
            
    // 3-CNF -> (A || B || C) && (D || E || F) && ...
    for(n = 0; n < tam; n += 3){
        var1 = solucao[vet[n]-1];
        var2 = solucao[vet[n+1]-1];
        var3 = solucao[vet[n+2]-1];
        
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

    final[qtdFinal] = qtdSatisfeitas;
    //printf("%d %d %d\n", qtdFinal, qtdSatisfeitas, final[qtdFinal]);
    qtdFinal++;
}


float resfriamento(int i){
    float novaTemp;
        
    /* Fórmula de resfriamento n1 */
    //novaTemp = T0 - (float)i * ((T0 - TN) / (float)N);

    /* Fórmula de resfriamento n4 */
    //novaTemp = ((T0 - TN)/(1.0 + pow(2.7, 3*((float)i - ((float)N)/2.0))));

    /* Fórmula de resfriamento n5 */
    novaTemp = (0.5 * (T0 - TN)) * (1.0 + cos(((float)i * 3.141592) / (float)N)) + TN;

    printf("%f\n", novaTemp);
        
    return novaTemp;
}


void salva_final(){
    FILE *escreve = fopen("resultados.txt", "w+");
    int j;

    for(j = 0; j < qtdFinal; j++){
        //printf("%d\n", final[j]);
        fprintf(escreve, "%d\n", final[j]);
    }
}



