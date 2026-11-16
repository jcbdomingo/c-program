#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_BORROWERS 5
#define MAX_BOOKS 5
#define ALLOWED_DAYS 7
#define FINE_PER_DAY 5.00

struct Date { int day, month, year; };

struct Book {
    char title[50], author[50], serialNo[20];
    int volume;
    struct Date borrowDate, returnDate;
};

struct Borrower {
    char name[50];
    int bookCount;
    struct Book books[MAX_BOOKS];
};

int isValidName(char str[]) {
    for (int i = 0; str[i]; i++)
        if (!(isalpha(str[i]) || str[i] == ' ')) return 0;
    return 1;
}

int isValidDate(int d, int m, int y) {
    return (y >= 2000 && y <= 2099) && (m >= 1 && m <= 12) && (d >= 1 && d <= 31);
}

int getDays(struct Date d) {
    int mo[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int total = d.year * 365;
    for (int i = 1; i < d.month; i++) total += mo[i];
    return total + d.day;
}

int computeDays(struct Date b, struct Date r) { return getDays(r) - getDays(b); } 

void clearBuffer() {
    while (getchar() != '\n');
}

int main() {
    struct Borrower borrowers[MAX_BORROWERS];
    int borrowerCount, totalBooks = 0, i, j, days;
    float fine;

    do {
        printf("Enter number of borrowers: ");
        if (scanf("%d", &borrowerCount) != 1) {
            printf("Invalid input! Integer Only\n");
            clearBuffer();
            continue;
        }
        if (borrowerCount <= 0 || borrowerCount > MAX_BORROWERS)
            printf("Invalid number! Max %d borrowers only.\n", MAX_BORROWERS);
        clearBuffer();
    } while (borrowerCount <= 0 || borrowerCount > MAX_BORROWERS);

    for (i = 0; i < borrowerCount; i++) {
        printf("\n========== BORROWER %d ==========\n", i + 1);

        do {
            printf("Borrower Name: ");
            scanf(" %[^\n]", borrowers[i].name);
            if (!isValidName(borrowers[i].name)) printf("Invalid name! Letters only.\n");
        } while (!isValidName(borrowers[i].name));

        do {
            printf("Number of books from Borrower %d: ", i + 1);
            if (scanf("%d", &borrowers[i].bookCount) != 1) {
                printf("Invalid input! 1-5 ONLY\n");
                clearBuffer();
                continue;
            }
            if (borrowers[i].bookCount <= 0 || borrowers[i].bookCount > MAX_BOOKS)
                printf("Invalid number of books!1-5 ONLY\n");
            clearBuffer();
        } while (borrowers[i].bookCount <= 0 || borrowers[i].bookCount > MAX_BOOKS);

        totalBooks += borrowers[i].bookCount;

        for (j = 0; j < borrowers[i].bookCount; j++) {

            printf("\n----- BOOK %d -----\n", j + 1);

            printf("Book Title: ");
            scanf(" %[^\n]", borrowers[i].books[j].title);

           do {
                printf("Volume: ");
                if (scanf("%d", &borrowers[i].books[j].volume) != 1) {
                    printf("Invalid volume!\n");
                    clearBuffer();
                    continue;
                }
                if (borrowers[i].books[j].volume <= 0) printf("Invalid volume!\n");
                clearBuffer();
            } while (borrowers[i].books[j].volume <= 0);

            do {
                printf("Author(s): ");
                scanf(" %[^\n]", borrowers[i].books[j].author);
                if (!isValidName(borrowers[i].books[j].author)) printf("Invalid author name!\n");
            } while (!isValidName(borrowers[i].books[j].author));

            printf("Serial Number: ");
            scanf(" %[^\n]", borrowers[i].books[j].serialNo);


           do {
                printf("Date Borrowed (dd mm yyyy): ");
                if (scanf("%d %d %d",
                    &borrowers[i].books[j].borrowDate.day,
                    &borrowers[i].books[j].borrowDate.month,
                    &borrowers[i].books[j].borrowDate.year) != 3) {
                    printf("Invalid date!\n");
                    clearBuffer();
                    continue;
                }
                if (!isValidDate(
                    borrowers[i].books[j].borrowDate.day,
                    borrowers[i].books[j].borrowDate.month,
                    borrowers[i].books[j].borrowDate.year))
                    printf("Invalid date!\n");
                clearBuffer();
            } while (!isValidDate(
                    borrowers[i].books[j].borrowDate.day,
                    borrowers[i].books[j].borrowDate.month,
                    borrowers[i].books[j].borrowDate.year));

            do {
                printf("Date Returned (dd mm yyyy): ");
                if (scanf("%d %d %d",
                    &borrowers[i].books[j].returnDate.day,
                    &borrowers[i].books[j].returnDate.month,
                    &borrowers[i].books[j].returnDate.year) != 3) {
                    printf("Invalid date!\n");
                    clearBuffer();
                    continue;
                }
                if (!isValidDate(
                    borrowers[i].books[j].returnDate.day,
                    borrowers[i].books[j].returnDate.month,
                    borrowers[i].books[j].returnDate.year))
                    printf("Invalid date!\n");
                clearBuffer();
            } while (!isValidDate(
                    borrowers[i].books[j].returnDate.day,
                    borrowers[i].books[j].returnDate.month,
                    borrowers[i].books[j].returnDate.year));
        }
    }

    printf("\n\n");
    printf("====================================================================================================================================\n");
    printf("                                                        ABC LIBRARY WEEKLY REPORT\n");
    printf("====================================================================================================================================\n");
    printf("Total Borrowers      : %d\n", borrowerCount);
    printf("Total Books Borrowed : %d\n", totalBooks);
    printf("====================================================================================================================================\n");
    printf("| %-20s | %-22s | %-20s | %-12s | %-13s | %-13s | %-10s |\n",
           "BORROWER NAME", "BOOK TITLE - VOL", "BOOK AUTHOR(S)", "SERIAL NO", "DATE BORROWED", "DATE RETURNED", "FINE");
    printf("====================================================================================================================================\n");

    for (i = 0; i < borrowerCount; i++) {
        for (j = 0; j < borrowers[i].bookCount; j++) {

            char titleVol[60];
            sprintf(titleVol, "%s - %d",
                borrowers[i].books[j].title,
                borrowers[i].books[j].volume);

            char dateBorrow[15];
            sprintf(dateBorrow, "%02d/%02d/%04d",
                borrowers[i].books[j].borrowDate.day,
                borrowers[i].books[j].borrowDate.month,
                borrowers[i].books[j].borrowDate.year);

            char dateReturn[15];
            sprintf(dateReturn, "%02d/%02d/%04d",
                borrowers[i].books[j].returnDate.day,
                borrowers[i].books[j].returnDate.month,
                borrowers[i].books[j].returnDate.year);

            days = computeDays(borrowers[i].books[j].borrowDate, borrowers[i].books[j].returnDate);
            fine = (days > ALLOWED_DAYS) ? (days - ALLOWED_DAYS) * FINE_PER_DAY : 0;

            printf("| %-20s | %-22s | %-20s | %-12s | %-13s | %-13s | PHP %-5.2f |\n",
                   borrowers[i].name,
                   titleVol,
                   borrowers[i].books[j].author,
                   borrowers[i].books[j].serialNo,
                   dateBorrow,
                   dateReturn,
                   fine);
        }
    }

    printf("====================================================================================================================================\n");
    printf("\nEnd of Report.\n");

    return 0;
}