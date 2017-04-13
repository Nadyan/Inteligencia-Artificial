/*
  Nadyan Suriel Pscheidt
  Inteligencia artificial
  Busca Cega

  g++ -o buscaCega buscaCega.cpp
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
//#include <SFML/Graphics.hpp>

#define TAM 42  //terreno TAMxTAM

/* struct para os pontos de origem e destino */
typedef struct{
  int x;
  int y;
}Ponto;

Ponto inicio, destino;
int terreno[TAM][TAM];

void montaCenario();
void imprimeCenario();
void limpaTela();
void dfs(int i, int *visitados);

int main(){
  int visitados[TAM*TAM]; // vetor de posicoes visitadas
  memset(visitados, 0, sizeof(visitados)); // zera o vetor

  montaCenario();
  imprimeCenario();

  dfs(0, visitados);

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
    printf("Informe o ponto de destino(#): ");
    scanf("%d %d", &destino.x, &destino.y);

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            fscanf(f, "%d", &terreno[i][j]);
        }
    }
}

void imprimeCenario(){
  int i, j;

  printf("\n");

  for(i = 0; i < TAM; i++){
    for(j = 0; j < TAM; j++){
      if(i == inicio.x && j == inicio.y)
        printf("# ");
      else if(i == destino.x && j == destino.y)
        printf("* ");
      else
        printf("%d ", terreno[i][j]);
    }
    printf("\n");
  }
}

void dfs(int i, int *visitados){
  int j;

  visitados[i] = 1;

  for(j = 0; j < TAM; j++){
    if(!visitados[j] && terreno[i][j] != -1){
      terreno[i][j] = 4;
      limpaTela();
      imprimeCenario();
      dfs(j, visitados);
    }
  }
}

void limpaTela(){
    system("cls || clear");
}
