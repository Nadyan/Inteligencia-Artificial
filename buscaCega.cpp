#include <cstdio>
//#include <SFML/Graphics.hpp>

#define TAM 42  //terreno TAMxTAM

typedef struct{
  int x;
  int y;
}Ponto;

Ponto inicio, destino;
int terreno[TAM][TAM];

void montaCenario();

int main(){
    montaCenario();

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

    printf("informe o ponto de inicio e destino x y x y: ");
    scanf("%d %d %d %d", &inicio.x, &inicio.y, &destino.x, &destino.y);

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            fscanf(f, "%d", &terreno[i][j]);
        }
        printf("\n");
    }
}

void imprimeCenario(){
  int i, j;

  for(i = 0; i < TAM; i++){
    for(j = 0; j < TAM; j++){
      if(i == inicio.x && j == inicio.y)
        printf("# ");
      else if(i == destino.x && j == destino.y)
        printf("* ");
      else
        printf("%d ", terreno[i][j]);
    }
  }
}
