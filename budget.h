#ifndef BUDGET_H
#define BUDGET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- MACROS ---------- */

#define MAX_USERS 100
#define MAX_TRANSACTIONS 500
#define MAX_EVENTS 100
#define MAX_FUNDINGS 100

#define USER_FILE "data/users.dat"
#define TRANSACTION_FILE "data/transactions.dat"
#define EVENT_FILE "data/events.dat"
#define FUNDING_FILE "data/fundings.dat"

/* ---------- ENUM ---------- */

typedef enum {
    TRANSFER,
    FUNDING,
    GROUP_PAYMENT,
    SETTLEMENT
} TransactionType;


/* ---------- STRUCTURES ---------- */

typedef struct {
    char username[30];
    char password[50];

    float balance;
    float monthly_income;
    float monthly_budget;
    float savings_goal;

} User;


typedef struct {
    int id;

    char username[30];
    char description[100];
    char category[30];

    float amount;

    int day;
    int month;
    int year;

} Transaction;


/* ---------- FUNCTION PROTOTYPES ---------- */

/* Authentication */
void register_user(void);
int login_user(const char username[]);

/* Budget */
void show_dashboard(User *user);
void user_menu(User *user);

/* File handling */
void save_user(User *user);
int load_user(const char username[], User *user);

#endif