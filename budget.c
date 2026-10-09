#include "budget.h"

void manageBudget(void) {
    printf("Budget module\n");
}

#include <stdio.h>

float totalBudget = 0;
float totalIncome = 0;

void budgetMenu() {
    int choice;
    float amount;

    do {
        printf("\n--- Budget & Income Management ---\n");
        printf("1. Set Budget\n");
        printf("2. Add Income\n");
        printf("3. Show Budget Summary\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter budget amount: ");
                scanf("%f", &totalBudget);
                printf("Budget set successfully!\n");
                break;

            case 2:
                printf("Enter income amount: ");
                scanf("%f", &amount);
                totalIncome += amount;
                printf("Income added successfully!\n");
                break;

            case 3:
                printf("\nCurrent Budget: %.2f\n", totalBudget);
                printf("Total Income: %.2f\n", totalIncome);
                break;

            case 0:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while(choice != 0);
}