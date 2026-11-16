#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void inputScores(float *scores, int n);
float computeSum(float *scores, int n);
float findMax(float *scores, int n);
float findMin(float *scores, int n);
void countPassFail(float *scores, int n, int *pass, int *fail);
void sortScores(float *scores, int n);
void assignRanks(float *scores, int n, int *ranks);
void displayReport(float *scores, int *ranks, int n);

int main() {
    int n;
    
    printf("Enter number of students: ");
    scanf("%d", &n);

    // Dynamic memory allocation
    float *scores = (float *)malloc(n * sizeof(float));
    int *ranks = (int *)malloc(n * sizeof(int));

    if (scores == NULL || ranks == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Input
    inputScores(scores, n);

    // Calculations
    float sum = computeSum(scores, n);
    float avg = sum / n;
    float max = findMax(scores, n);
    float min = findMin(scores, n);

    int pass = 0, fail = 0;
    countPassFail(scores, n, &pass, &fail);

    // Assign ranks (based on original scores)
    assignRanks(scores, n, ranks);

    // Output summary
    printf("\nMath Exam Report:\n");
    printf("Total Sum of Scores: %.2f\n", sum);
    printf("Average Score: %.2f\n", avg);
    printf("Highest Score: %.2f\n", max);
    printf("Lowest Score: %.2f\n", min);
    printf("Number of Students Passed: %d\n", pass);
    printf("Number of Students Failed: %d\n", fail);

    // Display report cards
    displayReport(scores, ranks, n);

    // Free memory
    free(scores);
    free(ranks);

    return 0;
}

// Function to input scores
void inputScores(float *scores, int n) {
    for (int i = 0; i < n; i++) {
        printf("Enter score of student %d: ", i + 1);
        scanf("%f", (scores + i)); // pointer usage
    }
}

// Compute total sum
float computeSum(float *scores, int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += *(scores + i);
    }
    return sum;
}

// Find maximum score
float findMax(float *scores, int n) {
    float max = *scores;
    for (int i = 1; i < n; i++) {
        if (*(scores + i) > max)
            max = *(scores + i);
    }
    return max;
}

// Find minimum score
float findMin(float *scores, int n) {
    float min = *scores;
    for (int i = 1; i < n; i++) {
        if (*(scores + i) < min)
            min = *(scores + i);
    }
    return min;
}

// Count pass and fail
void countPassFail(float *scores, int n, int *pass, int *fail) {
    for (int i = 0; i < n; i++) {
        if (*(scores + i) >= 50)
            (*pass)++;
        else
            (*fail)++;
    }
}

// Sort scores (ascending) using bubble sort
void sortScores(float *scores, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (*(scores + j) > *(scores + j + 1)) {
                float temp = *(scores + j);
                *(scores + j) = *(scores + j + 1);
                *(scores + j + 1) = temp;
            }
        }
    }
}

// Assign ranks (higher score = better rank)
void assignRanks(float *scores, int n, int *ranks) {
    for (int i = 0; i < n; i++) {
        int rank = 1;
        for (int j = 0; j < n; j++) {
            if (*(scores + j) > *(scores + i)) {
                rank++;
            }
        }
        *(ranks + i) = rank;
    }
}

// Display report cards
void displayReport(float *scores, int *ranks, int n) {
    printf("\nStudent Report Cards:\n");
    for (int i = 0; i < n; i++) {
        char remark[25];

        if (*(ranks + i) == 1)
            sprintf(remark, "Excellent");
        else if (*(ranks + i) == 2 || *(ranks + i) == 3)
            sprintf(remark, "Good");
        else
            sprintf(remark, "Needs Improvement");

        printf("Student %d: Score = %.2f, Rank = %d, Remark = %s\n",
               i + 1, *(scores + i), *(ranks + i), remark);
    }
}