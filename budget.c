#include "budget.h"
/* Budget */
void show_dashboard(User *user);
void user_menu(User *user);

void show_dashboard(User *user)
{
    printf("\n");
    printf("=====================================\n");
    printf("             DASHBOARD\n");
    printf("=====================================\n");

    printf("Balance          : %.2f\n", user->balance);
    printf("Monthly Income   : %.2f\n", user->monthly_income);
    printf("Monthly Budget   : %.2f\n", user->monthly_budget);
    printf("Savings Goal     : %.2f\n", user->savings_goal);

    printf("\n");
    printf("Budget Used      : 0.00%%\n");
    printf("Alerts           : None\n");
    printf("Upcoming Expense : None\n");
    printf("Pending Dues     : 0.00\n");

    printf("=====================================\n");
}

void user_menu(User *user)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("=====================================\n");
        printf("          MYPOCKET FINANCE MENU\n");
        printf("=====================================\n");
        printf("1. Manage Income\n");
        printf("2. Manage Budget\n");
        printf("3. Add Expense\n");
        printf("4. View Transactions\n");
        printf("5. Financial Summary\n");
        printf("6. Logout\n");
        printf("=====================================\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("\nIncome module coming soon...\n");
                break;

            case 2:
                printf("\nBudget module coming soon...\n");
                break;

            case 3:
                printf("\nExpense module coming soon...\n");
                break;

            case 4:
                printf("\nTransaction module coming soon...\n");
                break;

            case 5:
                printf("\nFinancial summary coming soon...\n");
                break;

            case 6:
                printf("\nLogging out...\n");
                return;

            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
}