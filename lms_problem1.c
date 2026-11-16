#include <stdio.h> 

int merge(int arrA[], int arrB[], int arrc[], int k) {
    int dup;

    /* add unique values from arrA */
    for (int i = 0; i < 5; i++) {
        dup = 0;
        for (int j = 0; j < k; j++) {
            if (arrA[i] == arrc[j]) { dup = 1; break; }
        }
        if (!dup) arrc[k++] = arrA[i];
    }

    /* add unique values from arrB */
    for (int i = 0; i < 5; i++) {
        dup = 0;
        for (int j = 0; j < k; j++) {
            if (arrB[i] == arrc[j]) { dup = 1; break; }
        }
        if (!dup) arrc[k++] = arrB[i];
    }

    return k;

}
int main () {
    int arrA[] = {1, 2, 3, 4, 5}, arrB[] = {4, 5, 6, 7, 8}, arrc[10];
    int k= 0;

printf("\nArray A: ");
for(int i = 0; i < 5; i++) {
     printf("%d ", arrA[i]);
} 

printf("\nArray B: ");
for(int i = 0; i < 5; i++) {
     printf("%d ", arrB[i]);
} 
  
k = merge(arrA, arrB, arrc, k); 



printf("\n\nMerge Array (Unqiue Values): ");
for (int i = 0; i < k; i++) {
    printf("%d ", arrc[i]);
}


   return 0;
}
