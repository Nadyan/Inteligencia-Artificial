/*
  Nadyan Suriel Pscheidt
  Inteligencia artificial
  Busca Cega

  g++ -o buscaCega buscaCega.cpp
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
//#include <SFML/Graphics.hpp>

#define TAM 42  //terreno TAMxTAM

/* struct para os pontos de origem e destino */
typedef struct{
  int x;
  int y;
}Ponto;

Ponto inicio, destino;
int terreno[TAM][TAM];
char terrenoChar[TAM][TAM];
int visitados[TAM][TAM];
int frente = 0, cauda = 0;
int fila[TAM];

void montaCenario();
void imprimeCenario();
void limpaTela();
int bfs(int i, int j);

int main(){
  int a; // retorno do bfs

  for(int i = 0; i < TAM; i++){
    for(int j = 0; j < TAM; j++){
      visitados[i][j] = 0;
    }
  }

  montaCenario();
  imprimeCenario();

  int start1 = inicio.x, start2 = inicio.y;

  //bfs(start1, star2);
  a = bfs(start1, start2);

  imprimeCenario();


  /* //Parte grafica
  sf::RenderWindow window(sf::VideoMode(800, 600), "Busca Cega");

  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::Black);

    // draw here

    window.display();
  }*/

	return 0;
}

void montaCenario(){
    FILE *f = fopen("terreno.txt", "r");
    int i, j;

    printf("Informe o ponto de inicio(#): ");
    scanf("%d %d", &inicio.x, &inicio.y);
    printf("Informe o ponto de destino(*): ");
    scanf("%d %d", &destino.x, &destino.y);

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            fscanf(f, "%d", &terreno[i][j]);

            switch(terreno[i][j]){
              case 0:
                terrenoChar[i][j] = '0'; break;
              case 1:
                terrenoChar[i][j] = '1'; break;
              case 2:
                terrenoChar[i][j] = '2'; break;
              case 3:
                terrenoChar[i][j] = '3'; break;
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
        printf("  ");
      else if(i == destino.x && j == destino.y)
        printf("  ");
      else
        printf("%c ", terrenoChar[i][j]);
    }
    printf("\n");
  }
}

int bfs(int i, int j){
  terrenoChar[i][j] = '*';
  visitados[i][j] = 1;

  imprimeCenario();
  usleep(1000*10);
  limpaTela();

  if(i == destino.x && j == destino.y)
    return 1;
  else{
    /* para cima */
    if(i - 1 >= 0 && !visitados[i-1][j])
      bfs(i-1, j);
    /* para a direita */
    else if(j + 1 < TAM && !visitados[i][j+1])
      bfs(i, j+1);
    /* para baixo */
    else if(i + 1 < TAM && !visitados[i+1][j])
      bfs(i+1, j);
    /* para a esquerda */
    else if(j - 1 >= 0 && !visitados[i][j-1])
      bfs(i, j-1);
  }
}

void limpaTela(){
    system("clear");
}
