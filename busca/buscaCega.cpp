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
//#include <SFML/Graphics.hpp>

/* Defs */
#define TAM 42  //terreno TAMxTAM

/* struct para os pontos de origem e destino */
typedef struct{
    int x;
    int y;
    int paix, paiy;
}Ponto;

/* Vars */
std::list<Ponto> lista;
Ponto origem, destino;
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
void bfs(int i, int j);
int dfs(int i, int j);
void uniforme(int i, int j);
int buscaGulosa(int i, int j);

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

    /* Montagem da matriz de resultados largura */
    /*for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            if(terrenoChar[i][j][1] == '.'){
                terrenoLarg[i][j][0] = terrenoChar[i][j][0];
                terrenoLarg[i][j][1] = terrenoChar[i][j][1];
            }else{
                terrenoLarg[i][j][0] = ' ';
                terrenoLarg[i][j][1] = ' ';
            }
        }
    }*/

    //initVisitados();
    //montaCenario();

    //printf("\n\nExecutando Busca Cega Uniforme:\n\n");
    //uniforme(inicio.x, inicio.y);

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
            terrenoLarg[i][j][0] = terrenoChar[i][j][0];
            terrenoLarg[i][j][1] = terrenoChar[i][j][1];
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
			      dfs(i-1, j);
		}
        /* para a direita */
        else if(j + 1 < TAM && !visitados[i][j+1]){
			      dfs(i, j+1);
		}
        /* para baixo */
        else if(i + 1 < TAM && !visitados[i+1][j]){
			      dfs(i+1, j);
		}
        /* para a esquerda */
        else if(j - 1 >= 0 && !visitados[i][j-1]){
			      dfs(i, j-1);
		}
    }
}

void bfs(int i, int j){
    Ponto paternidade[TAM][TAM];
    int qtd = 0;

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
                limpaTela();
                imprimeCenario();
                usleep(1000*30);
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
                limpaTela();
                imprimeCenario();
                usleep(1000*30);
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
                limpaTela();
                imprimeCenario();
                usleep(1000*30);
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
                limpaTela();
                imprimeCenario();
                usleep(1000*30);
            }
        }
    }

    // caminho
    Ponto cam = destino;
    while(paternidade[cam.x][cam.y].paix != -1 && paternidade[cam.x][cam.y].paiy != -1){
        terrenoLarg[cam.x][cam.y][1] = '.';
        custoLarg += terreno[cam.x][cam.y];

        cam.x = paternidade[cam.x][cam.y].paix;
        cam.y = paternidade[cam.x][cam.y].paiy;
    }

    limpaTela();
    printf("\n\n");

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

    printf("\nLargura:\n  - Custo: %d\n  - Visitados: %d\n\n%d %d\n\n", custoLarg, qtd);//, paternidade[21][25].paix, paternidade[20][25].paiy);
}

void uniforme(int i, int j){
    Ponto paternidade[TAM][TAM];
    int qtd = 0, custo = 4;

    qtdUni++; // contagem de posicoes

    limpaTela();
    printf("\nExecutando Busca de custo Uniforme:\n\n");

    visitados[i][j] = 1;

    Ponto inicio;
    inicio.x = i; inicio.y = j;
    paternidade[inicio.x][inicio.y].paix = -1;  // no raiz
    paternidade[inicio.x][inicio.y].paiy = -1;  // no raiz

    lista.push_front(inicio);

    while(!lista.empty()){
        Ponto p = lista.front();
        Ponto pnovo;
        lista.pop_front();

        /* Encontrar o vizinho de menor custo */
        /* Cima */
        if(p.x - 1 >= 0 && !visitados[p.x-1][p.y] && terreno[p.x-1][p.y] < custo){
            custo = terreno[p.x-1][p.y];
            pnovo.x = p.x-1;
            pnovo.y = p.y;
        }
        /* Direita */
        if(p.y + 1 < TAM && visitados[p.x][p.y+1] && terreno[p.x][p.y+1] < custo){
            custo = terreno[p.x][p.y+1];
            pnovo.x = p.x;
            pnovo.y = p.y+1;
        }
        /* Baixo */
        if(p.x + 1 < TAM && !visitados[p.x+1][p.y] && terreno[p.x+1][p.y] < custo){
            custo = terreno[p.x+1][p.y];
            pnovo.x = p.x+1;
            pnovo.y = p.y;
        }
        /* Esquerda */
        if(p.y - 1 >= 0 && !visitados[p.x][p.y-1] && terreno[p.x][p.y-1] < custo){
            custo = 4; // reinicia o custo
            pnovo.x = p.x;
            pnovo.y = p.y-1;
        }

        visitados[pnovo.x][pnovo.y] = 1;

        /* linha 93 do dijkstra (for) */
    }


}


int buscaGulosa(int i, int j){
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
    printf("\n\nExecutando Busca Cega Gulosa:\n\n");
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
            if(i == origem.x && j == origem.y)
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
          if(i == origem.x && j == origem.y)
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
