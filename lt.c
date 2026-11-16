#include <stdio.h> 

int main () {

    int num = 0; 

    printf("Input num: "); 
    scanf("%d", &num); 

    while (num >= 10) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        num = sum;
    }


    printf("Output: %d", num);



    return 0;
}