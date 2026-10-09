/* =========================================================
   main.c  -  Program entry point, menus, global data
   My Pocket - Multi-User Student Budget System
   ========================================================= */

#include "budget.h"


/* =========================================================
   GLOBAL DATA DEFINITIONS
   (declared as 'extern' in budget.h - defined ONLY here)
   ========================================================= */

User             users[MAX_USERS];
Transaction      transactions[MAX_TRANSACTIONS];
SharedEvent      events[MAX_EVENTS];
FundRecord       funds[MAX_FUNDS];
Reminder         reminders[MAX_REMINDERS];
RecurringExpense recurrings[MAX_RECURRING];

int userCount        = 0;
int transactionCount = 0;
int eventCount       = 0;
int fundCount        = 0;
int reminderCount    = 0;
int recurringCount   = 0;


/* =========================================================
   BANNER
   ========================================================= */

void showBanner(void)
{
    printf("\n");
    lineBreak('=', 62);
    printf("        M Y   P O C K E T   -   v1.0\n");
    printf("   Multi-User Student Budget & Financial Planning System\n");
    lineBreak('=', 62);
}


/* =========================================================
   USER DASHBOARD  (after successful login)
   ========================================================= */

void dashboard(int ui)
{
    int choice;

    /* Auto-run checks the moment user logs in */
    processRecurringExpenses(ui);
    checkUpcomingExpenses(ui);
    showBudgetAlert(ui);

    while (1) {
        printf("\n");
        lineBreak('-', 62);
        printf("   DASHBOARD  |  %s  |  Balance: %.2f Tk\n",
               users[ui].name, users[ui].balance);
        lineBreak('-', 62);
        printf("    1. View Profile\n");
        printf("    2. Add Income\n");
        printf("    3. Add Expense\n");
        printf("    4. View Balance\n");
        printf("    5. Set Monthly Budget\n");
        printf("    6. Shared Event Management\n");
        printf("    7. Funding / Internal Transfer\n");
        printf("    8. Transaction History\n");
        printf("    9. Search Transactions\n");
        printf("   10. Calendar & Reminders\n");
        printf("   11. Recurring Expenses\n");
        printf("   12. Monthly Review & Reports\n");
        printf("   13. Edit Profile\n");
        printf("   14. Save Data\n");
        printf("    0. Logout\n");
        lineBreak('-', 62);

        choice = readInt("   Enter your choice: ", 0, 14);

        switch (choice) {
            case 1:  viewProfile(ui);          break;
            case 2:  addIncome(ui);            break;
            case 3:  addExpense(ui);           break;
            case 4:  viewBalance(ui);          break;
            case 5:  setMonthlyBudget(ui);     break;
            case 6:  eventMenu(ui);            break;
            case 7:  fundMenu(ui);             break;
            case 8:  viewTransactions(ui);     break;
            case 9:  searchTransactions(ui);   break;
            case 10: calendarMenu(ui);         break;
            case 11: recurringMenu(ui);        break;
            case 12: monthlyReview(ui);        break;
            case 13: editProfile(ui);          break;

            case 14:
                saveAllData();
                printf("\n   [OK] All data saved successfully.\n");
                break;

            case 0:
                saveAllData();
                printf("\n   [OK] Logged out. Data saved.\n");
                return;
        }
    }
}


/* =========================================================
   MAIN MENU
   ========================================================= */

void mainMenu(void)
{
    int choice;
    int ui;

    while (1) {
        printf("\n");
        lineBreak('-', 62);
        printf("   MAIN MENU\n");
        lineBreak('-', 62);
        printf("    1. Register New User\n");
        printf("    2. Login\n");
        printf("    3. View All Users\n");
        printf("    0. Exit\n");
        lineBreak('-', 62);

        choice = readInt("   Enter your choice: ", 0, 3);

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                ui = loginUser();
                if (ui >= 0)
                    dashboard(ui);
                break;

            case 3:
                listAllUsers();
                break;

            case 0:
                saveAllData();
                printf("\n   Thank you for using My Pocket. Goodbye!\n\n");
                return;
        }
    }
}


/* =========================================================
   ENTRY POINT
   ========================================================= */

int main(void)
{
    showBanner();

    printf("\n   Loading data...\n");
    loadAllData();
    printf("   Loaded: %d users | %d transactions | %d events | %d funds\n",
           userCount, transactionCount, eventCount, fundCount);

    mainMenu();

    return 0;
}
