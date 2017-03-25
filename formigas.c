#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM 50 // cenario TAMxTAM
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
    
    srand((unsigned)time(NULL));

    formigasVivas = iniciaCenario();
    formigasVivas = 0;
    formigasMortas = 0;

    imprimeCenario();

    for(i = 0; i < 900; i++){ // interacoes
        imprimeCenario();
        usleep(1000*80); // 1000*N = Nms
        limpaTela();        
        realizaMovimento();
        printf("interacao n: %d\nvivas: %d\nmortas: %d\nrand: %d\n", i+1, formigasVivas, formigasMortas, rand()%100);
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

    for(i = 1; i < TAM-1; i++){
        for(j = 1; j < TAM-1; j++){

            chance = rand() % 100;

            if(chance < 20){ //de 0 a 100, 20% de chance de ter morta
                cenario[i][j] = '*'; //se a funcao de aleatorio der < 25, inicia com uma formiga morta
                qtdFormigasMortas++;
                mortas[i][j] = 1;
            }            
            else if(chance > 90){
                cenario[i][j] = '.'; //se der maior que 90, inicia com uma formiga viva
                qtdFormigasVivas++;
                vivas[i][j] = 1;
            }else{
                cenario[i][j] = ' '; //senao inicia com a celula vazia
                qtdCelulasVazias++;
                espacoVazio[i][j] = 1;
            }
        }
    }

    return qtdFormigasVivas;
}


void realizaMovimento(){
    int i, j, movimento;
    int decisao, achou;
    int d, e; // d = i deslicado, e = j deslocado (deslocado signf. que é a variavel depois do movimento)
    
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            achou = 0; // flag para sinalizar que encontrou uma morta
            if(vivas[i][j] == 1){ // se a formiga em i j está vazia
                /* anda para alguma casa aleatoria */
                movimento = rand() % 100;
                
                if(i != 0 && movimento < 25 && vivas[i-1][j] != 1 && vivas[i-1][j] != 2 && mortas[i-1][j] != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i-1][j] = '.';  
                    d = i-1; // coordenada i deslocada
                    e = j;   // coordenada j deslocada
                }
                else if(i != TAM-1 && movimento > 25 && movimento < 50 && vivas[i+1][j] != 1 && vivas[i+1][j] != 2 && mortas[i+1][j] != 1){ // anda para o sul
                    vivas[i+1][j] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '.';
                    d = i+1;
                    e = j;
                }
                else if(j != TAM-1 && movimento > 50 && movimento < 75 && vivas[i][j+1] != 1 && vivas[i][j+1] != 2 && mortas[i][j+1] != 1){ // anda para leste
                    vivas[i][j+1] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '.';
                    d = i;
                    e = j+1;
                }
                else if(j != 0 && movimento > 75 && vivas[i][j-1] != 1 && vivas[i][j-1] != 2 && mortas[i][j-1] != 1){ // anda para oeste
                    vivas[i][j-1] = 1;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '.';
                    d = i;
                    e = j-1;               
                }
                
                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSePega()
                if(d > 0 && d < TAM-1 && e < TAM-1 && e > 0){  // se nao tá em nenhuma borda                  
                    if(mortas[d-1][e] == 1 && achou == 0){ // se esta no norte
                        decisao = decideSePega(d-1, e);
                        if(decisao == 1){
                            vivas[d][e] = 2; // formiga passou a estar carregando
                            mortas[d-1][e] = 0; // remove a morta
                            cenario[d-1][e] = ' ';
                            cenario[d][e] = '+';                
                        }    
                        achou = 1;            
                    }
                    if(mortas[d+1][e] == 1 && achou == 0){ // se esta no sul
                        decisao = decideSePega(d+1, e);
                        if(decisao == 1){
                            vivas[d][e] = 2;
                            mortas[d+1][e] = 0;
                            cenario[d+1][e] = ' ';
                            cenario[d][e] = '+';
                        }
                        achou = 1;
                    }               
                    if(mortas[d][e+1] == 1 && achou == 0){ // se esta no leste
                        decisao = decideSePega(d, e+1);
                        if(decisao == 1){
                            vivas[d][e] = 2;
                            mortas[d][e+1] = 0;
                            cenario[d][e+1] = ' ';
                            cenario[d][e] = '+';
                        }
                        achou = 1;
                    }
                    if(mortas[d][e-1] == 1 && achou == 0){ // se esta no oeste
                        decisao = decideSePega(d, e-1);
                        if(decisao == 1){
                            vivas[d][e] = 2;
                            mortas[d][e-1] = 0;
                            cenario[d][e-1] = ' ';
                            cenario[d][e] = '+';                    
                        }
                        achou = 1;
                    }
                }
            }

            else if(vivas[i][j] == 2){ // se a formiga em i j estiver carregando
                /* anda para uma casa aleatoria */
                movimento = rand() % 100;

                if(i != 0 && movimento < 25 && vivas[i-1][j] != 1 && vivas[i-1][j] != 2 && mortas[i-1][j] != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i-1][j] = '+'; // + é formiga carregada                    
                    d = i-1;
                    e = j;       
                }
                else if(i != TAM-1 && movimento > 25 && movimento < 50 && vivas[i+1][j] != 1 && vivas[i+1][j] != 2 && mortas[i+1][j] != 1){ // anda para o sul
                    vivas[i+1][j] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '+';                    
                    d = i+1;
                    e = j;
                }
                else if(j != TAM-1 && movimento > 50 && movimento < 75 && vivas[i][j+1] != 1 && vivas[i][j+1] != 2 && mortas[i][j+1] != 1){ // anda para leste
                    vivas[i][j+1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '+';
                    d = i;
                    e = j+1;
                }
                else if(j != 0 && movimento > 75 && vivas[i][j-1] != 1 && vivas[i][j-1] != 2 && mortas[i][j-1] != 1){ // anda para oeste
                    vivas[i][j-1] = 2;
                    vivas[i][j] = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '+';
                    d = i;
                    e = j-1;                
                }
        
                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSeLarga()
                if(d > 0 && d < TAM-1 && e < TAM-1 && e > 0){ // condicao para nao sair da matriz
                    if(mortas[d-1][e] == 1 && achou == 0){ // se esta no norte
                        decisao = decideSeLarga(d-1, e);
                        if(decisao == 1){
                            vivas[d][e] = 1; // formiga passou a estar vazia
                            mortas[d][e] = 1; // larga a morta onde esta
                            cenario[d][e] = '*';
                        }
                        achou = 1;
                    }                  
                    if(mortas[d+1][e] == 1 && achou == 0){ // se esta no sul
                        decisao = decideSeLarga(d+1, e);
                        if(decisao == 1){
                            vivas[d][e] = 1;
                            mortas[d][e] = 1;
                            cenario[d][e] = '*';
                        }
                        achou = 1;
                    }
                    if(mortas[d][e+1] == 1 && achou == 0){ // se esta no leste
                        decisao = decideSeLarga(d, e+1);
                        if(decisao == 1){
                            vivas[d][e] = 1;
                            mortas[d][e] = 1;
                            cenario[d][e] = '*';
                        }
                        achou = 1;
                    }
                    if(mortas[d][e-1] == 1 && achou == 0){ // se esta no oeste
                        decisao = decideSeLarga(d, e-1);
                        if(decisao == 1){
                            vivas[d][e] = 1;
                            mortas[d][e] = 1;
                            cenario[d][e] = '*';
                        }
                        achou = 1;
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
    int rvc; // raio vertical para cima
    int rvb; // raio vertical para baixo
    int rhe; // raio horizontal para esquerda
    int rhd; // raio horizontal para direita

    //printf("decide se pega\n");

    rvc = r; rvb = r; rhe = r; rhd = r;

    // tratamento de bordas
    if(i < r && j > r) // borda superior
        rvc = i;
    else if(i > r && j < r) // borda esquerda
        rhe = j;
    if(TAM < i + r && j + r < TAM) // borda inferior
        rvb = (TAM-1) - i;
    else if(TAM < j + r && i + r < TAM) // borda direita
        rhd = (TAM-1) - j;
    if(i < r && j < r){ // canto superior esquerdo
        rhe = j;
        rvc = i;
    }
    else if(i < r && j + r < TAM){ // canto superior direito
        rhd = (TAM-1) - j;
        rvc = i;
    }
    if(i + r > TAM && j + r > TAM){ // canto inferior direito
        rhd = (TAM-1) - j;
        rvb = (TAM-1) - i;
    }
    else if(i + r > TAM && j < r){ // canto inferior esquerdo
        rvb = (TAM-1) - i;
        rhe = j;
    }


    // contagem de mortas em relacao ao raio de visao
    for(a = i - rvc; a < i + rvb; a++){
        for(b = j - rhe; b < j + rhd; b++){           
            if(mortas[a][b] == 1)
                cont++;
        }    
    }

    // CALCULO DECISAO
    for(a = 1; a < r; a++)
        sum += (TAM * a); 

    chance = (sum - cont) / sum;

    if(chance == 1)
        chance -= 0.2; // 98%
    else if(chance == 0)
        chance += 0.2; // 2%


    if((int)chance > rand() % 100){
        return 1; //printf("pegou\n");// pega
    }else{
        return 0; //printf("nao pegou\n");// nao pega
    }
}

int decideSeLarga(int i, int j){
    int cont = 0, a, b; // contagem de formigas agrupadas
    int r = RAIO, sum = 0;
    float chance;    
    int rvc; // raio vertical para cima
    int rvb; // raio vertical para baixo
    int rhe; // raio horizontal para esquerda
    int rhd; // raio horizontal para direita

    //printf("decide se pega\n");

    rvc = r; rvb = r; rhe = r; rhd = r;

    // tratamento de bordas
    if(i < r && j > r) // borda superior
        rvc = i;
    else if(i > r && j < r) // borda esquerda
        rhe = j;
    if(TAM < i + r && j + r < TAM) // borda inferior
        rvb = (TAM-1) - i;
    else if(TAM < j + r && i + r < TAM) // borda direita
        rhd = (TAM-1) - j;
    if(i < r && j < r){ // canto superior esquerdo
        rhe = j;
        rvc = i;
    }
    else if(i < r && j + r < TAM){ // canto superior direito
        rhd = (TAM-1) - j;
        rvc = i;
    }
    if(i + r > TAM && j + r > TAM){ // canto inferior direito
        rhd = (TAM-1) - j;
        rvb = (TAM-1) - i;
    }
    else if(i + r > TAM && j < r){ // canto inferior esquerdo
        rvb = (TAM-1) - i;
        rhe = j;
    }


    // contagem de mortas em relacao ao raio de visao
    for(a = i - rvc; a < i + rvb; a++){
        for(b = j - rhe; b < j + rhd; b++){           
            if(mortas[a][b] == 1)
                cont++;
        }    
    }

    // CALCULO DECISAO
    for(a = 1; a < r; a++)
        sum += (TAM * a); 

    chance = cont / sum;

    if(chance == 1)
        chance -= 0.2; // 98%
    else if(chance == 0)
        chance += 0.2; // 2%


    if((int)chance < rand() % 100){
        return 1; //printf("largou\n"); // pega
    }else{
        return 0; //printf("nao largou\n");// nao pega
    }
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





























