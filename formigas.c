#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TAM 50
#define RAIO 5

//fazer experimentos com raios 1, 5 e 10

//funcoes
int iniciaCenario();
void imprimeCenario();
void realizaMovimento();
void limpaTela();
int decideSePega();
int decideSeLarga();


//variaveis globais
char cenario[TAM][TAM]; // ' ' = nada, '.' = viva, '*' = morta
int vivas[TAM][TAM]; // 0 = nada, 1 = formiga vazia, 2 = formiga carregando
int mortas[TAM][TAM]; // 0 = nada, 1 = formiga morta
int espacoVazio[TAM][TAM]; // 0 = nada, 1 = espaco vazio


/*typedef struct formiga{
    int status; // 1 para carregando, 0 para vazia  
};*/


int main(){

    int i, j, formigasVivas;

    // inicia a matriz de formigas vivas = nada
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            vivas[i][j] = 0;
        }
    }    
    
    formigasVivas = iniciaCenario();

    for(i = 1; i < 500; i++){
        imprimeCenario();
        usleep(1000*80) // 80ms
        limpaTela();        
        realizaMovimento();
        printf("interacao n: %d\n", i);
    }


    return 0;
}


int iniciaCenario(){
    int i, j;
    int chance, qtdFormigasVivas = 0, qtdFormigasMortas = 0, qtdCelulasVazias = 0;

    srand((unsigned)time(NULL));

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){

            chance = rand() % 100;

            if(chance < 25){ //de 0 a 100, 25% de chance de ter morta
                cenario[i][j] = '*'; //se a funcao de aleatorio der < 25, inicia com uma formiga morta
                qtdFormigasMortas++;
                mortas[i][j] = 1;
            }            
            else if(chance > 70){
                cenario[i][j] = '.'; //se der maior que 70, inicia com uma formiga viva
                qtdFormigasVivas++;
                vivas[i][j] = 1;
            }else{
                cenario[i][j] = ' '; //senao inicia com a celula vazia
                qtdCelulasVazias++;
                espacoVazio[i][j] = 1;
            }
        }
    }
    //printf("qtdFormigas vivas = %d\nqtdFormigas mortas = %d\nqtd Vazio = %d\n", qtdFormigasVivas, qtdFormigasMortas, qtdCelulasVazias);
    return qtdFormigasVivas;
}


void realizaMovimento(){
    int i, j, movimento;

    srand((unsigned)time(NULL));
    
    for(i = 1; i < TAM-1; i++){
        for(j = 1; j < TAM-1; j++){
            if(vivas[i][j] == 1){ // se a formiga em i j está vazia
                /* anda para alguma casa aleatoria */
                movimento = rand() % 100;
                
                if(movimento < 25 && vivas[i-1][j] != 1 && vivas[i-1][j] != 2 && mortas[i-1][j] != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i-1][j] = '.';                
                }
                if(movimento > 25 && movimento < 50 && vivas[i+1][j] != 1 && vivas[i+1][j] != 2 && mortas[i+1][j] != 1){ // anda para o sul
                    vivas[i+1][j] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '.';
                }
                if(movimento > 50 && movimento < 75 && vivas[i][j+1] != 1 && vivas[i][j+1] != 2 && mortas[i][j+1] != 1){ // anda para leste
                    vivas[i][j+1] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '.';
                }
                if(movimento > 75 && vivas[i][j-1] != 1 && vivas[i][j-1] != 2 && mortas[i][j-1] != 1){ // anda para oeste
                    vivas[i][j-1] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '.';
                }

                    /* se encontrar uma morta dentro do raio, chama decideSePega(); */
                        /* se pegar, vivas[i][j] = 2 */
                        /* movimenta para outra casa aleatoria */
                    /* senao, anda para outra casa aleatoria */
            }
            if(vivas[i][j] == 2){ // se a formiga em i j estiver carregando
                /* anda para uma casa aleatoria */
                    /* se encontrar uma morta dentro do raio, chama a decideSeLarga(); */
                        /* se largar, vivas[i][j] = 1 */
                        /* movimenta para outra casa aleatoria */
                    /* senao, anda para outra casa aleatoria */
            }
        }
    }
}


/* Funcoes de decisao */

int decideSePega(){
    int decisao; // 1 = pega, 0 = nao pega

    
    

    return decisao;
}

int decideSeLarga(){
    int decisao; // 1 = pega, 0 = nao pega    

    return decisao;
}


/* Funcoes operacionais */

void limpaTela(){
    system("cls || clear");
}

void imprimeCenario(){
    int i, j;

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            printf("%c ", cenario[i][j]);
        }
        printf("\n");
    }
}





























