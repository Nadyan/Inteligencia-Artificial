#include <cstdio>

#define TAM 42  //terreno TAMxTAM

void montaCenario();

int main(){
    montaCenario();

	return 0;
}

void montaCenario(){

    int terreno[TAM][TAM];
    FILE *f = fopen("terreno.txt", "r");
    int i, j;

    for(i = 0; i < TAM; i++){
        for(j = 0; j < TAM; j++){
            terreno[i][j] = fscanf(f, "%d", &terreno[i][j]);
            printf("%d\n", terreno[i][j]);
        }
        printf("\n");
    }
}
