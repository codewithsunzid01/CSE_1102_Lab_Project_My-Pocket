#include "budget.h"

void register_user(void)
{
    User user;

    printf("\n========== REGISTER ==========\n");

    printf("Enter username: ");
    scanf("%29s", user.username);

    printf("Enter password: ");
    scanf("%49s", user.password);

    printf("Enter monthly income: ");
    scanf("%f", &user.monthly_income);

    printf("Enter monthly budget: ");
    scanf("%f", &user.monthly_budget);

    printf("Enter savings goal: ");
    scanf("%f", &user.savings_goal);

    user.balance = user.monthly_income;

    save_user(&user);

    printf("\nRegistration successful!\n");
}

int login_user(const char username[])
{
    User user;

    if (load_user(username, &user))
    {
        char password[50];

        printf("Enter password: ");
        scanf("%49s", password);

        if (strcmp(password, user.password) == 0)
        {
            return 1;
        }
    }

    return 0;
}