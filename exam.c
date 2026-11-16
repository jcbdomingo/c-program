#include <stdio.h>
#include <string.h>

#define MAX_STUDENT 5

typedef struct Grades
{

    float prelims;
    float midterms;
    float finals;

} Grades;

typedef struct Student
{

    int id;
    char name[50];
    float average;
    Grades grade;

} Student;

int main()
{

    Student student[MAX_STUDENT];

    int choice = 0;
    int count = 0;
    int found;
    int search_id;
    FILE *fp;

    do
    {
        printf("\n\n======== STUDENT RECORD SYSTEM ========\n\n");
        printf("\n1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Compute Average\n");
        printf("4. Search Student\n");
        printf("5. Delete Student\n");
        printf("6. Save to file\n");
        printf("7. Load from the file\n");
        printf("8. Exit the program\n");

        printf("\n\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (count >= MAX_STUDENT)
            {
                printf("\nStudent record system is only up to 5!");
                break;
            }
            printf("\nEnter ID: ");
            scanf("%d", &student[count].id);
            getchar();
            printf("Enter name: ");
            fgets(student[count].name, sizeof(student[count].name), stdin);
            student[count].name[strcspn(student[count].name, "\n")] = '\0';
            printf("Enter prelim grade: ");
            scanf("%f", &student[count].grade.prelims);
            printf("Enter midterms grade: ");
            scanf("%f", &student[count].grade.midterms);
            printf("Enter finals grade: ");
            scanf("%f", &student[count].grade.finals);
            count++;

            printf("\nStudent added succesfully!");
            break;
        case 2:
            printf("\n==== Student record ====");
            printf("\nID\t\tNAME\tPRELIM GRADE\tMID-TERMS GRADE\tFINAL-GRADE\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d\t%s\t\t%.2f\t\t%.2f\t\t%.2f\n",
                       student[i].id, student[i].name,
                       student[i].grade.prelims,
                       student[i].grade.midterms,
                       student[i].grade.finals);
            }
            break;
        case 3:
            for (int i = 0; i < count; i++)
            {
                student[i].average =
                    (student[i].grade.prelims +
                     student[i].grade.midterms +
                     student[i].grade.finals) /
                    3;
            }
            break;
        case 4:
            found = 0;
            printf("\nEnter student ID: ");
            scanf("%d", &search_id);

            for (int i = 0; i < count; i++)
            {
                if (search_id == student[i].id)
                {
                    printf("\nStudent found!\n");
                    printf("Student ID: %d\n", student[i].id);
                    printf("Student Name: %s\n", student[i].name);
                    printf("Student Average: %.2f\n", student[i].average);
                    found = 1;
                    break;
                }
            }
            if (!found)
                printf("\nStudent not found!");
            break;
        case 5:
            printf("\nEnter ID: ");
            scanf("%d", &search_id);

            for (int i = 0; i < count; i++)
            {
                if (search_id == student[i].id)
                {
                    printf("\nStudent found!\n");
                    printf("Student ID: %d\n", student[i].id);
                    printf("Student Name: %s\n", student[i].name);
                    printf("Student Average: %.2f\n", student[i].average);
                }
                for (int j = i; j < count - 1; j++)
                {
                    student[j] = student[j + 1];
                }
                count--;
                found = 1;
                printf("\nThis student deleted succesfully!\n");
                break;
            }
            if (!found)
                printf("\nStudent not found!");

            break;
        case 6:
            fp = fopen("studentrecord.txt", "w");
            if (fp == NULL)
            {
                printf("\nCannot open the file for writing!");
                break;
            }
            for (int i = 0; i < count; i++)
            {
                fprintf(fp, "%d\n", student[i].id);
                fprintf(fp, "%s\n", student[i].name);
                fprintf(fp, "%.2f\n", student[i].grade.prelims);
                fprintf(fp, "%.2f\n", student[i].grade.midterms);
                fprintf(fp, "%.2f\n", student[i].grade.finals);
                fprintf(fp, "%.2f\n", student[i].average);
            }
            fclose(fp);
            printf("\nRecord saved successfully!\n");
            break;
        case 7:
            fp = fopen("studentrecord.txt", "r");
            if (fp == NULL)
            {
                printf("\nCannot open the file for reading!");
                break;
            }
            count = 0;
            while (fscanf(fp, "%d\n", &student[count].id) == 1)
            {
                fgets(student[count].name, sizeof(student[count].name), fp);
                student[count].name[strcspn(student[count].name, "\n")] = '\0';
                fscanf(fp, "%f\n", &student[count].grade.prelims);
                fscanf(fp, "%f\n", &student[count].grade.midterms);
                fscanf(fp, "%f\n", &student[count].grade.finals);
                fscanf(fp, "%f\n", &student[count].average);

                count++;
            }
            fclose(fp);
            printf("Student Loaded Succesfully!");
            break;
        case 8:
            printf("Thank you for using the program!");
            return 0;
            break;
        default:
            printf("Invalid Choice 1-8 ONLY!");
        }
    } while (choice != 8);
    return 0;
}