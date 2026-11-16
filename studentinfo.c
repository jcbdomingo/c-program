#include <stdio.h>
#include <string.h>

typedef struct studentinfo
{
    char name[50];
    int id;
    float grade;

} studentinfo;

int main()
{
    studentinfo student[5];
    int count = 0;
    int choice;
    FILE *fp;
    int search_id;
    int found;

    do
    {
        printf("======= STUDENT RECORD SYSTEM ========");
        printf("\n\n1. Add Student\n");
        printf("2. Display Student/s\n");
        printf("3. Save Student/s to File\n");
        printf("4. Load Student/s From File\n");
        printf("5. Search Student by ID\n");
        printf("6. EXIT\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        if(count >= 5) {
            printf("Student list is only up to 5 students");
            break;
        }
            printf("\nEnter ID: ");
            scanf("%d", &student[count].id);
            printf("Enter Name: ");
            getchar();
            fgets(student[count].name, sizeof(student[count].name), stdin);
            student[count].name[strcspn(student[count].name, "\n")] = '\0';
            printf("Enter Grade: ");
            scanf("%f", &student[count].grade); 

            count++;

            printf("\n\nStudent added successfully!\n\n");
            break;
        case 2:
            printf("\nID\t\tNAME\t\tGRADE\n");

            for (int i = 0; i < count; i++)
            {
                printf("%d\t\t%s\t%.2f\n",
                       student[i].id, student[i].name, student[i].grade);
            }
            break;
        case 3:
            fp = fopen("studentinfo.txt", "w");
            if (fp == NULL)
            {
                printf("\nError opening a file for writing\n");
                break;
            }
            for (int i = 0; i < count; i++)
            {
                fprintf(fp, "%d\n", student[i].id);
                fprintf(fp, "%s\n", student[i].name);
                fprintf(fp, "%.2f\n", student[i].grade);
            }
            fclose(fp);
            printf("Records saved successfully!\n");
            break;

        case 4:
            fp = fopen("studentinfo.txt", "r");
            if (fp == NULL)
            {
                printf("\nError opening a file for Reading\n");
                break;
            }
            count = 0;
            while (
                fscanf(fp, "%d\n", &student[count].id) == 1)
            {
                fgets(student[count].name,
                      sizeof(student[count].name),
                      fp);

                student[count].name[strcspn(student[count].name, "\n")] = '\0';

                fscanf(fp,
                       "%f\n",
                       &student[count].grade);

                count++;
            }
            fclose(fp);
            printf("Records loaded successfully!\n");
            break;
        case 5:
            found = 0;
            printf("Enter ID: ");
            scanf("%d", &search_id);
            for (int i = 0; i < count; i++)
            {
                if (search_id == student[i].id)
                {
                    printf("\nStudent found!\n\n");
                    printf("ID: %d", student[i].id);
                    printf("\nName: %s", student[i].name);
                    printf("\nGrade: %f\n\n", student[i].grade);
                    found = 1;
                    break;
                }
            }
            if (!found)
                printf("\n\nNot found!\n");
            break;
        case 6:
            printf("\nThank you for using the program!");
            return 0;
            break;
        default:
            printf("Invalid Choice! (1-6 ONLY)");
        }
    } while (choice != 6);

    return 0;
}