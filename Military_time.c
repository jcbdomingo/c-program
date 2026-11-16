#include <stdio.h>


void Military_time(int time){

    if(time < 0 || time > 2400 || time % 100 >= 60){
        printf("Input Error");
        return;
    }

    
    int hrs = time / 100;
    int mins = time % 100; 

    if (time == 2400) {
        printf("12:00 AM \n");
        return; 
    }

    if (hrs == 0) {
        printf("12:%02d AM\n", mins);
    }
    else if (hrs < 12) {
        printf("%d:%02d AM\n", hrs, mins); 
    }
    else if (hrs == 12){
        printf("12:%02d PM\n", mins);
    }
    else{
        printf("%d:%02d PM\n", hrs-12, mins);
    }


}

int main(void) {

    int time = 0;
   
    printf("Enter Military Time: ");
    if(scanf("%d", &time) != 1) {
        printf("Invald input!");
        return 1;
    }

    Military_time(time);

    return 0;
}