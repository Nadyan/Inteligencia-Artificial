#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM 51 //tam = n-1
#define RAIO 5

//fazer experimentos com raios 1, 5 e 10

int iniciaCenario();
void imprimeCenario();
void realizaMovimento();
void limpaTela();
int decideSePega();
int decideSeLarga();


char cenario[TAM][TAM]; // ' ' = nada, '.' = viva, '*' = morta
int vivas[TAM][TAM]; // 0 = nada, 1 = formiga vazia, 2 = formiga carregando
int mortas[TAM][TAM]; // 0 = nada, 1 = formiga morta
int espacoVazio[TAM][TAM]; // 0 = nada, 1 = espaco vazio
int raioVisao[RAIO][RAIO]; // matriz de visao da formiga. 0 = nada, 1 = formiga morta


int main(){
    int i, j, formigasVivas;

    // inicia a matriz de formigas vivas = nada
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            vivas[i][j] = 0;
        }
    }    
    
    formigasVivas = iniciaCenario();

    for(i = 1; i < 100; i++){
        imprimeCenario();
        usleep(1000*80); // 80ms
        limpaTela();        
        realizaMovimento();
        printf("interacao n: %d\n", i+1);
    }


    return 0;
}


int iniciaCenario(){
    int i, j;
    int chance, qtdFormigasVivas = 0, qtdFormigasMortas = 0, qtdCelulasVazias = 0;

    srand((unsigned)time(NULL));

    for(i = 1; i < TAM-1; i++){
        for(j = 1; j < TAM-1; j++){

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
    int decisao;

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

                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSePega()
                if(mortas[i-1][j] == 1){ // se esta no norte
                    decisao = decideSePega(i-1, j);
                    if(decisao == 1){
                        vivas[i][j] = 2; // formiga passou a estar carregando
                        mortas[i-1][j] = 0; // remove a morta                    
                    }                
                }
                else if(mortas[i+1][j] == 1){ // se esta no sul
                    decisao = decideSePega(i+1, j);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i+1][j] = 0;
                    }
                }
                else if(mortas[i][j+1] == 1){ // se esta no leste
                    decisao = decideSePega(i, j+1);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i][j+1] = 0;
                    }
                }
                else if(mortas[i][j-1] == 1){ // se esta no oeste
                    decisao = decideSePega(i, j-1);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i][j-1] = 0;
                    }
                }
            }
            if(vivas[i][j] == 2){ // se a formiga em i j estiver carregando
                /* anda para uma casa aleatoria */
                movimento = rand() % 100;

                if(movimento < 25 && vivas[i-1][j] != 1 && vivas[i-1][j] != 2 && mortas[i-1][j] != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i-1][j] = '.';        
                }
                if(movimento > 25 && movimento < 50 && vivas[i+1][j] != 1 && vivas[i+1][j] != 2 && mortas[i+1][j] != 1){ // anda para o sul
                    vivas[i+1][j] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '.';
                }
                if(movimento > 50 && movimento < 75 && vivas[i][j+1] != 1 && vivas[i][j+1] != 2 && mortas[i][j+1] != 1){ // anda para leste
                    vivas[i][j+1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '.';
                }
                if(movimento > 75 && vivas[i][j-1] != 1 && vivas[i][j-1] != 2 && mortas[i][j-1] != 1){ // anda para oeste
                    vivas[i][j-1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '.';
                }

                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSeLarga()
                if(mortas[i-1][j] == 1){ // se esta no norte
                    decisao = decideSeLarga(i-1, j);
                    if(decisao == 1){
                        vivas[i][j] = 1; // formiga passou a estar carregando
                        mortas[i][j] = 1; // larga a morta onde esta
                    }
                }
                else if(mortas[i+1][j] == 1){ // se esta no sul
                    decisao = decideSeLarga(i+1, j);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                    }
                }
                else if(mortas[i][j+1] == 1){ // se esta no leste
                    decisao = decideSeLarga(i, j+1);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                    }
                }
                else if(mortas[i][j-1] == 1){ // se esta no oeste
                    decisao = decideSeLarga(i, j-1);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                    }
                }
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





























