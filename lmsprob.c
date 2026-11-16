#include <stdio.h>

#define ROWS 3
#define COLS 4

int z[ROWS][COLS] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

int main(void) {
    int i, j;
    int minVal = z[0][0];            // start with first element

    for (i = 0; i < ROWS; ++i) {
        for (j = 0; j < COLS; ++j) {
            if (z[i][j] < minVal) {
                minVal = z[i][j];
            }
        }
    }

    printf("Minimum = %d\n", minVal);
    return 0;
}
