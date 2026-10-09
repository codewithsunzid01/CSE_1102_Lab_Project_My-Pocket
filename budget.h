#ifndef BUDGET_H
#define BUDGET_H

/* =========================================================
   My Pocket - Multi-User Student Budget & Finance System
   Master Header File : budget.h
   ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


/* =========================================================
   SECTION 1 : LIMITS / CONSTANTS
   ========================================================= */

#define MAX_USERS            100
#define MAX_TRANSACTIONS     2000
#define MAX_EVENTS           200
#define MAX_PARTICIPANTS     20
#define MAX_FUNDS            200
#define MAX_CONTRIBUTORS     20
#define MAX_REMINDERS        200
#define MAX_RECURRING        200

#define NAME_LEN             50
#define USERNAME_LEN         30
#define PASSWORD_LEN         30
#define PHONE_LEN            20
#define CATEGORY_LEN         30
#define DESC_LEN             100

#define LOW_BALANCE_LIMIT    200.0f   /* warn below this */
#define BUDGET_WARN_PERCENT  80.0f    /* warn at 80% of budget */


/* =========================================================
   SECTION 2 : DATA FILE NAMES
   ========================================================= */

#define FILE_USERS        "users.txt"
#define FILE_TRANSACTIONS "transactions.txt"
#define FILE_EVENTS       "events.txt"
#define FILE_FUNDS        "funds.txt"
#define FILE_REMINDERS    "reminders.txt"
#define FILE_RECURRING    "recurring.txt"


/* =========================================================
   SECTION 3 : ENUMS
   ========================================================= */

/* All transaction types used in the system */
typedef enum {
    TX_INCOME = 0,           /* money in  : salary, tuition, pocket money */
    TX_EXPENSE,              /* money out : personal spending             */
    TX_EVENT_PAYMENT,        /* money out : payer paid full event bill    */
    TX_EVENT_SETTLEMENT,     /* internal  : due clearing between users    */
    TX_FUND_CONTRIBUTION,    /* internal  : contributor -> collector      */
    TX_FUND_RECEIVED,        /* internal  : collector received total      */
    TX_INTERNAL_TRANSFER,    /* internal  : direct user to user transfer  */
    TX_TYPE_COUNT
} TransactionType;

/* Event cost split method */
typedef enum {
    SPLIT_EQUAL  = 1,
    SPLIT_CUSTOM = 2
} SplitType;

/* Recurring frequency */
typedef enum {
    FREQ_DAILY   = 1,
    FREQ_WEEKLY  = 2,
    FREQ_MONTHLY = 3
} Frequency;


/* =========================================================
   SECTION 4 : STRUCTURES
   ========================================================= */

/* ---------- Date ---------- */
typedef struct {
    int day;
    int month;
    int year;
} Date;


/* ---------- User ---------- */
typedef struct {
    int   userId;
    char  name[NAME_LEN];
    char  username[USERNAME_LEN];
    char  password[PASSWORD_LEN];
    char  phone[PHONE_LEN];
    float balance;
    float monthlyBudget;
    float totalIncome;
    float totalExpense;
    int   isActive;              /* 1 = active, 0 = deleted */
} User;


/* ---------- Transaction ---------- */
typedef struct {
    int   transactionId;
    int   userId;                /* owner of this transaction */
    int   type;                  /* TransactionType           */
    float amount;
    char  category[CATEGORY_LEN];
    char  description[DESC_LEN];
    Date  date;
    int   relatedUserId;         /* other side of internal tx, else -1 */
    int   relatedEventId;        /* linked event, else -1              */
    int   relatedFundId;         /* linked fund,  else -1              */
} Transaction;


/* ---------- Event participant ---------- */
typedef struct {
    int   userId;
    float shareAmount;           /* how much he must bear   */
    float paidAmount;            /* how much he already put */
    int   isSettled;             /* 1 = cleared, 0 = due    */
} EventParticipant;


/* ---------- Shared Event ---------- */
typedef struct {
    int   eventId;
    char  eventName[NAME_LEN];
    char  eventType[CATEGORY_LEN];   /* tour / transport / food ... */
    Date  date;
    float totalAmount;
    int   payerUserId;               /* who paid the bill */
    int   createdByUserId;
    int   splitType;                 /* SplitType */
    int   participantCount;
    EventParticipant participants[MAX_PARTICIPANTS];
    int   isClosed;                  /* 1 = all dues settled */
} SharedEvent;


/* ---------- Fund contributor ---------- */
typedef struct {
    int   userId;
    float amount;
    int   isPaid;                /* 1 = deducted successfully */
} FundContributor;


/* ---------- Fund record ---------- */
typedef struct {
    int   fundId;
    char  fundName[NAME_LEN];
    char  purpose[DESC_LEN];
    int   collectorUserId;
    Date  date;
    int   contributorCount;
    FundContributor contributors[MAX_CONTRIBUTORS];
    float totalCollected;
} FundRecord;


/* ---------- Reminder ---------- */
typedef struct {
    int   reminderId;
    int   userId;
    char  title[NAME_LEN];
    char  note[DESC_LEN];
    Date  dueDate;
    float expectedAmount;
    int   isDone;
} Reminder;


/* ---------- Recurring expense ---------- */
typedef struct {
    int   recurringId;
    int   userId;
    char  title[NAME_LEN];
    char  category[CATEGORY_LEN];
    float amount;
    int   frequency;             /* Frequency */
    Date  nextDate;
    int   isActive;
} RecurringExpense;


/* =========================================================
   SECTION 5 : GLOBAL DATA (defined in main.c)
   ========================================================= */

extern User             users[MAX_USERS];
extern Transaction      transactions[MAX_TRANSACTIONS];
extern SharedEvent      events[MAX_EVENTS];
extern FundRecord       funds[MAX_FUNDS];
extern Reminder         reminders[MAX_REMINDERS];
extern RecurringExpense recurrings[MAX_RECURRING];

extern int userCount;
extern int transactionCount;
extern int eventCount;
extern int fundCount;
extern int reminderCount;
extern int recurringCount;


/* =========================================================
   SECTION 6 : FUNCTION PROTOTYPES
   ========================================================= */

/* ---------- utils.c ---------- */
void  clearInputBuffer(void);
void  pauseScreen(void);
void  lineBreak(char ch, int n);
int   readInt(const char *prompt, int min, int max);
float readFloat(const char *prompt, float min);
void  readString(const char *prompt, char *dest, int size);
void  toLowerStr(char *s);
int   containsIgnoreCase(const char *haystack, const char *needle);

Date  getToday(void);
Date  inputDate(const char *prompt);
int   isValidDate(Date d);
int   compareDate(Date a, Date b);        /* -1 a<b, 0 equal, 1 a>b */
int   daysBetween(Date a, Date b);
void  printDate(Date d);
Date  addDays(Date d, int n);
Date  addMonths(Date d, int n);

const char *typeToString(int type);
int   stringToType(const char *s);


/* ---------- user.c ---------- */
int   generateUserId(void);
int   findUserById(int userId);
int   findUserByUsername(const char *username);
void  registerUser(void);
int   loginUser(void);                    /* returns user index or -1 */
void  viewProfile(int ui);
void  editProfile(int ui);
void  listAllUsers(void);


/* ---------- account.c ---------- */
void  viewBalance(int ui);
void  setMonthlyBudget(int ui);
int   hasEnoughBalance(int ui, float amount);
void  creditBalance(int ui, float amount);
void  debitBalance(int ui, float amount);
void  showBudgetAlert(int ui);


/* ---------- transaction.c ---------- */
int   generateTransactionId(void);
int   addTransaction(int userId, int type, float amount,
                     const char *category, const char *description,
                     Date date, int relatedUserId,
                     int relatedEventId, int relatedFundId);
void  printTransactionRow(const Transaction *t);
void  viewTransactions(int ui);
void  viewInternalTransactions(int ui);
void  searchTransactions(int ui);


/* ---------- income_expense.c ---------- */
void  addIncome(int ui);
void  addExpense(int ui);
void  viewCategorySummary(int ui);


/* ---------- event.c ---------- */
int   generateEventId(void);
int   findEventById(int eventId);
void  eventMenu(int ui);
void  createSharedEvent(int ui);
void  viewMyEvents(int ui);
void  viewEventDetails(int ui);
void  settleEventDue(int ui);
void  viewMyDues(int ui);


/* ---------- fund.c ---------- */
int   generateFundId(void);
int   findFundById(int fundId);
void  fundMenu(int ui);
void  createFundCollection(int ui);
void  viewFundRecords(int ui);
void  internalTransfer(int ui);
int   doTransfer(int fromIdx, int toIdx, float amount,
                 const char *note, int eventId, int fundId);


/* ---------- calendar.c ---------- */
int   generateReminderId(void);
void  calendarMenu(int ui);
void  addReminder(int ui);
void  viewReminders(int ui);
void  markReminderDone(int ui);
void  checkUpcomingExpenses(int ui);


/* ---------- recurring.c ---------- */
int   generateRecurringId(void);
void  recurringMenu(int ui);
void  addRecurringExpense(int ui);
void  viewRecurringExpenses(int ui);
void  processRecurringExpenses(int ui);
void  stopRecurringExpense(int ui);


/* ---------- report.c ---------- */
void  monthlyReview(int ui);
void  yearlySummary(int ui);
void  eventSpendingReport(int ui);
void  fundSummaryReport(int ui);


/* ---------- fileio.c ---------- */
void  loadAllData(void);
void  saveAllData(void);

void  loadUsers(void);        void saveUsers(void);
void  loadTransactions(void); void saveTransactions(void);
void  loadEvents(void);       void saveEvents(void);
void  loadFunds(void);        void saveFunds(void);
void  loadReminders(void);    void saveReminders(void);
void  loadRecurring(void);    void saveRecurring(void);


/* ---------- main.c ---------- */
void  showBanner(void);
void  mainMenu(void);
void  dashboard(int ui);

#endif /* BUDGET_H */
