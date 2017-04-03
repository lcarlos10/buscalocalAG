#include <stdio.h>
#include <stdlib.h>

#define MAX 5

void bsort(int *v[], int qtd);

int main(){
    int i;
    //char v[MAX] = {'v', 'f', 's', 'z', 'a'};
    int v[MAX] = {10,56,20,5,1};
    bsort(v, MAX);
    for(i = 0; i < MAX; i++)
          printf("v[%d]=%c\n", i, v[i]);
    printf("\n");
    system("pause");
    return 0;
}

void bsort(int *v[], int qtd){
     int i, j;
     int k;
     for(i = 0; i < qtd - 1; i++){
           for(j = 0; j < qtd - (i +1); j++){
                 if(v[j] > v[j + 1]){
                         k = v[j];
                         v[j] = v[j + 1];
                         v[j + 1] = k;
                 }
           }
     }
}
