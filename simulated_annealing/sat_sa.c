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
int resfriamento(int i);                // i representa a temperatura atual
int avalia_solucao();                   // retorna 1 caso seja melhor, e 0 caso seja pior
void perturba();

/* Vars */
int tam = 3 * QTDCLAUS;
int vet[QTDCLAUS];
int negs[QTDVARS];
int solucao[3 * QTDCLAUS];


int main(){
    srand((unsigned)time(NULL));
    
    monta_vetores();
    gera_solucao_inicial();
    printf("\n\n");
    perturba();
    printf("\n\n");
    perturba();
    printf("\n\n");
    perturba();


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
        }else{
            solucao[i] = 0; // 50& de chance de ser 0
        }
    }
}

void perturba(){
    int chance, i;

    /* existe uma chance de x% de bit flip */
    for(i = 0; i < tam; i++){
        chance = rand() % 100;
        if(chance <= CHANCE_PERTURBACAO){
            if(solucao[i] == 0)
                solucao[i] = 1;
            else if(solucao[i] == 1)
                solucao[i] = 0;
        }
    }
}

int avalia_solucao(){
    int melhor;             // Flag sinalizando a melhora ou piora da solucao

    /* ... */

    if(melhor)
        return 1;
    else
        return 0;
}

int resfriamento(int i){
    int novaTemp;           // nova temperatura que o sistema adquire apos interação

    if(i > TN){             // se ainda nao alcancou a temperatura final TN
        /* ... */

        return novaTemp;
    }else{
        return 0;           // temperatura final atingida
    }
}











