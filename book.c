#include <stdio.h>
#include <string.h>

#define MAX_BOOK 5

typedef struct
{

    char id[10];
    char author[50];
    char title[50];
    float price;

} bookinfo;

int main()
{

    bookinfo book[MAX_BOOK];
    int choice;
    int count = 0;
    int found;
    char search_id[50];
    int highest;
    FILE *fp;

    do
    {
        printf("\n\n===== BOOK RECORD SYSTEM ====\n\n");
        printf("1. Add Book\n");
        printf("2. Display book\n");
        printf("3. Search book by ID\n");
        printf("4. Display most expensive Book\n");
        printf("5. Save to file\n");
        printf("6. Load from file\n");
        printf("7. Delete Book\n");
        printf("8. Exit\n");

        printf("\nEnter Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            if (count >= MAX_BOOK)
            {
                printf("\nBOOK INPUT IS ONLY UP TO 5!\n");
                break;
            }
            printf("\nEnter Book ID: ");
            fgets(book[count].id, sizeof(book[count].id), stdin);
            book[count].id[strcspn(book[count].id, "\n")] = '\0';
            printf("Enter Book Title: ");
            fgets(book[count].title, sizeof(book[count].title), stdin);
            book[count].title[strcspn(book[count].title, "\n")] = '\0';
            printf("Enter Author: ");
            fgets(book[count].author, sizeof(book[count].author), stdin);
            book[count].author[strcspn(book[count].author, "\n")] = '\0';
            printf("Enter Price: ");
            scanf("%f", &book[count].price);
            getchar();

            printf("\nBook recorded successfully!");
            count++;
            break;
        case 2:
            printf("\n===== BOOK SUMMARY =====\n");
            for (int i = 0; i < count; i++)
            {
                printf("\nBOOK ID: %s\n", book[i].id);
                printf("BOOK TITLE: %s\n", book[i].title);
                printf("BOOK AUTHOR: %s\n", book[i].author);
                printf("BOOK PRICE: %.2f\n", book[i].price);
            }
            break;
        case 3:
            found = 0;
            printf("\nEnter Book ID to search: ");
            scanf("%s", search_id);
            getchar();
            for (int i = 0; i < count; i++)
            {
                if (strcmp(search_id, book[i].id) == 0)
                {
                    printf("\nBOOK Found!");
                    printf("\nBOOK ID: %s\n", book[i].id);
                    printf("BOOK TITLE: %s\n", book[i].title);
                    printf("BOOK AUTHOR: %s\n", book[i].author);
                    printf("BOOK PRICE: %.2f\n", book[i].price);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                printf("\nBook Not found!");
            }
            break;
        case 4:
            highest = 0;
            if (count == 0)
            {
                printf("\nNo book available\n");
                break;
            }
            for (int i = 0; i < count; i++)
            {
                if (book[i].price > book[highest].price)
                {
                    highest = i;
                }
            }
            printf("\n====== MOST EXPENSIVE BOOK ======\n");
            printf("\nBOOK ID: %s\n", book[highest].id);
            printf("BOOK TITLE: %s\n", book[highest].title);
            printf("BOOK AUTHOR: %s\n", book[highest].author);
            printf("BOOK PRICE: %.2f\n", book[highest].price);
            break;
        case 5:
            fp = fopen("bookrecord.txt", "w");
            if (fp == NULL)
            {
                printf("\nFile cannot be open for writing\n");
                break;
            }
            for (int i = 0; i < count; i++)
            {
                fprintf(fp, "%s", book[i].id);
                fprintf(fp, "%s", book[i].title);
                fprintf(fp, "%s", book[i].author);
                fprintf(fp, "%.2f", book[i].price);
            }

            fclose(fp);
            printf("\nRecoreded Successfully!\n");
            break;
        case 6:
            fp = fopen("bookrecord.txt", "r");
            if (fp == NULL)
            {
                printf("\nFile cannot be open for reading\n");
                break;
            }
            count = 0;
            while (fscanf(fp, "%f", &book[count].price) == 1)
            {
                fgets(book[count].id, sizeof(book[count].id), fp);
                book[count].id[strcspn(book[count].id, "\n")] = '\0';
                fgets(book[count].title, sizeof(book[count].title), fp);
                book[count].title[strcspn(book[count].title, "\n")] = '\0';
                fgets(book[count].author, sizeof(book[count].author), fp);
                book[count].author[strcspn(book[count].author, "\n")] = '\0';
                scanf("%f", &book[count].price);

                count++;
            }

            fclose(fp);
            printf("\nLoaded Succesfully\n");
            break;
        case 7:
            found = 0;
            printf("\nEnter Book ID to delete: ");
            scanf("%s", search_id);
            getchar();
            for (int i = 0; i < count; i++)
            {
                if (strcmp(search_id, book[i].id) == 0)
                {
                    for (int j = i; j < count - 1; j++)
                    {
                        book[j] = book[j + 1];
                    }
                }
                count--;
                found = 1;
                printf("\nBook Deleted Succesfully!\n");
                break;
            }
            if (!found)
            {
                printf("\nBook Not found!");
            }
            break;
        case 8:
            printf("\n\nThank you for using the program!");
            return 0;
            break;
        default:
            printf("\nInvalid Choice! 1-8 ONLY!\n");
        }
    } while (choice != 8);

    return 0;
}