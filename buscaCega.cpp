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
int visitados[TAM][TAM];

void montaCenario();
void imprimeCenario();
void limpaTela();
void dfs(int i, int j);

int main(){

  for(int i = 0; i < TAM; i++){
    for(int j = 0; j < TAM; j++){
      visitados[i][j] = 0;
    }
  }

  montaCenario();
  imprimeCenario();

  int start1 = inicio.x, star2 = inicio.y;

  dfs(start1, star2);

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

void dfs(int i, int j){

  visitados[i][j] = 1;

  for(j = 0; j < TAM; j++){
    for(j = 0; j < TAM; j++){
      if(!visitados[i][j] && terreno[i][j] != -1){
        terreno[i][j] = 4;
        limpaTela();
        imprimeCenario();
        dfs(j, i);
      }
    }
  }
}

void limpaTela(){
    system("cls || clear");
}
