#ifndef BUDGET_H
#define BUDGET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_TITLE 100
#define MAX_DATE 11
#define MAX_CATEGORY 30
#define MAX_RECORDS 500

typedef enum {
    EVENT_OPEN,
    EVENT_SETTLED
} EventStatus;

typedef enum {
    FUND_DRAFT,
    FUND_POSTED
} FundingStatus;

typedef struct {
    int id;
    char username[MAX_NAME];
    char password[MAX_NAME];
    double balance;
} User;

typedef struct {
    int id;
    int userId;
    double amount;
    char type[MAX_CATEGORY];
    char date[MAX_DATE];
} Transaction;

typedef struct {
    int id;
    char name[MAX_TITLE];
    double totalCost;
    char date[MAX_DATE];
    int creatorId;
    EventStatus status;
    int attendeeCount;
} Event;

typedef struct {
    int eventId;
    int userId;
    double paidAmount;
} Participant;

typedef struct {
    int id;
    char purpose[MAX_TITLE];
    double targetAmount;
    int collectorId;
    FundingStatus status;
} Funding;

typedef struct {
    int fundingId;
    int userId;
    double amount;
} Contributor;

/* Function prototypes */
void registerUser(void);
void loginUser(void);
void manageBudget(void);
void addExpense(void);
void createEvent(void);
void createFunding(void);
void searchRecords(void);
void handleCalendar(void);
void generateReport(void);
void saveToFile(void);
void loadFromFile(void);
void clearScreen(void);

#endif 