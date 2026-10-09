#include "budget.h"

int main(void) {
    int choice;

    do {
        printf("\n===== MyPocket =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Budget\n");
        printf("4. Expense\n");
        printf("5. Event\n");
        printf("6. Funding\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: manageBudget(); break;
            case 4: addExpense(); break;
            case 5: createEvent(); break;
            case 6: createFunding(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}