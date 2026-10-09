#include "budget.h"

int main(void)
{
    int choice;

    printf("\n");
    printf("=====================================\n");
    printf("       MYPOCKET - STUDENT FINANCE\n");
    printf("=====================================\n");

    while (1)
    {
        printf("\n");
        printf("---------- MAIN MENU ----------\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("-------------------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                register_user();
                break;

            case 2:
{
    char username[30];
    User current_user;

    printf("Enter username: ");
    scanf("%29s", username);

    if (login_user(username))
    {
        load_user(username, &current_user);

        printf("\nLogin successful!\n");

        show_dashboard(&current_user);
    }
    else
    {
        printf("\nLogin failed.\n");
    }

    break;
}
            case 3:
                printf("\nThank you for using MyPocket!\n");
                return 0;

            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}