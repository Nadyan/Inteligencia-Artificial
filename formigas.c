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
int decideSePega(int i, int j);
int decideSeLarga(int i, int j);


char cenario[TAM][TAM]; // ' ' = nada, '.' = viva, '*' = morta, '+' = carregando
int vivas[TAM][TAM]; // 0 = nada, 1 = formiga vazia, 2 = formiga carregando
int mortas[TAM][TAM]; // 0 = nada, 1 = formiga morta
int espacoVazio[TAM][TAM]; // 0 = nada, 1 = espaco vazio
int raioVisao[RAIO][RAIO]; // matriz de visao da formiga. 0 = nada, 1 = formiga morta


int main(){
    int i, j, formigasVivas, formigasMortas, x, y;

    // inicia a matriz de formigas vivas = nada
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            vivas[i][j] = 0;
            mortas[i][j] = 0;
        }
    }    
    
    formigasVivas = iniciaCenario();
    formigasVivas = 0;
    formigasMortas = 0;

    for(i = 1; i < 250; i++){ // 1500 interacoes
        imprimeCenario();
        usleep(1000*80); // 80ms
        limpaTela();        
        realizaMovimento();
        printf("interacao n: %d\nvivas: %d\nmortas: %d\n", i+1, formigasVivas, formigasMortas);
        formigasVivas = 0; formigasMortas = 0;

        // verificacao de perda ou aumento de itens
        for(x = 0; x < TAM; x++){
            for(y = 0; y < TAM; y++){
                if(vivas[x][y] == 1 || vivas[x][y] == 2)
                    formigasVivas++;
                if(mortas[x][y] == 1)
                    formigasMortas++;
            }
        }
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
            else if(chance > 85){
                cenario[i][j] = '.'; //se der maior que 85, inicia com uma formiga viva
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
                        cenario[i-1][j] = ' ';
                        cenario[i][j] = '+';                    
                    }                
                }
                else if(mortas[i+1][j] == 1){ // se esta no sul
                    decisao = decideSePega(i+1, j);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i+1][j] = 0;
                        cenario[i+1][j] = ' ';
                        cenario[i][j] = '+';
                    }
                }
                else if(mortas[i][j+1] == 1){ // se esta no leste
                    decisao = decideSePega(i, j+1);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i][j+1] = 0;
                        cenario[i][j+1] = ' ';
                        cenario[i][j] = '+';
                    }
                }
                else if(mortas[i][j-1] == 1){ // se esta no oeste
                    decisao = decideSePega(i, j-1);
                    if(decisao == 1){
                        vivas[i][j] = 2;
                        mortas[i][j-1] = 0;
                        cenario[i][j-1] = ' ';
                        cenario[i][j] = '+';                    
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
                    cenario[i-1][j] = '+'; // + é formiga carregada        
                }
                if(movimento > 25 && movimento < 50 && vivas[i+1][j] != 1 && vivas[i+1][j] != 2 && mortas[i+1][j] != 1){ // anda para o sul
                    vivas[i+1][j] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '+';
                }
                if(movimento > 50 && movimento < 75 && vivas[i][j+1] != 1 && vivas[i][j+1] != 2 && mortas[i][j+1] != 1){ // anda para leste
                    vivas[i][j+1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '+';
                }
                if(movimento > 75 && vivas[i][j-1] != 1 && vivas[i][j-1] != 2 && mortas[i][j-1] != 1){ // anda para oeste
                    vivas[i][j-1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '+';
                }

                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSeLarga()
                if(mortas[i-1][j] == 1){ // se esta no norte
                    decisao = decideSeLarga(i-1, j);
                    if(decisao == 1){
                        vivas[i][j] = 1; // formiga passou a estar vazia
                        mortas[i][j] = 1; // larga a morta onde esta
                        cenario[i][j] = '*';
                    }
                }
                else if(mortas[i+1][j] == 1){ // se esta no sul
                    decisao = decideSeLarga(i+1, j);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                        cenario[i][j] = '*';
                    }
                }
                else if(mortas[i][j+1] == 1){ // se esta no leste
                    decisao = decideSeLarga(i, j+1);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                        cenario[i][j] = '*';
                    }
                }
                else if(mortas[i][j-1] == 1){ // se esta no oeste
                    decisao = decideSeLarga(i, j-1);
                    if(decisao == 1){
                        vivas[i][j] = 1;
                        mortas[i][j] = 1;
                        cenario[i][j] = '*';
                    }
                }
            }
        }
    }
}


/* Funcoes de decisao */
int decideSePega(int i, int j){
    int cont = 0, a, b; // contagem de formigas agrupadas
    int r = RAIO, sum = 0;
    float chance;    

    // caso o raio seja maior do que falta para a borda do cenario
    if(i < RAIO)
        r = 1;
    else if(j < RAIO)
        r = 1;

    //srand((unsigned)time(NULL));

    // contagem de mortas em relacao ao raio de visao
    for(a = i - r; a < i + r; a++){
        for(b = j - r; b < j + r; b++){           
            if(mortas[a][b] == 1)
                cont++;
        }    
    }

    /* CALCULO DECISAO */
    for(a = 1; a < RAIO; a++)
        sum += (TAM * a); 

    chance = (sum - cont) / sum;

    if(chance == 1)
        chance -= 0.2; // 98%
    else if(chance == 0)
        chance += 0.2; // 2%


    if(chance > rand() % 100)
        return 1; // pega
    else
        return 0; // nao pega
}

int decideSeLarga(int i, int j){
    int cont = 0, a, b; // contagem de formigas agrupadas
    int r = RAIO, sum = 0;
    float chance;    

    // caso o raio seja maior do que falta para a borda do cenario
    if(i < RAIO)
        r = 1;
    else if(j < RAIO)
        r = 1;

    //srand((unsigned)time(NULL));

    // contagem de mortas em relacao ao raio de visao
    for(a = i - r; a < i + r; a++){
        for(b = j - r; b < j + r; b++){           
            if(mortas[a][b] == 1)
                cont++;
        }    
    }

    /* CALCULO DECISAO */
    for(a = 1; a < RAIO; a++)
        sum += (TAM * a); 

    chance = cont / sum;

    if(chance == 1)
        chance -= 0.2; // 98%
    else if(chance == 0)
        chance += 0.2; // 2%


    if(chance < rand() % 100)
        return 1; // pega
    else
        return 0; // nao pega
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





























