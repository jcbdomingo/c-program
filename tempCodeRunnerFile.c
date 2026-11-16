#include <stdio.h>
#include <string.h>

#define MAX_CLIENT 5

typedef struct
{
    char name[50];
    char id[10];
    int age;
    float bill;
} patientrecord;

int main()
{

    patientrecord client[MAX_CLIENT];
    int count = 0;
    int choice;
    int found;
    int highest;
    char search_id[10];

    do
    {
        printf("\n====== HOSPITAL SYSTEM RECORD ======\n");
        printf("1. Create patient record\n");
        printf("2. Display patient record\n");
        printf("3. Update Patient record\n");
        printf("4. Delete Patient record\n");
        printf("5. Display patient with highest bill amount\n");
        printf("6. Exit\n");

        printf("\n\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (count >= MAX_CLIENT)
            {
                printf("\nUp to 5 PATIENT ONLY!\n");
                break;
            }
            printf("\nEnter patient ID: ");
            scanf("%s", client[count].id);
            getchar();
            printf("Enter patient Name: ");
            fgets(client[count].name, sizeof(client[count].name), stdin);
            client[count].name[strcspn(client[count].name, "\n")] = '\0';
            printf("Enter patient Age: ");
            scanf("%d", &client[count].age);
            printf("Enter patient Bill Amount: ");
            scanf("%f", &client[count].bill);

            printf("\nPatient recorded\n");
            count++;
            break;
        case 2:
            printf("\n===== SUMMARY CLIENT REPORT =====\n");
            for (int i = 0; i < count; i++)
            {
                printf("\nPatient ID: %s\n", client[i].id);
                printf("Patient Name: %s\n", client[i].name);
                printf("Patient Bill Amount: %.2f\n", client[i].bill);
            }
            break;
        case 3:
            found = 0;
            printf("\nEnter Patient ID: ");
            scanf("%s", search_id);
            getchar();
            for (int i = 0; i < count; i++)
            {
                if (strcmp(search_id, client[i].id) == 0)
                {
                    printf("Enter new patient Name: ");
                    fgets(client[i].name, sizeof(client[i].name), stdin);
                    client[i].name[strcspn(client[i].name, "\n")] = '\0';
                    printf("Enter new patient Age: ");
                    scanf("%d", &client[i].age);
                    printf("Enter new patient Bill Amount: ");
                    scanf("%f", &client[i].bill);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                printf("\nStudent not found!");
            }
            break;
        case 4:
            found = 0;
            printf("\nEnter Patient ID to delete: ");
            scanf("%s", search_id);
            for (int i = 0; i < count; i++)
            {
                if (strcmp(search_id, client[i].id) == 0)
                {
                    for (int j = i; j < count - 1; j++)
                    {
                        client[j] = client[j + 1];
                    }
                    count--;
                    found = 1;
                    printf("\nStudent deleted successfully!");
                    break;
                }
            }
            if (!found)
            {
                printf("\nStudent not found!");
            }
            break;
        case 5:
            highest = 0;

            for (int i = 0; i < count; i++)
            {
                if (client[i].bill > client[highest].bill)
                {
                    highest = i;
                }
            }
            printf("\n\nPATIENT WITH THE HIGHEST BILL AMOUNT\n");
            printf("Patient ID: %s\n", client[highest].id);
            printf("Patient Name: %s\n", client[highest].name);
            printf("Patient Age: %d\n", client[highest].age);
            printf("Patient Bill Amount: %.2f\n", client[highest].bill);
            break;
        case 6:
            printf("\n\nTHANK YOU FOR USING THE PROGRAM!");
            return 0;
            break;
        default:
            printf("\nInvalid Choice! (1-5 ONLY)\n");
        }
    } while (choice != 6);

    return 0;
}