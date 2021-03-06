/*
  Nadyan Suriel Pscheidt
  Inteligencia artificial
  - Busca BFS
  - Busca Uniforme
  - Busca A*

  Compilar com Interface grafica:
  sudo apt-get install libsfml-dev
  g++ -c buscaCega.cpp
  g++ -o buscaCega buscaCega.o -lsfml-graphics -lsfml-window -lsfml-system
  ./buscaCega

  Compilar humildemente pra rodar no terminal:
  g++ -o buscaCega buscaCega.cpp
  ./buscaCega
*/


/* Bibs */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <list>
#include <cmath>
//#include <SFML/Graphics.hpp>


/* Defs */
#define TAM 42  //terreno TAMxTAM
#define INFINITO 99999
#define DELAYBFS 20
#define DELAYUNI 20
#define DELAYA 20
#define SUPERESTIMATIVA 1 // superestimando o valor da distancia (1 = nada)


/* struct para os pontos de origem e destino */
typedef struct{
    int x;
    int y;
    int paix, paiy;
}Ponto;


/* Vars */
std::list<Ponto> lista;
std::list<Ponto> listaUniforme;
Ponto origem, destino;
int terreno[TAM][TAM];
int terrenoCusto[TAM][TAM];
int visitados[TAM][TAM];
int distancia[TAM][TAM];
int distancia_estimada[TAM][TAM];
char terrenoChar[TAM][TAM][2];
char terrenoLarg[TAM][TAM][2];
char terrenoUni[TAM][TAM][2];
char terrenoAstar[TAM][TAM][2];


/* Funcs */
void montaCenario();
void imprimeCenario();
void limpaTela();
void initVisitados();
void bfs(int i, int j);
void uniforme(int i, int j);
void aStar(int i, int j);
int manhattan(int i, int j);
int euclidiana(int i, int j);
Ponto buscaMenor_uniforme();
Ponto buscaMenor_aStar();


int main(){
    int i, j;

    printf("Informe o ponto de inicio: ");
    scanf("%d %d", &origem.x, &origem.y);
    printf("Informe o ponto de destino: ");
    scanf("%d %d", &destino.x, &destino.y);

    initVisitados();
    montaCenario();
    limpaTela();

    printf("\nExecutando Busca em Largura:\n\n");
    bfs(origem.x, origem.y);

    getchar(); getchar();

    initVisitados();
    montaCenario();
    limpaTela();

    printf("\nExecutando Busca Uniforme:\n\n");
    uniforme(origem.x, origem.y);

    getchar();

    initVisitados();
    montaCenario();
    limpaTela();

    printf("\nExecutando Busca A*\n\n");
    aStar(origem.x, origem.y);

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
            terrenoLarg[i][j][0] = terrenoChar[i][j][0];
            terrenoLarg[i][j][1] = terrenoChar[i][j][1];
            terrenoUni[i][j][0] = terrenoChar[i][j][0];
            terrenoUni[i][j][1] = terrenoChar[i][j][1];
            terrenoAstar[i][j][0] = terrenoChar[i][j][0];
            terrenoAstar[i][j][1] = terrenoChar[i][j][1];
        }
    }
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


void bfs(int i, int j){
    Ponto paternidade[TAM][TAM];
    int qtd = 0, custoLarg = 0;

    limpaTela();
    printf("\nExecutando Busca em Largura:\n\n");

    visitados[i][j] = 1;

    Ponto inicio;
    inicio.x = i; inicio.y = j;
    paternidade[inicio.x][inicio.y].paix = -1;  // no raiz
    paternidade[inicio.x][inicio.y].paiy = -1;  // no raiz

    lista.push_front(inicio);

    while(!lista.empty()){
        Ponto p = lista.front();
        lista.pop_front();

        /* cima */
        if(p.x - 1 >= 0 && !visitados[p.x-1][p.y]){
            Ponto pnovo;
            pnovo.x = p.x - 1;
            pnovo.y = p.y;
            pnovo.paix = p.x; pnovo.paiy = p.y;

            paternidade[pnovo.x][pnovo.y].paix = p.x;
            paternidade[pnovo.x][pnovo.y].paiy = p.y;

            if(pnovo.x == destino.x && pnovo.y == destino.y)
                break;
            else{
                qtd++;
                lista.push_back(pnovo);
                terrenoChar[pnovo.x][pnovo.y][1] = '.';
                visitados[pnovo.x][pnovo.y] = 1;
                limpaTela(); printf("\nExecutando Busca em Largura:\n\n");
                imprimeCenario();
                usleep(1000*DELAYBFS);
            }
        }
        /* direita */
        if(p.y < TAM && !visitados[p.x][p.y+1]){
            Ponto pnovo;
            pnovo.x = p.x;
            pnovo.y = p.y + 1;
            pnovo.paix = p.x; pnovo.paiy = p.y;

            paternidade[pnovo.x][pnovo.y].paix = p.x;
            paternidade[pnovo.x][pnovo.y].paiy = p.y;

            if(pnovo.x == destino.x && pnovo.y == destino.y)
                break;
            else{
                qtd++;
                lista.push_back(pnovo);
                terrenoChar[pnovo.x][pnovo.y][1] = '.';
                visitados[pnovo.x][pnovo.y] = 1;
                limpaTela(); printf("\nExecutando Busca em Largura:\n\n");
                imprimeCenario();
                usleep(1000*DELAYBFS);
            }
        }
        /* baixo */
        if(p.x + 1 < TAM && !visitados[p.x+1][p.y]){
            Ponto pnovo;
            pnovo.x = p.x + 1;
            pnovo.y = p.y;
            pnovo.paix = p.x; pnovo.paiy = p.y;

            paternidade[pnovo.x][pnovo.y].paix = p.x;
            paternidade[pnovo.x][pnovo.y].paiy = p.y;

            if(pnovo.x == destino.x && pnovo.y == destino.y)
                break;
            else{
                qtd++;
                lista.push_back(pnovo);
                terrenoChar[pnovo.x][pnovo.y][1] = '.';
                visitados[pnovo.x][pnovo.y] = 1;
                limpaTela(); printf("\nExecutando Busca em Largura:\n\n");
                imprimeCenario();
                usleep(1000*DELAYBFS);
            }
        }
        /* esquerda */
        if(p.y - 1 >= 0 && !visitados[p.x][p.y-1]){
            Ponto pnovo;
            pnovo.x = p.x;
            pnovo.y = p.y - 1;
            pnovo.paix = p.x; pnovo.paiy = p.y;

            paternidade[pnovo.x][pnovo.y].paix = p.x; paternidade[pnovo.x][pnovo.y].paiy = p.y;
            paternidade[pnovo.x][pnovo.y].x = p.x; paternidade[pnovo.x][pnovo.y].y = p.y-1;

            if(pnovo.x == destino.x && pnovo.y == destino.y)
                break;
            else{
                qtd++;
                lista.push_back(pnovo);
                terrenoChar[pnovo.x][pnovo.y][1] = '.';
                visitados[pnovo.x][pnovo.y] = 1;
                limpaTela(); printf("\nExecutando Busca em Largura:\n\n");
                imprimeCenario();
                usleep(1000*DELAYBFS);
            }
        }
    }

    // caminho
    Ponto cam = destino;
    Ponto aux;
    while(paternidade[cam.x][cam.y].paix != -1 && paternidade[cam.x][cam.y].paiy != -1){
        terrenoLarg[cam.x][cam.y][1] = '.';
        custoLarg += terrenoCusto[cam.x][cam.y];

        aux.x = cam.x;
        aux.y = cam.y;
        cam.x = paternidade[aux.x][aux.y].paix;
        cam.y = paternidade[aux.x][aux.y].paiy;
    }

    limpaTela();
    printf("\n\n Caminho encontrado: \n\n");

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(i == inicio.x && j == inicio.y)
                printf("In ");
            else if(i == destino.x && j == destino.y)
                printf("Fi ");
            else
                printf("%c%c ", terrenoLarg[i][j][0], terrenoLarg[i][j][1]);
        }
        printf("\n");
    }

    printf("\nLargura:\n  - Custo: %d\n  - Nos expandidos: %d\n\n", custoLarg, qtd);
}


void uniforme(int i, int j){
    Ponto paternidade[TAM][TAM], inicio;
    int qtd = 0, custoUni = 0;

    for(int k = 0; k < TAM; k++){
        for(int l = 0; l < TAM; l++){
            if(k == i && l == j)
                distancia[k][l] = terrenoCusto[k][l];
            else
                distancia[k][l] = INFINITO;
        }
    }

    inicio.x = i; inicio.y = j;
    paternidade[inicio.x][inicio.y].paix = -1;
    paternidade[inicio.x][inicio.y].paiy = -1;

    limpaTela();
    printf("\nExecutando Busca de custo Uniforme:\n\n");

    while(1){
        Ponto p = buscaMenor_uniforme();

        visitados[p.x][p.y] = 1; // marca p como visitado

        if(p.x == destino.x && p.y == destino.y)// verificar se p = fim, se sim break
            break;

        /* Calcula as distancias dos vizinhos */
        /* Cima */
        if(p.x-1 >= 0 && (distancia[p.x-1][p.y] > distancia[p.x][p.y] + terrenoCusto[p.x-1][p.y]) && !visitados[p.x-1][p.y]){
            distancia[p.x-1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x-1][p.y];
            terrenoChar[p.x-1][p.y][1] = '.';
            limpaTela();
            printf("\nExecutando Busca Uniforme:\n\n");
            imprimeCenario();
            usleep(1000*DELAYUNI);
            paternidade[p.x-1][p.y].paix = p.x; paternidade[p.x-1][p.y].paiy = p.y;
            qtd++;
        }
        /* Direita */
        if(p.y+1 < TAM && (distancia[p.x][p.y+1] > distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1]) && !visitados[p.x][p.y+1]){
            distancia[p.x][p.y+1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1];
            terrenoChar[p.x][p.y+1][1] = '.';
            limpaTela();
            printf("\nExecutando Busca Uniforme:\n\n");
            imprimeCenario();
            usleep(1000*DELAYUNI);
            paternidade[p.x][p.y+1].paix = p.x; paternidade[p.x][p.y+1].paiy = p.y;
            qtd++;
        }
        /* Baixo */
        if(p.x+1 < TAM && (distancia[p.x+1][p.y] > distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y]) && !visitados[p.x+1][p.y]){
            distancia[p.x+1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y];
            terrenoChar[p.x+1][p.y][1] = '.';
            limpaTela();
            printf("\nExecutando Busca Uniforme:\n\n");
            imprimeCenario();
            usleep(1000*DELAYUNI);
            paternidade[p.x+1][p.y].paix = p.x; paternidade[p.x+1][p.y].paiy = p.y;
            qtd++;
        }
        /* Esquerda */
        if(p.y-1 >= 0 && (distancia[p.x][p.y-1] > distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1]) && !visitados[p.x][p.y-1]){
            distancia[p.x][p.y-1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1];
            terrenoChar[p.x][p.y-1][1] = '.';
            limpaTela();
            printf("\nExecutando Busca Uniforme:\n\n");
            imprimeCenario();
            usleep(1000*DELAYUNI);
            paternidade[p.x][p.y-1].paix = p.x; paternidade[p.x][p.y-1].paiy = p.y;
            qtd++;
        }
    }

    // caminho
    Ponto cam = destino;
    Ponto aux;
    while(paternidade[cam.x][cam.y].paix != -1 && paternidade[cam.x][cam.y].paiy != -1){
        terrenoUni[cam.x][cam.y][1] = '.';
        custoUni += terrenoCusto[cam.x][cam.y];

        aux.x = cam.x;
        aux.y = cam.y;
        cam.x = paternidade[aux.x][aux.y].paix;
        cam.y = paternidade[aux.x][aux.y].paiy;
    }

    limpaTela();
    printf("\n\n Caminho encontrado: \n\n");

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(i == inicio.x && j == inicio.y)
                printf("In ");
            else if(i == destino.x && j == destino.y)
                printf("Fi ");
            else
                printf("%c%c ", terrenoUni[i][j][0], terrenoUni[i][j][1]);
        }
        printf("\n");
    }

    printf("\nUniforme:\n  - Custo: %d\n  - Nos expandidos: %d\n\n", custoUni, qtd);
}


Ponto buscaMenor_uniforme(){
    int i, j, min = INFINITO;
    Ponto menor;

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(distancia[i][j] != -1 && distancia[i][j] < min && !visitados[i][j]){
                min = distancia[i][j];
                menor.x = i;
                menor.y = j;
            }
        }
    }
    return menor;
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

    limpaTela();
    printf("\nExecutando Busca A*:\n\n");

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
            limpaTela();
            printf("\nExecutando Busca A*:\n\n");
            imprimeCenario();
            usleep(1000*DELAYA);
            paternidade[p.x-1][p.y].paix = p.x; paternidade[p.x-1][p.y].paiy = p.y;
            qtd++;
        }
        /* Direita */
        if(p.y+1 < TAM && (distancia_estimada[p.x][p.y+1] > (distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1] + manhattan(p.x, p.y+1))) && !visitados[p.x][p.y+1]){
            distancia_estimada[p.x][p.y+1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1] + manhattan(p.x, p.y+1);
            distancia[p.x][p.y+1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y+1];
            terrenoChar[p.x][p.y+1][1] = '.';
            limpaTela();
            printf("\nExecutando Busca A*:\n\n");
            imprimeCenario();
            usleep(1000*DELAYA);
            paternidade[p.x][p.y+1].paix = p.x; paternidade[p.x][p.y+1].paiy = p.y;
            qtd++;
        }
        /* Baixo */
        if(p.x+1 < TAM && (distancia_estimada[p.x+1][p.y] > (distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y] + manhattan(p.x+1, p.y))) && !visitados[p.x+1][p.y]){
            distancia_estimada[p.x+1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y] + manhattan(p.x+1, p.y);
            distancia[p.x+1][p.y] = distancia[p.x][p.y] + terrenoCusto[p.x+1][p.y];
            terrenoChar[p.x+1][p.y][1] = '.';
            limpaTela();
            printf("\nExecutando Busca A*:\n\n");
            imprimeCenario();
            usleep(1000*DELAYA);
            paternidade[p.x+1][p.y].paix = p.x; paternidade[p.x+1][p.y].paiy = p.y;
            qtd++;
        }
        /* Esquerda */
        if(p.y-1 >= 0 && (distancia_estimada[p.x][p.y-1] > (distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1] + manhattan(p.x, p.y-1))) && !visitados[p.x][p.y-1]){
            distancia_estimada[p.x][p.y-1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1] + manhattan(p.x, p.y-1);
            distancia[p.x][p.y-1] = distancia[p.x][p.y] + terrenoCusto[p.x][p.y-1];
            terrenoChar[p.x][p.y-1][1] = '.';
            limpaTela();
            printf("\nExecutando Busca A*:\n\n");
            imprimeCenario();
            usleep(1000*DELAYA);
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


int euclidiana(int i, int j){
    Ponto p;
    p.x = i; p.y = j;

    //return (sqrt((p.x - destino.x)^2 + (p.y - destino.y)^2)) * SUPERESTIMATIVA;
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
