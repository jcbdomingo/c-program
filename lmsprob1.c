#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 3

// INNER STRUCTURE
typedef struct PRODUCT {
    char prod_code[9];
    char prod_desc[30];
    float unit_price;
    int quantity;
} PRODUCT;

// OUTER STRUCTURE
typedef struct CUSTOMER {
    char customerName[30];
    char customerAddress[30];
    PRODUCT prod[MAX];
} CUSTOMER;

// Function Prototypes
void RECORD(CUSTOMER *c);
void KOMPUTE ( CUSTOMER c);

int main() {
    CUSTOMER customer;

    RECORD(&customer);
    KOMPUTE(customer);

    return 0;
}

// FUNCTION TO ACCEPT AND VALIDATE INPUTS
void RECORD( CUSTOMER *c) {
    int i, valid;
    char temp[100];

    // CUSTOMER NAME
    do {
        valid = 1;

        printf("Enter Customer Name: ");
        fgets(c->customerName, sizeof(c->customerName), stdin);
        c->customerName[strcspn(c->customerName, "\n")] = '\0';

        if(strlen(c->customerName) == 0) {
            valid = 0;
        }

        for(i = 0; c->customerName[i] != '\0'; i++) {
            if(!isalpha(c->customerName[i]) && c->customerName[i] != ' ') {
                valid = 0;
                break;
            }
        }

        if(!valid) {
            printf("Invalid input! Name should contain letters only.\n\n");
        }

    } while(!valid);

    // CUSTOMER ADDRESS
    do {
        printf("Enter Customer Address: ");
        fgets(c->customerAddress, sizeof(c->customerAddress), stdin);
        c->customerAddress[strcspn(c->customerAddress, "\n")] = '\0';

        if(strlen(c->customerAddress) == 0) {
            printf("Invalid input! Address cannot be empty.\n\n");
        }

    } while(strlen(c->customerAddress) == 0);

    // PRODUCT INPUTS
    for(i = 0; i < MAX; i++) {

        printf("\nPRODUCT #%d\n", i + 1);

        // PRODUCT CODE
        do {
            printf("\tEnter product code: ");
            fgets(c->prod[i].prod_code, sizeof(c->prod[i].prod_code), stdin);
            c->prod[i].prod_code[strcspn(c->prod[i].prod_code, "\n")] = '\0';

            if(strlen(c->prod[i].prod_code) == 0) {
                printf("\tInvalid input! Product code cannot be empty.\n");
            }

        } while(strlen(c->prod[i].prod_code) == 0);

        // PRODUCT DESCRIPTION
        do {
            printf("\tEnter product description: ");
            fgets(c->prod[i].prod_desc, sizeof(c->prod[i].prod_desc), stdin);
            c->prod[i].prod_desc[strcspn(c->prod[i].prod_desc, "\n")] = '\0';

            if(strlen(c->prod[i].prod_desc) == 0) {
                printf("\tInvalid input! Description cannot be empty.\n");
            }

        } while(strlen(c->prod[i].prod_desc) == 0);

        // UNIT PRICE
        do {
            printf("\tEnter unit price: ");

            if(fgets(temp, sizeof(temp), stdin) != NULL &&
               sscanf(temp, "%f", &c->prod[i].unit_price) == 1 &&
               c->prod[i].unit_price > 0) {

                valid = 1;

            } else {
                valid = 0;
                printf("\tInvalid input! Enter a valid positive price.\n");
            }

        } while(!valid);

        // QUANTITY
        do {
            printf("\tEnter number quantity: ");

            if(fgets(temp, sizeof(temp), stdin) != NULL &&
               sscanf(temp, "%d", &c->prod[i].quantity) == 1 &&
               c->prod[i].quantity > 0) {

                valid = 1;

            } else {
                valid = 0;
                printf("\tInvalid input! Enter a valid positive quantity.\n");
            }

        } while(!valid);
    }
}

// FUNCTION TO COMPUTE AND DISPLAY RECEIPT
void KOMPUTE( CUSTOMER c) {

    int i;
    float amount, total = 0;

    printf("\n\n==============================================");
    printf("\n         ABC HARDWARE COMPANY");
    printf("\n==============================================\n");

    printf("\nReceipt for Customer: %s", c.customerName);
    printf("\nCustomer Address: %s\n", c.customerAddress);

    printf("\n--------------------------------------------------------------------------------------------");
    printf("\nProduct Code\tDescription\t\tUnit Price\tQuantity\tAmount");
    printf("\n--------------------------------------------------------------------------------------------");

    for(i = 0; i < MAX; i++) {

        amount = c.prod[i].unit_price * c.prod[i].quantity;
        total += amount;

        printf("\n%-15s %-25s %10.2f %10d %15.2f",
               c.prod[i].prod_code,
               c.prod[i].prod_desc,
               c.prod[i].unit_price,
               c.prod[i].quantity,
               amount);
    }

    printf("\n--------------------------------------------------------------------------------------------");
    printf("\n\t\t\t\t\t\tTotal Billing : Php %.2f\n", total);
}