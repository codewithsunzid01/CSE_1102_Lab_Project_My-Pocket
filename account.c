/* =========================================================
   account.c  -  Balance & Budget Management
   ========================================================= */

#include "budget.h"


/* Total expense of this user in the current month */
static float currentMonthExpense(int ui)
{
    int i;
    float total = 0.0f;
    Date today = getToday();
    int uid = users[ui].userId;

    for (i = 0; i < transactionCount; i++) {
        if (transactions[i].userId != uid)
            continue;
        if (transactions[i].date.month != today.month ||
            transactions[i].date.year  != today.year)
            continue;

        if (transactions[i].type == TX_EXPENSE          ||
            transactions[i].type == TX_EVENT_PAYMENT    ||
            transactions[i].type == TX_EVENT_SETTLEMENT ||
            transactions[i].type == TX_FUND_CONTRIBUTION||
            transactions[i].type == TX_INTERNAL_TRANSFER)
        {
            total += transactions[i].amount;
        }
    }
    return total;
}


void viewBalance(int ui)
{
    float spent = currentMonthExpense(ui);
    float left  = users[ui].monthlyBudget - spent;

    printf("\n");
    lineBreak('=', 62);
    printf("   ACCOUNT SUMMARY  -  %s\n", users[ui].name);
    lineBreak('=', 62);
    printf("   Current Balance        : %10.2f Tk\n", users[ui].balance);
    printf("   Monthly Budget         : %10.2f Tk\n", users[ui].monthlyBudget);
    printf("   Spent This Month       : %10.2f Tk\n", spent);
    printf("   Budget Remaining       : %10.2f Tk\n", left);
    lineBreak('-', 62);
    printf("   Lifetime Income        : %10.2f Tk\n", users[ui].totalIncome);
    printf("   Lifetime Expense       : %10.2f Tk\n", users[ui].totalExpense);
    printf("   Net Savings            : %10.2f Tk\n",
           users[ui].totalIncome - users[ui].totalExpense);
    lineBreak('=', 62);

    showBudgetAlert(ui);
    pauseScreen();
}


void setMonthlyBudget(int ui)
{
    float b;

    printf("\n   Current monthly budget : %.2f Tk\n", users[ui].monthlyBudget);
    b = readFloat("   Enter new monthly budget (Tk): ", 0.0f);

    users[ui].monthlyBudget = b;
    saveUsers();

    printf("   [OK] Monthly budget set to %.2f Tk\n", b);
    pauseScreen();
}


int hasEnoughBalance(int ui, float amount)
{
    if (ui < 0 || ui >= userCount)
        return 0;
    return (users[ui].balance >= amount) ? 1 : 0;
}


void creditBalance(int ui, float amount)
{
    if (ui < 0 || ui >= userCount)
        return;
    users[ui].balance += amount;
}


void debitBalance(int ui, float amount)
{
    if (ui < 0 || ui >= userCount)
        return;
    users[ui].balance -= amount;
}


void showBudgetAlert(int ui)
{
    float spent, percent;

    if (users[ui].monthlyBudget <= 0.0f)
        return;

    spent   = currentMonthExpense(ui);
    percent = (spent / users[ui].monthlyBudget) * 100.0f;

    printf("\n");
    if (percent >= 100.0f) {
        lineBreak('*', 62);
        printf("   [ALERT] BUDGET EXCEEDED! Used %.1f%% of monthly budget.\n",
               percent);
        printf("           Over by %.2f Tk\n", spent - users[ui].monthlyBudget);
        lineBreak('*', 62);
    } else if (percent >= BUDGET_WARN_PERCENT) {
        printf("   [WARNING] You have used %.1f%% of your monthly budget.\n",
               percent);
    }

    if (users[ui].balance < LOW_BALANCE_LIMIT) {
        printf("   [WARNING] Low balance! Only %.2f Tk left.\n",
               users[ui].balance);
    }
}