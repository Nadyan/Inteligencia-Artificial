/*
    Nadyan S. Pscheidt

    Inteligência Artificial
    Robo de manutenção
*/

#include <stdio.h>
#include <time.h>

/* Defines */
#define TAM 42              // tamanho do terreno TAMxTAM
#define INFINITO 99999
#define DELAYA 20           // delay do algortimo a* (nao usar)
#define RAIO 4              // raio de visao do robo = 4, totalizando 80 regioes (9x9-1)

/* Funções */
void aStar(int i, int j);
Ponto buscaMenor_aStar();
int manhattan(int i, int j);
void montaCenario();
void imprimeCenario();
void limpaTela();
void initVisitados();

/* Structs */
typedef struct{
    int x;
    int y;
    int paix, paiy;
}Ponto;

/* Vars */
int ferramentas[5];                 // cada posicao é uma ferramenta, pode ter mais de 1 por posicao
int fabricas[];                     // vetor de fabricas consertadas (1), e nao consertadas (0)
int distancia_estimada[TAM][TAM];
int distancia[TAM][TAM];
int visitados[TAM][TAM];
char terrenoChar[TAM][TAM][2];
int terreno[TAM][TAM];
int terrenoCusto[TAM][TAM];
char terrenoAstar[TAM][TAM][2];


int main(){

    return 0;
}


void montaCenario(){
    FILE *f = fopen("terreno.txt", "r");
    int i, j;

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            fscanf(f, "%d", &terreno[i][j]);

            switch(terreno[i][j]){
                case 0:
                    terrenoChar[i][j][0] = '0'; terrenoChar[i][j][1] = ' ';
                    terrenoCusto[i][j] = 1;
                    break;
                case 1:
                    terrenoChar[i][j][0] = '1'; terrenoChar[i][j][1] = ' ';
                    terrenoCusto[i][j] = 5;
                    break;
                case 2:
                    terrenoChar[i][j][0] = '2'; terrenoChar[i][j][1] = ' ';
                    terrenoCusto[i][j] = 10;
                    break;
                case 3:
                    terrenoChar[i][j][0] = '3'; terrenoChar[i][j][1] = ' ';
                    terrenoCusto[i][j] = 15;
                    break;
            }
            terrenoAstar[i][j][0] = terrenoChar[i][j][0];
            terrenoAstar[i][j][1] = terrenoChar[i][j][1];
        }
    }
}


Ponto buscaMenor_aStar(){
        int i, j, min = INFINITO;
        Ponto menor;

        for(i = 0; i < TAM; i++){
            for(j = 0; j < TAM; j++){
                if(distancia_estimada[i][j] != -1 && distancia_estimada[i][j] < min && !visitados[i][j]){
                    min = distancia_estimada[i][j];
                    menor.x = i;
                    menor.y = j;
                }
            }
        }
        return menor;
}


void aStar(int i, int j){
    Ponto paternidade[TAM][TAM], inicio;
    int qtd = 0, custoAstar = 0;

    for(int k = 0; k < TAM; k++){
        for(int l = 0; l < TAM; l++){
            if(k == i && l == j){
                distancia_estimada[k][l] = terrenoCusto[k][l] + manhattan(k, l);
                distancia[k][l] = terrenoCusto[k][l];
            }else{
                distancia_estimada[k][l] = INFINITO;
                distancia[k][l] = -1;
            }
        }
    }

    inicio.x = i; inicio.y = j;
    paternidade[inicio.x][inicio.y].paix = -1;
    paternidade[inicio.x][inicio.y].paiy = -1;

    //limpaTela();
    //printf("\nExecutando Busca A*:\n\n");

    while(1){
        Ponto p = buscaMenor_aStar();

        visitados[p.x][p.y] = 1; // marca p como visitado

        if(p.x == destino.x && p.y == destino.y)// verificar se p = fim, se sim break
            break;

        /* Calcula as distancias dos vizinhos */
        /* Cima */
        if(p.x-1 >= 0 && (distancia_estimada[p.x-1][p.y] > (distancia[p.x][p.y] + terrenoCusto[p.x-1][p.y] + manhattan(p.x-1, p.y))) && !visitados[p.x-1][p.y]){
            distancia_estimada[p.x-1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x-1][p.y] + manhattan(p.x-1, p.y);
            distancia[p.x-1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x-1][p.y];
            terrenoChar[p.x-1][p.y][1] = '.';
            //limpaTela();
            //printf("\nExecutando Busca A*:\n\n");
            //imprimeCenario();
            //usleep(1000*DELAYA);
            paternidade[p.x-1][p.y].paix = p.x; paternidade[p.x-1][p.y].paiy = p.y;
            qtd++;
        }
        /* Direita */
        if(p.y+1 < TAM && (distancia_estimada[p.x][p.y+1] > (distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1] + manhattan(p.x, p.y+1))) && !visitados[p.x][p.y+1]){
            distancia_estimada[p.x][p.y+1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1] + manhattan(p.x, p.y+1);
            distancia[p.x][p.y+1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1];
            //terrenoChar[p.x][p.y+1][1] = '.';
            //limpaTela();
            //printf("\nExecutando Busca A*:\n\n");
            //imprimeCenario();
            //usleep(1000*DELAYA);
            paternidade[p.x][p.y+1].paix = p.x; paternidade[p.x][p.y+1].paiy = p.y;
            qtd++;
        }
        /* Baixo */
        if(p.x+1 < TAM && (distancia_estimada[p.x+1][p.y] > (distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y] + manhattan(p.x+1, p.y))) && !visitados[p.x+1][p.y]){
            distancia_estimada[p.x+1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y] + manhattan(p.x+1, p.y);
            distancia[p.x+1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y];
            //terrenoChar[p.x+1][p.y][1] = '.';
            //limpaTela();
            //printf("\nExecutando Busca A*:\n\n");
            //imprimeCenario();
            //usleep(1000*DELAYA);
            paternidade[p.x+1][p.y].paix = p.x; paternidade[p.x+1][p.y].paiy = p.y;
            qtd++;
        }
        /* Esquerda */
        if(p.y-1 >= 0 && (distancia_estimada[p.x][p.y-1] > (distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1] + manhattan(p.x, p.y-1))) && !visitados[p.x][p.y-1]){
            distancia_estimada[p.x][p.y-1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1] + manhattan(p.x, p.y-1);
            distancia[p.x][p.y-1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1];
            //terrenoChar[p.x][p.y-1][1] = '.';
            //limpaTela();
            //printf("\nExecutando Busca A*:\n\n");
            //imprimeCenario();
            //usleep(1000*DELAYA);
            paternidade[p.x][p.y-1].paix = p.x; paternidade[p.x][p.y-1].paiy = p.y;
            qtd++;
        }
    }

    // caminho
    Ponto cam = destino;
    Ponto aux;
    while(paternidade[cam.x][cam.y].paix != -1 && paternidade[cam.x][cam.y].paiy != -1){
        terrenoAstar[cam.x][cam.y][1] = '.';
        custoAstar += terrenoCusto[cam.x][cam.y];

        aux.x = cam.x;
        aux.y = cam.y;
        cam.x = paternidade[aux.x][aux.y].paix;
        cam.y = paternidade[aux.x][aux.y].paiy;
    }

    /*
    limpaTela();
    printf("\n\n Caminho encontrado: \n\n");

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(i == inicio.x && j == inicio.y)
                printf("In ");
            else if(i == destino.x && j == destino.y)
                printf("Fi ");
            else
                printf("%c%c ", terrenoAstar[i][j][0], terrenoAstar[i][j][1]);
        }
        printf("\n");
    }

    printf("\nA*:\n  - Custo: %d\n  - Nos expandidos: %d\n\n", custoAstar, qtd);
    */
}


int manhattan(int i, int j){
    /* a distancia de manhattan entre o
       ponto P1 com coordenadas (x1, y1)
       e o ponto P2 em (x2, y2)
       é |x1 - x2| + |y1 - y2|
       ou abs(x1 - x2) + abs(y1 - y2)   */

    /* A avaliacao heuristica ira retornar h(n),
       para calcular f(n) = g(n) + h(n),
       sendo g(n) a distância de n ao nó inicial (custo uniforme)
       e h(n) a distância estimada de n ao nó final (manhattan)    */

    Ponto p;
    p.x = i; p.y = j;

    return (abs(p.x - destino.x) + abs(p.y - destino.y)) * SUPERESTIMATIVA;
}


void imprimeCenario(){
    int i, j;

    printf("\n");

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(i == origem.x && j == origem.y)
                printf("   ");
            else if(i == destino.x && j == destino.y)
                printf("   ");
            else
                printf("%c%c ", terrenoChar[i][j][0], terrenoChar[i][j][1]);
        }
        printf("\n");
    }
}


void initVisitados(){
    int i, j;

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            visitados[i][j] = 0;
        }
    }
}


void limpaTela(){
    system("clear");
}



