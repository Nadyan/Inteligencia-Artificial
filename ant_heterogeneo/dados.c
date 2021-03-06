/*
    Nadyan Suriel Pscheidt
    Inteligência Artificial
    Ant Clustering com dados heterogeneos

    gcc -o dados dados.c -lm
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define TAM 50 // cenario TAMxTAM
#define RAIO 1
#define ALFA 8 // fator de dissimilaridade
#define TEMPO 50
#define QTD_DADOS 400
#define K1 0.014    // valor inicial = 0.014
#define K2 0.615    // valor inicial = 0.615

// parametros do print
#define PRINT1 1
#define PRINT2 20000
#define PRINT3 40000
#define PRINT4 60000
#define PRINT5 100000
#define PRINT6 200000

typedef struct{
    double col1;    // coluna 1
    double col2;    // coluna 2
    int tipo;       // 1, 2, 3 e 4
    int idx;        // apenas para mostrar se tem ou nao dado na matriz mortas
}FormigaMorta;

typedef struct{
    int idx; // 1 = vazia, 2 = cheia
    
    //caracteristicas caso esteja carregando uma morta (vazia = 2)    
    double col1;
    double col2;
    int tipo;
}FormigaViva;

void iniciaCenario();
void imprimeCenario();
void realizaMovimento();
void limpaTela();
int decideSePega(int i, int j, int tipo);
int decideSeLarga(int i, int j, int tipo);


char cenario[TAM][TAM];     // ' ' = nada, '.' = viva, '*' = morta, '+' = carregando
int espacoVazio[TAM][TAM];  // 0 = nada, 1 = espaco vazio
int raioVisao[RAIO][RAIO];  // matriz de visao da formiga. 0 = nada, 1 = formiga morta
FormigaMorta dados[400];
FormigaMorta mortas[TAM][TAM];
FormigaViva vivas[TAM][TAM]; // 0 = nada, 1 = formiga vazia, 2 = formiga carregando


int main(){
    int j, formigasVivas = 0, formigasMortas = 0, x, y, novaSemente = 0, qtdSementesUsadas = 1;
    long int i;
    FILE *f = fopen("dados.txt", "r");

    if(f == NULL){
        printf("Erro ao ler arquivo de dados!\n");
        return 1;
    }

    // iniciar matriz de dados com o arquivo
    for(i = 0; i < QTD_DADOS; i++){
        fscanf(f, "%lf", &dados[i].col1);
        fscanf(f, "%lf", &dados[i].col2);
        fscanf(f, "%d", &dados[i].tipo);
    }

    // inicia a matriz de formigas vivas = nada
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            vivas[i][j].idx = 0;
            mortas[i][j].idx = 0;
            cenario[i][j] = ' ';
        }
    }    
    
    srand((unsigned)time(NULL));

    iniciaCenario();

    for(i = 1; i <= 60000; i++){ // interacoes        
        imprimeCenario();
        usleep(1000*TEMPO); // 1000*N = Nms
        novaSemente += TEMPO; // adiciona 50ms para geracao da nova semente         
        realizaMovimento();
        limpaTela();
        printf("interacao n: %ld\nvivas: %d\nmortas: %d\nrand: %d\nqtd sementes utilizadas: %d\n", i+1, formigasVivas, formigasMortas, rand()%100, qtdSementesUsadas);
        //printf("interacao n: %ld\n", i);
        formigasVivas = 0; formigasMortas = 0;

        // verificacao de perda ou aumento de itens
        for(x = 0; x < TAM; x++){
            for(y = 0; y < TAM; y++){
                if(vivas[x][y].idx == 1 || vivas[x][y].idx == 2)
                    formigasVivas++;
                if(mortas[x][y].idx == 1 || vivas[x][y].idx == 2)
                    formigasMortas++;
            }
        }
        
        if(novaSemente == 1000){
            srand((unsigned)time(NULL));
            novaSemente = 0;
            qtdSementesUsadas++;
        }

        // if utilizado apenas para os experimentos
        /*if(i == PRINT1 || i == PRINT2 || i == PRINT3 || i == PRINT4 || i == PRINT5 || i == PRINT6){
            imprimeCenario();   
            printf("\n---- %ld\n", i);
        }*/
    }

    imprimeCenario();

    return 0;
}


void iniciaCenario(){
    int i, j;
    int chance, qual, qtdFormigasVivas = 0, qtdFormigasMortas = 0, qtdCelulasVazias = 0;
    int escolhida[QTD_DADOS];

    for(i = 1; i < QTD_DADOS; i++)
        escolhida[i] = 0; // vetor de mortas que ja foram distribuidas no cenario

    for(i = 1; i < TAM-1; i++){
        for(j = 1; j < TAM-1; j++){

            chance = rand() % 100;

            if(chance < 5){
                cenario[i][j] = '.';
                vivas[i][j].idx = 1;
                qtdFormigasVivas++;
            }else{
                cenario[i][j] = ' ';
                espacoVazio[i][j] = 1;
            }
        }
    }
     
    // espalhar os 400 dados pelo cenario de forma aleatoria
    while(qtdFormigasMortas < QTD_DADOS-1){        
        for(i = 1; i < TAM-1; i++){
            for(j = 1; j < TAM-1; j++){
                if(espacoVazio[i][j] == 1){

                    chance = rand() % 100; // chance de cair uma morta na posicao i, j

                    if(chance > 90){
                        qual = rand() % QTD_DADOS; // sorteia qual das mortas

                        if(escolhida[qual] == 0){
                            mortas[i][j] = dados[qual];
                            mortas[i][j].idx = 1;
                            qtdFormigasMortas++;
                            //printf("%d\n", qtdFormigasMortas);
                            espacoVazio[i][j] = 0;
                            escolhida[qual] = 1;                            

                            if(dados[qual].tipo == 1)
                                cenario[i][j] = '1';
                            else if(dados[qual].tipo == 2)
                                cenario[i][j] = '2';
                            else if(dados[qual].tipo == 3)
                                cenario[i][j] = '3';
                            else if(dados[qual].tipo == 4)
                                cenario[i][j] = '4';
                        }
                    }
                }
            }
        }
    }
}


void realizaMovimento(){
    int i, j, movimento;
    int decisao, achou, andou;
    int d, e; // d = i deslicado, e = j deslocado (deslocado signf. que é a variavel depois do movimento)
    
    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            achou = 0; // flag para sinalizar que encontrou uma morta
            andou = 0;
            if(vivas[i][j].idx == 1){ // se a formiga em i j está vazia
                // anda para alguma casa aleatoria
                movimento = rand() % 100;
                
                if(i != 0 && movimento < 25 && vivas[i-1][j].idx != 1 && vivas[i-1][j].idx != 2 && mortas[i-1][j].idx != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j].idx = 1;                    
                    vivas[i][j].idx = 0;

                    if(mortas[i][j].idx == 0)                
                        cenario[i][j] = ' ';
                    else{
                        if(mortas[i][j].tipo == 1)                        
                            cenario[i][j] = '1';
                        else if(mortas[i][j].tipo == 2)
                            cenario[i][j] = '2';
                        else if(mortas[i][j].tipo == 3)
                            cenario[i][j] = '3';
                        else if(mortas[i][j].tipo == 4)
                            cenario[i][j] = '4';
                    }                    

                    cenario[i-1][j] = '.';  
                    d = i-1; // coordenada i deslocada
                    e = j;   // coordenada j deslocada

                    andou = 1;
                }
                else if(i != TAM-1 && (movimento >= 25 && movimento < 50) && vivas[i+1][j].idx != 1 && vivas[i+1][j].idx != 2 && mortas[i+1][j].idx != 1){ // anda para o sul
                    vivas[i+1][j].idx = 1;
                    vivas[i][j].idx = 0;

                    if(mortas[i][j].idx == 0)                
                        cenario[i][j] = ' ';
                    else{
                        if(mortas[i][j].tipo == 1)                        
                            cenario[i][j] = '1';
                        else if(mortas[i][j].tipo == 2)
                            cenario[i][j] = '2';
                        else if(mortas[i][j].tipo == 3)
                            cenario[i][j] = '3';
                        else if(mortas[i][j].tipo == 4)
                            cenario[i][j] = '4';
                    }  

                    cenario[i+1][j] = '.';
                    d = i+1;
                    e = j;

                    andou = 1;
                }
                else if(j != TAM-1 && (movimento >= 50 && movimento < 75) && vivas[i][j+1].idx != 1 && vivas[i][j+1].idx != 2 && mortas[i][j+1].idx != 1){ // anda para leste
                    vivas[i][j+1].idx = 1;
                    vivas[i][j].idx = 0;

                    if(mortas[i][j].idx == 0)                
                        cenario[i][j] = ' ';
                    else{
                        if(mortas[i][j].tipo == 1)                        
                            cenario[i][j] = '1';
                        else if(mortas[i][j].tipo == 2)
                            cenario[i][j] = '2';
                        else if(mortas[i][j].tipo == 3)
                            cenario[i][j] = '3';
                        else if(mortas[i][j].tipo == 4)
                            cenario[i][j] = '4';
                    }  

                    cenario[i][j+1] = '.';
                    d = i;
                    e = j+1;

                    andou = 1;
                }
                else if(j != 0 && movimento >= 75 && vivas[i][j-1].idx != 1 && vivas[i][j-1].idx != 2 && mortas[i][j-1].idx != 1){ // anda para oeste
                    vivas[i][j-1].idx = 1;
                    vivas[i][j].idx = 0;

                    if(mortas[i][j].idx == 0)                
                        cenario[i][j] = ' ';
                    else{
                        if(mortas[i][j].tipo == 1)                        
                            cenario[i][j] = '1';
                        else if(mortas[i][j].tipo == 2)
                            cenario[i][j] = '2';
                        else if(mortas[i][j].tipo == 3)
                            cenario[i][j] = '3';
                        else if(mortas[i][j].tipo == 4)
                            cenario[i][j] = '4';
                    }  

                    cenario[i][j-1] = '.';
                    d = i;
                    e = j-1;  

                    andou = 1;             
                }
                
                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSePega()
                if(andou == 1){                    
                    if(d > 0 && d < TAM-1 && e < TAM-1 && e > 0){  // se nao tá em nenhuma borda                  
                        if(mortas[d-1][e].idx == 1 && achou == 0 && vivas[d-1][e].idx == 0){ // se esta no norte
                            decisao = decideSePega(d-1, e, mortas[d-1][e].tipo);
                            if(decisao == 1){
                                vivas[d][e].idx = 0; // formiga passou a estar carregando
                                vivas[d-1][e].idx = 2;
                                mortas[d-1][e].idx = 0; // remove a morta
                                cenario[d-1][e] = '+';
                                cenario[d][e] = ' '; 

                                // atribui as caracteristicas da morta para a viva
                                vivas[d-1][e].col1 = mortas[d-1][e].col1; 
                                vivas[d-1][e].col2 = mortas[d-1][e].col2;
                                vivas[d-1][e].tipo = mortas[d-1][e].tipo;
                                //mortas[d-1][e].col1 = 0; mortas[d-1][e].col2 = 0; mortas[d-1][e].tipo = 0;              
                            }    
                            achou = 1;            
                        }
                        if(mortas[d+1][e].idx == 1 && achou == 0 && vivas[d+1][e].idx == 0){ // se esta no sul
                            decisao = decideSePega(d+1, e, mortas[d+1][e].tipo);
                            if(decisao == 1){
                                vivas[d][e].idx = 0;
                                vivas[d+1][e].idx = 2;
                                mortas[d+1][e].idx = 0;
                                cenario[d+1][e] = '+';
                                cenario[d][e] = ' '; 

                                vivas[d+1][e].col1 = mortas[d+1][e].col1;
                                vivas[d+1][e].col2 = mortas[d+1][e].col2;
                                vivas[d+1][e].tipo = mortas[d+1][e].tipo;
                            }
                            achou = 1;
                        }               
                        if(mortas[d][e+1].idx == 1 && achou == 0 && vivas[d][e+1].idx == 0){ // se esta no leste
                            decisao = decideSePega(d, e+1, mortas[d][e+1].tipo);
                            if(decisao == 1){
                                vivas[d][e].idx = 0;
                                vivas[d][e+1].idx = 2;
                                mortas[d][e+1].idx = 0;
                                cenario[d][e+1] = '+';
                                cenario[d][e] = ' '; 

                                vivas[d][e+1].col1 = mortas[d][e+1].col1;
                                vivas[d][e+1].col2 = mortas[d][e+1].col2;
                                vivas[d][e+1].tipo = mortas[d][e+1].tipo;
                            }
                            achou = 1;
                        }
                        if(mortas[d][e-1].idx == 1 && achou == 0 && vivas[d][e-1].idx == 0){ // se esta no oeste
                            decisao = decideSePega(d, e-1, mortas[d][e-1].tipo);
                            if(decisao == 1){
                                vivas[d][e].idx = 0;
                                vivas[d][e-1].idx = 2;
                                mortas[d][e-1].idx = 0;
                                cenario[d][e-1] = '+';
                                cenario[d][e] = ' '; 

                                vivas[d][e-1].col1 = mortas[d][e-1].col1;
                                vivas[d][e-1].col2 = mortas[d][e-1].col2;
                                vivas[d][e-1].tipo = mortas[d][e-1].tipo;                    
                            }
                            achou = 1;
                        }
                    }
                }
            }

            else if(vivas[i][j].idx == 2){ // se a formiga em i j estiver carregando
                // anda para uma casa aleatoria
                movimento = rand() % 100;

                if(i != 0 && movimento < 25 && vivas[i-1][j].idx != 1 && vivas[i-1][j].idx != 2 && mortas[i-1][j].idx != 1){ // anda para o norte caso esteja livre
                    vivas[i-1][j].idx = 2;
                    vivas[i][j].idx = 0;
                    cenario[i][j] = ' ';
                    cenario[i-1][j] = '+'; // + é formiga carregada                    
                    d = i-1;
                    e = j;   

                    // transfere as caracteristicas
                    vivas[i-1][j].col1 = vivas[i][j].col1;
                    vivas[i-1][j].col2 = vivas[i][j].col2;
                    vivas[i-1][j].tipo = vivas[i][j].tipo;

                    andou = 1;    
                }
                else if(i != TAM-1 && (movimento >= 25 && movimento < 50) && vivas[i+1][j].idx != 1 && vivas[i+1][j].idx != 2 && mortas[i+1][j].idx != 1){ // anda para o sul
                    vivas[i+1][j].idx = 2;
                    vivas[i][j].idx = 0;
                    cenario[i][j] = ' ';
                    cenario[i+1][j] = '+';                    
                    d = i+1;
                    e = j;

                    vivas[i+1][j].col1 = vivas[i][j].col1;
                    vivas[i+1][j].col2 = vivas[i][j].col2;
                    vivas[i+1][j].tipo = vivas[i][j].tipo;

                    andou = 1;
                }
                else if(j != TAM-1 && (movimento >= 50 && movimento < 75) && vivas[i][j+1].idx != 1 && vivas[i][j+1].idx != 2 && mortas[i][j+1].idx != 1){ // anda para leste
                    vivas[i][j+1].idx = 2;
                    vivas[i][j].idx = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j+1] = '+';
                    d = i;
                    e = j+1;

                    vivas[i][j+1].col1 = vivas[i][j].col1;
                    vivas[i][j+1].col2 = vivas[i][j].col2;
                    vivas[i][j+1].tipo = vivas[i][j].tipo;

                    andou = 1;
                }
                else if(j != 0 && movimento >= 75 && vivas[i][j-1].idx != 1 && vivas[i][j-1].idx != 2 && mortas[i][j-1].idx != 1){ // anda para oeste
                    vivas[i][j-1].idx = 2;
                    vivas[i][j].idx = 0;
                    cenario[i][j] = ' ';
                    cenario[i][j-1] = '+';
                    d = i;
                    e = j-1;

                    vivas[i][j-1].col1 = vivas[i][j].col1;
                    vivas[i][j-1].col2 = vivas[i][j].col2;
                    vivas[i][j-1].tipo = vivas[i][j].tipo;  

                    andou = 1;              
                }

                // procura formigas mortas nas celulas adjacentes e se encontrar, chama decideSeLarga()
                if(andou == 1){
                    if(d > 0 && d < TAM-1 && e < TAM-1 && e > 0){ // condicao para nao sair da matriz
                        if(mortas[d-1][e].idx == 1 && achou == 0 && vivas[d-1][e].idx == 0){ // se esta no norte
                            decisao = decideSeLarga(d-1, e, mortas[d-1][e].tipo);
                            if(decisao == 1 && mortas[d][e].idx == 0){
                                vivas[d][e].idx = 1; // formiga passou a estar vazia
                                mortas[d][e].idx = 1; // larga a morta onde esta
                                    
                                if(vivas[d][e].tipo == 1) // largou uma morta do tipo 1                                   
                                    cenario[d][e] = '1';
                                else if(vivas[d][e].tipo == 2)
                                    cenario[d][e] = '2';
                                else if(vivas[d][e].tipo == 3)
                                    cenario[d][e] = '3';
                                else if(vivas[d][e].tipo == 4)
                                    cenario[d][e] = '4';

                                mortas[d][e].col1 = vivas[d][e].col1;
                                mortas[d][e].col2 = vivas[d][e].col2;
                                mortas[d][e].tipo = vivas[d][e].tipo;
                            }
                            achou = 1;
                        }                  
                        if(mortas[d+1][e].idx == 1 && achou == 0 && vivas[d+1][e].idx == 0){ // se esta no sul
                            decisao = decideSeLarga(d+1, e, mortas[d+1][e].tipo);
                            if(decisao == 1 && mortas[d][e].idx == 0){
                                vivas[d][e].idx = 1;
                                mortas[d][e].idx = 1;

                                if(vivas[d][e].tipo == 1)                                    
                                    cenario[d][e] = '1';
                                else if(vivas[d][e].tipo == 2)
                                    cenario[d][e] = '2';
                                else if(vivas[d][e].tipo == 3)
                                    cenario[d][e] = '3';
                                else if(vivas[d][e].tipo == 4)
                                    cenario[d][e] = '4';

                                mortas[d][e].col1 = vivas[d][e].col1;
                                mortas[d][e].col2 = vivas[d][e].col2;
                                mortas[d][e].tipo = vivas[d][e].tipo;
                            }
                            achou = 1;
                        }
                        if(mortas[d][e+1].idx == 1 && achou == 0 && vivas[d][e+1].idx == 0){ // se esta no leste
                            decisao = decideSeLarga(d, e+1, mortas[d][e+1].tipo);
                            if(decisao == 1 && mortas[d][e].idx == 0){
                                vivas[d][e].idx = 1;
                                mortas[d][e].idx = 1;

                                if(vivas[d][e].tipo == 1)                                    
                                    cenario[d][e] = '1';
                                else if(vivas[d][e].tipo == 2)
                                    cenario[d][e] = '2';
                                else if(vivas[d][e].tipo == 3)
                                    cenario[d][e] = '3';
                                else if(vivas[d][e].tipo == 4)
                                    cenario[d][e] = '4';

                                mortas[d][e].col1 = vivas[d][e].col1;
                                mortas[d][e].col2 = vivas[d][e].col2;
                                mortas[d][e].tipo = vivas[d][e].tipo;
                            }
                            achou = 1;
                        }
                        if(mortas[d][e-1].idx == 1 && achou == 0 && vivas[d][e-1].idx == 0){ // se esta no oeste
                            decisao = decideSeLarga(d, e-1, mortas[d][e-1].tipo);
                            if(decisao == 1 && mortas[d][e].idx == 0){
                                vivas[d][e].idx = 1;
                                mortas[d][e].idx = 1;

                                if(vivas[d][e].tipo == 1)                                    
                                    cenario[d][e] = '1';
                                else if(vivas[d][e].tipo == 2)
                                    cenario[d][e] = '2';
                                else if(vivas[d][e].tipo == 3)
                                    cenario[d][e] = '3';
                                else if(vivas[d][e].tipo == 4)
                                    cenario[d][e] = '4';

                                mortas[d][e].col1 = vivas[d][e].col1;
                                mortas[d][e].col2 = vivas[d][e].col2;
                                mortas[d][e].tipo = vivas[d][e].tipo;
                            }
                            achou = 1;
                        }
                    }
                }   
            }
        }
    }
}


// Funcoes de decisao
int decideSePega(int i, int j, int tipo){
    int cont = 0, a, b, area; // contagem de formigas agrupadas
    int r = RAIO, sum = 0;
    double chance, similaridade, distancia, somatorio = 0.0;    
    int rvc; // raio vertical para cima
    int rvb; // raio vertical para baixo
    int rhe; // raio horizontal para esquerda
    int rhd; // raio horizontal para direita

    rvc = r; rvb = r; rhe = r; rhd = r; area = 0;

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
            if(mortas[a][b].idx == 0)
                cont++; // qtd de espaços vazios arredores
            else if(mortas[a][b].idx == 1 && (a != i && b != j)){ // se nao for a formiga sendo analisada
                //calculo do d(i,j) = sqrt((i1 - j1)²+(i2 - j2)²) -> i(1,2) e j(1,2)
                distancia = sqrt(((mortas[i][j].col1 - mortas[a][b].col1)*(mortas[i][j].col1 - mortas[a][b].col1))+((mortas[i][j].col2 - mortas[a][b].col2)*(mortas[i][j].col2 - mortas[a][b].col2)));         
                somatorio += (1.0 - (distancia / ALFA));
            }
            area++;     // qtd total de espaços, vazios e cheios
        }
    }

    //f(i) = 1/area * sum(1 - d(i,j)/ALFA)
    similaridade = (double)(1.0/area) * somatorio;

    if(similaridade < 0)
        similaridade = 0;

    // CALCULO DECISAO
    // chance = (K1 / (K1+similaridade))²
    chance = (K1/(K1 + similaridade)) * (K1/(K1  + similaridade));

    chance = chance * 100;

    if(chance == 100)
        chance -= 5; // 95%
    else if(chance == 0)
        chance += 5; // 5%

    if(((int)chance) >= rand() % 100)
        return 1; // pega
    else
        return 0; // nao pega
}

int decideSeLarga(int i, int j, int tipo){
    int cont = 0, a, b, area; // contagem de formigas agrupadas
    int r = RAIO, sum = 0;
    float chance, distancia, somatorio = 0.0, similaridade;    
    int rvc; // raio vertical para cima
    int rvb; // raio vertical para baixo
    int rhe; // raio horizontal para esquerda
    int rhd; // raio horizontal para direita

    rvc = r; rvb = r; rhe = r; rhd = r; area = 0;

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
            if(mortas[a][b].idx == 1)
                cont++;
            if(mortas[a][b].idx == 1 && (a != i && b != j)){
                //calculo do d(i,j) = sqrt((i1 - j1)²+(i2 - j2)²) -> i(1,2) e j(1,2)
                distancia = sqrt(((mortas[i][j].col1 - mortas[a][b].col1)*(mortas[i][j].col1 - mortas[a][b].col1))+((mortas[i][j].col2 - mortas[a][b].col2)*(mortas[i][j].col2 - mortas[a][b].col2)));                
                somatorio += (1.0 - (distancia / ALFA)); 
            }
            area++;        
        }    
    }

    //f(i) = 1/area * sum(1 - d(i,j)/ALFA)
    similaridade = (double)(1/area) * somatorio;

    if(similaridade < 0)
        similaridade = 0;

    // CALCULO DECISAO
    //chance = (similaridade / (K2 + similaridade))²
    chance = (similaridade / (K2 + similaridade)) * (similaridade / (K2 + similaridade));

    chance = chance * 100; // ou 1000

    if(chance == 100.0)
        chance -= 2.0; // 98%
    else if(chance == 0)
        chance += 2.0; // 2%

    if(((int)chance) >= rand() % 100)
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


