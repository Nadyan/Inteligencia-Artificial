/*
  Nadyan Suriel Pscheidt
  Inteligencia artificial
  Busca Cega

  Compilar com Interface grafica:
  sudo apt-get install libsfml-dev
  g++ -c buscaCega.cpp
  g++ -o buscaCega buscaCega.o -lsfml-graphics -lsfml-window -lsfml-system
  ./buscaCega

  Compilar humildemente pra rodar no terminal:
  g++ -o buscaCega buscaCega.cpp Lista.c Lista.h
  ./buscaCega 
*/

/* Bibs */
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include "Lista.h"
//#include <SFML/Graphics.hpp>

/* Defs */
#define TAM 42  //terreno TAMxTAM

/* struct para os pontos de origem e destino */
typedef struct{
    int x;
    int y;
}Ponto;

/* Vars */
Lista lista;
Ponto inicio, destino;
int terreno[TAM][TAM];
int visitados[TAM][TAM];
char terrenoChar[TAM][TAM][2];
char terrenoLarg[TAM][TAM][2];
char terrenoUni[TAM][TAM][2];
int qtdLarg = 0, qtdUni = 0, custoLarg = 0, custoUni = 0;

/* Funcs */
void montaCenario();
void imprimeCenario();
void limpaTela();
void initVisitados();
void imprimeFinal();
int bfs(int i, int j);
int bfsRun();
int dfs(int i, int j);
int uniforme(int i, int j);


int main(){
    int a; // retorno das funcs
    int i, j;

    printf("Informe o ponto de inicio: ");
    scanf("%d %d", &inicio.x, &inicio.y);
    printf("Informe o ponto de destino: ");
    scanf("%d %d", &destino.x, &destino.y);

    inicializa_lista(&lista, sizeof(Ponto));
    initVisitados();
    montaCenario();
    limpaTela();

    printf("\nExecutando Busca em Largura:\n\n");
    a = bfs(inicio.x, inicio.y);

    /* Montagem da matriz de resultados largura */
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(terrenoChar[i][j][1] == '.'){
                terrenoLarg[i][j][0] = terrenoChar[i][j][0];
                terrenoLarg[i][j][1] = terrenoChar[i][j][1];
            }else{
                terrenoLarg[i][j][0] = ' ';
                terrenoLarg[i][j][1] = ' ';
            }
        }
    }
/*
    initVisitados();
    montaCenario();

    printf("\n\nExecutando Busca Cega Uniforme:\n\n");
    a = uniforme(inicio.x, inicio.y);

    /* Montagem da matriz de resultados uniforme */
  /*  for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(terrenoChar[i][j][1] == '.'){
                terrenoUni[i][j][0] = terrenoChar[i][j][0];
                terrenoUni[i][j][1] = terrenoChar[i][j][1];
            }else{
                terrenoUni[i][j][0] = ' ';
                terrenoUni[i][j][1] = ' ';
            }
        }
    }

    limpaTela();
    imprimeFinal();
    //printf("\nLargura:\n  - Custo: %d\n  - Visitados: %d\n\nUniforme:\n  - Custo: %d\n  - Visitados: %d\n\n", custoLarg, qtdLarg, custoUni, qtdUni);
*/
    // Interface grafica
    /*
    sf::RenderWindow window(sf::VideoMode(800, 800), "Busca Cega");
    sf::RectangleShape rectangle(sf::Vector2f(120, 50));
    rectangle.setFillColor(sf::Color::Blue);

    while(window.isOpen()){
      sf::Event event;
      while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed)
          window.close();
      }

      window.clear(sf::Color::Black);
      window.draw(rectangle);
      window.display();
    }*/

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
                    terrenoChar[i][j][0] = '0'; terrenoChar[i][j][1] = ' '; break;
                case 1:
                    terrenoChar[i][j][0] = '1'; terrenoChar[i][j][1] = ' '; break;
                case 2:
                    terrenoChar[i][j][0] = '2'; terrenoChar[i][j][1] = ' '; break;
                case 3:
                    terrenoChar[i][j][0] = '3'; terrenoChar[i][j][1] = ' '; break;
            }
        }
    }
}

void imprimeCenario(){
    int i, j;

    printf("\n");

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(i == inicio.x && j == inicio.y)
                printf("   ");
            else if(i == destino.x && j == destino.y)
                printf("   ");
            else
                printf("%c%c ", terrenoChar[i][j][0], terrenoChar[i][j][1]);
        }
        printf("\n");
    }
}

int dfs(int i, int j){
    terrenoChar[i][j][1] = '.';
    visitados[i][j] = 1;

    limpaTela();
    printf("\nExecutando Busca em Profundidade:\n\n");
    imprimeCenario();
    usleep(1000*30);

    if(i == destino.x && j == destino.y)
        return 1;
    else{
        /* para cima */
        if(i - 1 >= 0 && !visitados[i-1][j]){
			      bfs(i-1, j);
		    }
        /* para a direita */
        else if(j + 1 < TAM && !visitados[i][j+1]){
			      bfs(i, j+1);
		    }
        /* para baixo */
        else if(i + 1 < TAM && !visitados[i+1][j]){
			      bfs(i+1, j);
		    }
        /* para a esquerda */
        else if(j - 1 >= 0 && !visitados[i][j-1]){
			      bfs(i, j-1);
		    }
    }
}

int bfs(int i, int j){
    terrenoChar[i][j][1] = '.';
    visitados[i][j] = 1;

    // insere no fim da lista

    qtdLarg++; // contagem de posicoes
    custoLarg += terreno[i][j];

    limpaTela();
    printf("\nExecutando Busca em Largura:\n\n");
    imprimeCenario();
    usleep(1000*30);

    if(i == destino.x && j == destino.y)
        return 1;
    else{
        /* para cima */
        if(i - 1 >= 0 && !visitados[i-1][j]){
            // remove do inicio da lista
		}
        /* para a direita */
        else if(j + 1 < TAM && !visitados[i][j+1]){
            // remove do inicio da lista
		}
        /* para baixo */
        else if(i + 1 < TAM && !visitados[i+1][j]){
            // remove do inicio da lista
		}
        /* para a esquerda */
        else if(j - 1 >= 0 && !visitados[i][j-1]){
            // remove do inicio da lista
		}
    }
}
/*
int bfsRun(?){
    open = [e0]; //open contem a lista de nos a serem visitados

    while(open != vazia){
        e = primeiroElementoDeOpen;
        open = removePrimeiroElemento;

        for(i = 0; todo ri em R tal que ri(e) em E){ // determina todos os sucessores de e
            eLinha = ri(e); // e os coloca em open
            if(eLinha é nó final)
                return eLinha;
            open = insereNoFim(eLinha)
        }
    }
    if(open == vazia)
        printf("Nao ha solucao\n");
}*/

int uniforme(int i, int j){
    int menor = 4; // para encontrar o menor
    int i2 = 42, j2 = 42;

    qtdUni++; // contagem de posicoes
    custoUni += terreno[i][j];

  	  /* Caso origem: 1 1
  	          destino 30 35
  	          funciona bem
      */

      /* DESCOBRIR FORMA DE TRATAR CAMINHOS SEM SAIDA */

    terrenoChar[i][j][1] = '.';
    visitados[i][j] = 1;

    limpaTela();
    printf("\n\nExecutando Busca Cega Uniforme:\n\n");
    imprimeCenario();
    usleep(1000*30);

    if(i == destino.x && j == destino.y)
        return 1;
    else{
        /* escolha do vizinho de menor custo */
        /* se for para cima */
        if(i - 1 >= 0 && !visitados[i-1][j]){
            if(terreno[i-1][j] < menor ){
                i2 = i-1; j2 = j;
                menor = terreno[i-1][j];
            }
        }
        /* se for para a direitra */
        if(j + 1 < TAM && !visitados[i][j+1]){
            if(terreno[i][j+1] < menor){
                i2 = i; j2 = j+1;
                menor = terreno[i][j+1];
            }
        }
        /* se for para baixo */
        if(i + 1 < TAM && !visitados[i+1][j]){
            if(terreno[i+1][j] < menor){
                i2 = i+1; j2 = j;
                menor = terreno[i+1][j];
            }
        }
        /* se for para a esquerda */
        if(j - 1 >= 0 && !visitados[i][j-1]){
            if(terreno[i][j-1] < menor){
                i2 = i; j2 = j-1;
                menor = terreno[i][j-1];
            }
        }
        if(i2 != 42 && j2 != 42) /* se entrou em algum if */
            uniforme(i2, j2);
        else{
            printf("\n\nFalha na busca, caminho nao encontrado!\n\n");
            return 1;
        }
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

void imprimeFinal(){
    int i, j;

    printf("\nCaminho percorrido pela busca em largura\n\n");

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(i == inicio.x && j == inicio.y)
                printf("I  ");
            else if(i == destino.x && j == destino.y)
                printf("F  ");
            else
                printf("%c%c ", terrenoLarg[i][j][0], terrenoLarg[i][j][1]);
        }
        printf("\n");
    }

    printf("\n\nCaminho percorrido pela busca de custo uniforme:\n\n");

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
          if(i == inicio.x && j == inicio.y)
              printf("I  ");
          else if(i == destino.x && j == destino.y)
              printf("F  ");
          else
              printf("%c%c ", terrenoUni[i][j][0], terrenoUni[i][j][1]);
        }
        printf("\n");
    }

    printf("\nLargura:\n  - Custo: %d\n  - Visitados: %d\n\nUniforme:\n  - Custo: %d\n  - Visitados: %d\n\n", custoLarg, qtdLarg, custoUni, qtdUni);
}

void limpaTela(){
    system("clear");
}
