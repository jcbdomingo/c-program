#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_MOVIES 50

/* ============================
   HIGHLIGHTS / TOPICS COVERED:
   - Searching in Arrays
   - Predefined Functions (printf, fgets, sscanf, fopen, fprintf, fscanf)
   - User-defined Functions (addMovie, displayMovies, searchMovie, saveMovies, loadMovies)
   - Fundamentals of Pointers (using pointers, arrow operator ->)
   - Call by Reference (functions that modify array/count via pointers)
   - Call by Value (printMovieByValue receives a movie by value)
   - Structure / Nested Structure (movie contains nested date struct)
   - Structure Array (movie mov[MAX_MOVIES])
   - File Handling (save/load to text file)
   ============================ */

/* nested structure for date */
typedef struct date {
    int day;
    int month;
    int year;
} date;

/* movie structure contains nested date (youinput) */
typedef struct movie {
    int ID;
    char title[50];
    float rating;
    date youinput;      /* nested structure */
} movie;

/* function prototypes (user-defined functions) */
void addMovie(movie *arr, int *count);                /* call by reference */
void displayMovies(const movie *arr, int count);     /* uses call-by-value for each printed movie */
void printMovieByValue(movie m);                      /* call by value (demonstration) */
movie *searchMovie(movie *arr, int count, int id);    /* searching in arrays, returns pointer (reference) */
void saveMovies(const movie *arr, int count);         /* file handling (write) */
void loadMovies(movie *arr, int *count);              /* file handling (read) */
void clearInput(void);                                /* predefined helper to flush stdin */

/* clear leftover input from stdin */
void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Add a movie (call by reference): modifies the array and count using pointers.
   Demonstrates use of pointers and arrow operator to access members of the element. */
void addMovie(movie *arr, int *count) {
    if (*count >= MAX_MOVIES) {
        printf("Movie list is full (max %d).\n", MAX_MOVIES);
        return;
    }

    movie *m = &arr[*count];    /* pointer to the next free slot (arrow operator below) */

    /* read date using predefined sscanf on formatted input */
    while (1) {
        char buf[64];
        printf("\nEnter date today (dd-mm-yyyy): ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        if (sscanf(buf, "%d-%d-%d", &m->youinput.day, &m->youinput.month, &m->youinput.year) == 3) break;
        printf("Invalid date format. Try again (format 03-04-2026).\n");
    }

    /* read ID */
    while (1) {
        char buf[64];
        printf("Enter Movie ID: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        if (sscanf(buf, "%d", &m->ID) == 1) break;
        printf("Invalid ID. Enter an integer.\n");
    }

    /* read title using fgets (predefined function) */
    printf("Enter Movie Title: ");
    if (fgets(m->title, sizeof(m->title), stdin) == NULL) m->title[0] = '\0';
    m->title[strcspn(m->title, "\n")] = '\0';

    /* rating validation */
    while (1) {
        char buf[64];
        printf("Enter your movie rating (1.0 - 5.0): ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        if (sscanf(buf, "%f", &m->rating) == 1) {
            if (m->rating >= 1.0f && m->rating <= 5.0f) break;
        }
        printf("Invalid rating! (1.0 to 5.0 ONLY)\n");
    }

    (*count)++; /* increment movie count (call-by-reference effect) */
    printf("Movie added successfully!\n");
}

/* Print a single movie (call by value) - demonstrates passing the whole structure by value.
   This is less efficient but illustrates the concept. */
void printMovieByValue(movie m) {
    printf("%d\t%s\t%.1f\t%d-%02d-%04d\n",
           m.ID, m.title, m.rating, m.youinput.day, m.youinput.month, m.youinput.year);
}

/* Display all movies. This uses the call-by-value print for each element to show both methods. */
void displayMovies(const movie *arr, int count) {
    if (count == 0) {
        printf("\nNo movies to display.\n");
        return;
    }
    printf("\nID\tTITLE\t\tRATING\tDATE\n");
    for (int i = 0; i < count; ++i) {
        /* demonstrate printing by value: pass arr[i] (a copy) */
        printMovieByValue(arr[i]);
    }
}

/* Search for a movie in the array by ID (Searching in Arrays).
   Returns pointer to movie (call by reference style) or NULL if not found. */
movie *searchMovie(movie *arr, int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (arr[i].ID == id) {
            return &arr[i]; /* return reference to found element (pointer) */
        }
    }
    return NULL;
}

/* Save movies to a file (File Handling) - uses predefined functions fopen/fprintf/fclose */
void saveMovies(const movie *arr, int count) {
    FILE *fp = fopen("movieinfo.txt", "w");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }
    for (int i = 0; i < count; ++i) {
        /* write ID, title, rating and date in a simple text format */
        fprintf(fp, "%d|%s|%.1f|%d-%d-%d\n",
                arr[i].ID, arr[i].title, arr[i].rating,
                arr[i].youinput.day, arr[i].youinput.month, arr[i].youinput.year);
    }
    fclose(fp);
    printf("Saved %d movie(s) to movieinfo.txt\n", count);
}

/* Load movies from file (File Handling). Modifies the array and count via pointers (call by reference). */
void loadMovies(movie *arr, int *count) {
    FILE *fp = fopen("movieinfo.txt", "r");
    if (fp == NULL) {
        perror("Error opening file for reading");
        return;
    }
    *count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL && *count < MAX_MOVIES) {
        /* parse lines of the format: ID|title|rating|dd-mm-yyyy */
        int id, d, mth, y;
        char title[50];
        float rating;
        if (sscanf(line, "%d|%49[^|]|%f|%d-%d-%d", &id, title, &rating, &d, &mth, &y) >= 6) {
            arr[*count].ID = id;
            strncpy(arr[*count].title, title, sizeof(arr[*count].title)-1);
            arr[*count].title[sizeof(arr[*count].title)-1] = '\0';
            arr[*count].rating = rating;
            arr[*count].youinput.day = d;
            arr[*count].youinput.month = mth;
            arr[*count].youinput.year = y;
            (*count)++;
        }
    }
    fclose(fp);
    printf("Loaded %d movie(s) from movieinfo.txt\n", *count);
}

/* main menu uses small user-defined functions for each case to improve modularity */
int main(void) {
    movie mov[MAX_MOVIES];
    int count = 0;
    int choice;

    do {
        printf("\n======MOVIE COLLECTION SYSTEM======\n");
        printf("1. Add Movie\n");
        printf("2. Display Movies\n");
        printf("3. Search Movie\n");
        printf("4. Save Movies\n");
        printf("5. Load Movies\n");
        printf("6. Exit\n");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice. Enter a number 1-6.\n");
            clearInput();
            continue;
        }
        clearInput();

        switch (choice) {
            case 1:
                /* user-defined function: addMovie (modifies array via pointer) */
                addMovie(mov, &count);
                break;
            case 2:
                /* displayMovies demonstrates call-by-value printing for each element */
                displayMovies(mov, count);
                break;
            case 3: {
                /* Searching in Arrays: searchMovie() returns a pointer (reference) */
                int search_ID;
                printf("Enter ID to search: ");
                if (scanf("%d", &search_ID) != 1) {
                    printf("Invalid ID.\n");
                    clearInput();
                    break;
                }
                clearInput();
                movie *found = searchMovie(mov, count, search_ID);
                if (found) {
                    printf("Movie found:\n");
                    /* demonstrate accessing via pointer with arrow operator */
                    printf("ID: %d\nTitle: %s\nRating: %.1f\nDate: %d-%02d-%04d\n",
                           found->ID, found->title, found->rating,
                           found->youinput.day, found->youinput.month, found->youinput.year);
                } else {
                    printf("Movie with ID %d not found.\n", search_ID);
                }
                break;
            }
            case 4:
                saveMovies(mov, count);
                break;
            case 5:
                loadMovies(mov, &count);
                break;
            case 6:
                printf("Thank you for using the program!\n");
                break;
            default:
                printf("Invalid Choice (1-6 Only)\n");
        }
    } while (choice != 6);

    return 0;
}