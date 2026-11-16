# include <stdio.h>

int main() {

    int arr[3][3] = {
        {1, 2, 3},
        {10, 6, 7},
        {-4, -5, -10}
    };


int sum1 = 0;
int sum2 = 0; 

for (int i = 0; i < 3; i++) {
   sum1 += arr[i][i];
   sum2 += arr[i][2 - i];
}

printf("Sum of the main diagonal: %d\n", sum1);
printf("Sum of the anti-diagonal: %d\n", sum2); 

    return 0;
}