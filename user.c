/* =========================================================
   user.c  -  Registration, Login, Profile Management
   ========================================================= */

#include "budget.h"


/* Generate next available user ID */
int generateUserId(void)
{
    int i, maxId = 0;
    for (i = 0; i < userCount; i++)
        if (users[i].userId > maxId)
            maxId = users[i].userId;
    return maxId + 1;
}


/* Find user array index by userId. Returns -1 if not found. */
int findUserById(int userId)
{
    int i;
    for (i = 0; i < userCount; i++)
        if (users[i].userId == userId && users[i].isActive)
            return i;
    return -1;
}


/* Find user array index by username (case-insensitive) */
int findUserByUsername(const char *username)
{
    int i;
    char a[USERNAME_LEN], b[USERNAME_LEN];

    strncpy(b, username, USERNAME_LEN - 1);
    b[USERNAME_LEN - 1] = '\0';
    toLowerStr(b);

    for (i = 0; i < userCount; i++) {
        strncpy(a, users[i].username, USERNAME_LEN - 1);
        a[USERNAME_LEN - 1] = '\0';
        toLowerStr(a);
        if (strcmp(a, b) == 0)
            return i;
    }
    return -1;
}


/* ---------------- REGISTER ---------------- */
void registerUser(void)
{
    User u;
    char uname[USERNAME_LEN];
    char pass1[PASSWORD_LEN], pass2[PASSWORD_LEN];

    if (userCount >= MAX_USERS) {
        printf("\n   [!] User limit reached (%d).\n", MAX_USERS);
        pauseScreen();
        return;
    }

    printf("\n");
    lineBreak('-', 62);
    printf("   NEW USER REGISTRATION\n");
    lineBreak('-', 62);

    memset(&u, 0, sizeof(User));

    readString("   Full Name          : ", u.name, NAME_LEN);

    /* unique username */
    while (1) {
        readString("   Username           : ", uname, USERNAME_LEN);
        if (strchr(uname, ' ') != NULL) {
            printf("   [!] Username cannot contain spaces.\n");
            continue;
        }
        if (findUserByUsername(uname) >= 0) {
            printf("   [!] Username already taken. Try another.\n");
            continue;
        }
        strcpy(u.username, uname);
        break;
    }

    /* password with confirmation */
    while (1) {
        readString("   Password           : ", pass1, PASSWORD_LEN);
        if (strlen(pass1) < 4) {
            printf("   [!] Password must be at least 4 characters.\n");
            continue;
        }
        readString("   Confirm Password   : ", pass2, PASSWORD_LEN);
        if (strcmp(pass1, pass2) != 0) {
            printf("   [!] Passwords do not match.\n");
            continue;
        }
        strcpy(u.password, pass1);
        break;
    }

    readString("   Phone / Email      : ", u.phone, PHONE_LEN);

    u.userId        = generateUserId();
    u.balance       = readFloat("   Opening Balance(Tk): ", 0.0f);
    u.monthlyBudget = readFloat("   Monthly Budget (Tk): ", 0.0f);
    u.totalIncome   = u.balance;
    u.totalExpense  = 0.0f;
    u.isActive      = 1;

    users[userCount] = u;
    userCount++;

    /* record opening balance as income */
    if (u.balance > 0.0f) {
        addTransaction(u.userId, TX_INCOME, u.balance,
                       "Opening Balance", "Initial account balance",
                       getToday(), -1, -1, -1);
    }

    saveUsers();
    saveTransactions();

    printf("\n   [OK] Registration successful!\n");
    printf("   Your User ID is: %d  (share this for group events)\n", u.userId);
    pauseScreen();
}


/* ---------------- LOGIN ---------------- */
int loginUser(void)
{
    char uname[USERNAME_LEN];
    char pass[PASSWORD_LEN];
    int idx, attempts;

    if (userCount == 0) {
        printf("\n   [!] No users registered yet. Please register first.\n");
        pauseScreen();
        return -1;
    }

    printf("\n");
    lineBreak('-', 62);
    printf("   LOGIN\n");
    lineBreak('-', 62);

    for (attempts = 1; attempts <= 3; attempts++) {
        readString("   Username : ", uname, USERNAME_LEN);
        readString("   Password : ", pass,  PASSWORD_LEN);

        idx = findUserByUsername(uname);

        if (idx >= 0 && strcmp(users[idx].password, pass) == 0
                     && users[idx].isActive) {
            printf("\n   [OK] Welcome back, %s!\n", users[idx].name);
            return idx;
        }

        printf("   [!] Invalid username or password. (Attempt %d/3)\n",
               attempts);
    }

    printf("\n   [!] Too many failed attempts. Returning to main menu.\n");
    pauseScreen();
    return -1;
}


/* ---------------- VIEW PROFILE ---------------- */
void viewProfile(int ui)
{
    float savings = users[ui].totalIncome - users[ui].totalExpense;

    printf("\n");
    lineBreak('=', 62);
    printf("   MY PROFILE\n");
    lineBreak('=', 62);
    printf("   User ID           : %d\n",       users[ui].userId);
    printf("   Name              : %s\n",       users[ui].name);
    printf("   Username          : %s\n",       users[ui].username);
    printf("   Phone / Email     : %s\n",       users[ui].phone);
    lineBreak('-', 62);
    printf("   Current Balance   : %10.2f Tk\n", users[ui].balance);
    printf("   Monthly Budget    : %10.2f Tk\n", users[ui].monthlyBudget);
    printf("   Total Income      : %10.2f Tk\n", users[ui].totalIncome);
    printf("   Total Expense     : %10.2f Tk\n", users[ui].totalExpense);
    printf("   Net Savings       : %10.2f Tk\n", savings);
    lineBreak('=', 62);

    pauseScreen();
}


/* ---------------- EDIT PROFILE ---------------- */
void editProfile(int ui)
{
    int choice;
    char buf[NAME_LEN];
    char p1[PASSWORD_LEN], p2[PASSWORD_LEN];

    printf("\n");
    lineBreak('-', 62);
    printf("   EDIT PROFILE\n");
    lineBreak('-', 62);
    printf("    1. Change Name\n");
    printf("    2. Change Phone / Email\n");
    printf("    3. Change Password\n");
    printf("    0. Back\n");
    lineBreak('-', 62);

    choice = readInt("   Choice: ", 0, 3);

    switch (choice) {
        case 1:
            readString("   New Name : ", buf, NAME_LEN);
            strcpy(users[ui].name, buf);
            printf("   [OK] Name updated.\n");
            break;

        case 2:
            readString("   New Phone/Email : ", buf, PHONE_LEN);
            strncpy(users[ui].phone, buf, PHONE_LEN - 1);
            users[ui].phone[PHONE_LEN - 1] = '\0';
            printf("   [OK] Contact updated.\n");
            break;

        case 3:
            readString("   Current Password : ", p1, PASSWORD_LEN);
            if (strcmp(p1, users[ui].password) != 0) {
                printf("   [!] Wrong password.\n");
                break;
            }
            readString("   New Password     : ", p1, PASSWORD_LEN);
            readString("   Confirm Password : ", p2, PASSWORD_LEN);
            if (strcmp(p1, p2) != 0) {
                printf("   [!] Passwords do not match.\n");
                break;
            }
            strcpy(users[ui].password, p1);
            printf("   [OK] Password changed.\n");
            break;

        case 0:
            return;
    }

    saveUsers();
    pauseScreen();
}


/* ---------------- LIST ALL USERS ---------------- */
void listAllUsers(void)
{
    int i, shown = 0;

    printf("\n");
    lineBreak('=', 62);
    printf("   REGISTERED USERS\n");
    lineBreak('=', 62);
    printf("   %-6s %-22s %-18s\n", "ID", "NAME", "USERNAME");
    lineBreak('-', 62);

    for (i = 0; i < userCount; i++) {
        if (!users[i].isActive)
            continue;
        printf("   %-6d %-22s %-18s\n",
               users[i].userId, users[i].name, users[i].username);
        shown++;
    }

    if (shown == 0)
        printf("   (no users found)\n");

    lineBreak('=', 62);
}
