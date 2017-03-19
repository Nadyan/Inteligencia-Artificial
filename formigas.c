#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TAM 50

//funcoes
void iniciaCenario();
void imprimeCenario();
//void realizaMovimento();
void limpaTela();


//variaveis globais
char cenario[TAM][TAM];
int qtdFormigas = 0;


int main(){

    int i, j;
    
    iniciaCenario();
    imprimeCenario();

    /*    
    while(1){
        imprimeCenario();
        realizaMovmento();
        limpaTela();
    }*/

    return 0;
}

//iniciar as formigas
void iniciaCenario(){
    int i, j;
    int chance;

    srand((unsigned)time(NULL));

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){

            //srand((unsigned)time(NULL));
            chance = rand() % 100;
            //printf("%d\n", chance);

            if(chance < 30) //de 0 a 100, 30% de chance de ter morta
                cenario[i][j] = '*'; //se a funcao de aleatorio der < 40, inicia com uma formiga morta 
            else if(chance > 80){
                cenario[i][j] = '.'; //se der maior que 80, inicia com uma formiga viva
                qtdFormigas++;
            }
            else 
               cenario[i][j] = ' '; //senao inicia com a celula vazia
        }        
    }
}

//imprimir cenario
void imprimeCenario(){
    int i, j;  
  
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            printf("%c ", cenario[i][j]);
        }
        printf("\n");
    }
}
/*
void realizaMovimento(){

}*/

void limpaTela(){
    system("cls || clear");
}






























