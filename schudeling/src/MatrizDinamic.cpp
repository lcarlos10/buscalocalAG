#include "MatrizDinamic.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    int **p;
    int i, j, N=2;

    p = (int **) malloc(N * sizeof(int *));

    for(i = 0; i < N; i++){
        //criar um array de int
        p[i] = (int *) malloc(N * sizeof(int));
        for(j = 0; j < N; j++)
            scanf("%d", &p[i][j]);
    }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++)
            printf("%d", &p[i][j]);
    }

    system("pause");
    return 0;
}
