#include <stdio.h>

int main () {

int arrayA[3] = {1, 2, 3};
int arrayB[3][3] = {{1, 2 ,3}, 
{4, 5, 6},
{7, 8, 9}
};
int sum[3];

int row = 3, col = 3;



printf("Array A: \n");
for(int i = 0; i < 3; i++){
    printf("%d, ", arrayA[i]); 
}

printf("\n\nArray B: \n");
for(int i = 0; i < row; i++) {
    for(int j = 0; j < col; j++){
        printf("%d ", arrayB[i][j]);
    }
    printf("\n");
}

printf("\nArray C: \n");
for(int i = 0; i < row; i++) {
    sum[i] = 0;
    for(int j = 0; j < col; j++){
        sum[i] += arrayA[j] * arrayB[i][j];
    }
     printf("%d ", sum[i]);
}


    return 0;
}