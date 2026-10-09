#include "budget.h"

void addExpense(void) {
    printf("Expense module\n");
}


#include <stdio.h>

/* যদি তোমার budget.c-তে totalBudget/totalIncome থাকে এবং extern দিয়ে দিতে পারো,
   তাহলে এখানে Remaining Balance ঠিকভাবে দেখাবে। না থাকলে শুধু Total Expense দেখাবে। */
extern float totalBudget;
extern float totalIncome;

static float totalExpense = 0.0f;

void expenseMenu(void) {
    int choice;
    float amount;

    do {
        printf("\n--- Expense & Transaction Management ---\n");
        printf("1. Add Expense\n");
        printf("2. Show Total Expense\n");
        printf("3. Show Remaining Balance (Budget - Income - Expense)\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter expense amount: ");
                if (scanf("%f", &amount) != 1) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    printf("Invalid amount. Try again.\n");
                    break;
                }
                if (amount < 0) {
                    printf("Expense amount cannot be negative.\n");
                    break;
                }
                totalExpense += amount;
                printf("Expense added successfully!\n");
                break;

            case 2:
                printf("\nTotal Expense: %.2f\n", totalExpense);
                break;

            case 3:
                /* যদি budget/income set করা থাকে তাহলে দেখাবে */
                if (totalBudget == 0.0f && totalIncome == 0.0f) {
                    printf("\nRemaining Balance calculation needs Budget and Income.\n");
                    printf("Please set Budget and add Income first (Budget menu).\n");
                    printf("Current Total Expense: %.2f\n", totalExpense);
                } else {
                    printf("\nRemaining Balance: %.2f\n",
                           (totalBudget + totalIncome - totalExpense));
                }
                break;

            case 0:
                printf("Returning to Main Menu...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);
}
