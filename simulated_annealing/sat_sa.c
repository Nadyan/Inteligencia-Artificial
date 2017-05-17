/*

    Nadyan Suriel Pscheidt
    Inteligência Artificial
    
    Utilizando Simulated Annealing para
    resolver o problema 3-SAT

    Na análise, verificar se o algoritmo convergiu,
    ou seja, o espaço de busca se afunila com o tempo,
    diferente da busca aleatoria
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Defines */
#define T0                  100         // Temperatura inicial
#define TN                  0           // Temperatura final
#define N                   500000      // Quantidade de iterações
#define CHANCE_PERTURBACAO  20          // 20% de chance de bit flip da solucao
#define INSTANCIA           1

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
int resfriamento(int i, int temp);                // i representa a temperatura atual
int avalia_solucao();                   // retorna 1 caso seja melhor, e 0 caso seja pior
void perturba();

/* Vars */
int tam = 3 * QTDCLAUS;
int vet[QTDCLAUS];
int negs[QTDVARS];
int solucao[3 * QTDCLAUS];
int candidato_sol[3 * QTDCLAUS];


int main(){
    int j, k, temperatura = T0, chance;

    srand((unsigned)time(NULL));
    
    monta_vetores();
    gera_solucao_inicial();

    for(j = 0; j < N; j++){
        int aux;
        
        aux = avalia_solucao();                 // avalia a solucao contida no candidato_sol[]
        
        if(aux == 1){                           // se a solucao candidata for melhor
            
            for(k = 0; k < tam; k++)
                solucao[k] = candidato_sol[k];  // atribui como solucao
            
            perturba();
            temperatura = resfriamento(j, temperatura);
        }else{
            temperatura = resfriamento(j, temperatura);
            
            chance = rand() % 100;              // calcula a chance de atribuir a solucao pior de acordo com a temperatura
            
            if(chance < temperatura){           // se der, atribui a solucao pior
                for(k = 0; k < tam; k++)
                    solucao[k] = candidato_sol[k];
            }
            
            perturba();                         // se nao for melhor, apenas gera nova
        }
    }

    return 0;
}

void monta_vetores(){
    
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
    for(i = 0; i < tam; i++){
        chance = rand() % 100;
        if(chance <= CHANCE_PERTURBACAO){
            if(candidato_sol[i] == 0)
                candidato_sol[i] = 1;
            else if(candidato_sol[i] == 1)
                candidato_sol[i] = 0;
        }
    }
}

int avalia_solucao(){
    int qtdSatisfeitas = 0; // var contadora de clausulas satisfeitas
    int var1, var2, var3;
    int var1neg = 0; var2neg = 0; var3neg = 0;

    // pega candidato_sol e ve quantas clausulas ele satisfaz
        // se for melhor do que qtdAnteriorSat
            // qtdAnteriorSat = qtdSatisfeitas;
            // return 1;
        //else
            // return 0;
            
    // 3-CNF -> (A && B && C) || (D && E && F)
    for(n = 0; n < tam; n += 3){
        var1 = vet[n];
        var2 = vet[n+1];
        var3 = vet[n+2];
        
        if(negs[n] == 1)
            var1neg = 1;
        if(negs[n+1] == 1)
            var2neg = 1;
        if(negs[n+2] == 1)
            var3neg = 1;
    } 

}

int resfriamento(int i, int temp){
    int novaTemp;           // nova temperatura que o sistema adquire apos interação

    if(temp > TN){             // se ainda nao alcancou a temperatura final TN
        
        /* Fórmula de resfriamento linear */
        novaTemp = T0 - i * ((T0 - TN) / N);

        return novaTemp;
    }else{
        return -1;           // temperatura final atingida
    }
}











