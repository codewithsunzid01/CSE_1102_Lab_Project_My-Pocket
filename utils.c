/* =========================================================
   utils.c  -  Common helper utilities
   My Pocket - Multi-User Student Budget System
   ========================================================= */

#include "budget.h"


/* =========================================================
   SECTION A : INPUT / SCREEN HELPERS
   ========================================================= */

/* Flush leftover characters from stdin (after scanf) */
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Wait for user to press Enter */
void pauseScreen(void)
{
    printf("\n   Press Enter to continue...");
    clearInputBuffer();
}

/* Print a horizontal line of 'n' characters 'ch' */
void lineBreak(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
        putchar(ch);
    putchar('\n');
}

/* Safe integer input with range validation.
   Keeps asking until a valid number inside [min, max] is given. */
int readInt(const char *prompt, int min, int max)
{
    int value;
    int ok;

    while (1) {
        printf("%s", prompt);
        ok = scanf("%d", &value);
        clearInputBuffer();

        if (ok != 1) {
            printf("   [!] Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("   [!] Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

/* Safe float input, must be >= min */
float readFloat(const char *prompt, float min)
{
    float value;
    int ok;

    while (1) {
        printf("%s", prompt);
        ok = scanf("%f", &value);
        clearInputBuffer();

        if (ok != 1) {
            printf("   [!] Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min) {
            printf("   [!] Value must be at least %.2f.\n", min);
            continue;
        }
        return value;
    }
}

/* Safe string input (allows spaces), trims trailing newline */
void readString(const char *prompt, char *dest, int size)
{
    int len;

    while (1) {
        printf("%s", prompt);

        if (fgets(dest, size, stdin) == NULL) {
            dest[0] = '\0';
            continue;
        }

        len = (int)strlen(dest);

        /* remove trailing newline */
        if (len > 0 && dest[len - 1] == '\n') {
            dest[len - 1] = '\0';
            len--;
        } else {
            clearInputBuffer();   /* input was too long */
        }

        if (len == 0) {
            printf("   [!] This field cannot be empty.\n");
            continue;
        }
        return;
    }
}


/* =========================================================
   SECTION B : STRING HELPERS
   ========================================================= */

/* Convert a string to lowercase in place */
void toLowerStr(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        s[i] = (char)tolower((unsigned char)s[i]);
}

/* Case-insensitive substring search.
   Returns 1 if 'needle' is found inside 'haystack', else 0. */
int containsIgnoreCase(const char *haystack, const char *needle)
{
    char a[DESC_LEN * 2];
    char b[DESC_LEN * 2];

    if (needle == NULL || needle[0] == '\0')
        return 1;

    strncpy(a, haystack, sizeof(a) - 1);
    a[sizeof(a) - 1] = '\0';
    strncpy(b, needle, sizeof(b) - 1);
    b[sizeof(b) - 1] = '\0';

    toLowerStr(a);
    toLowerStr(b);

    return (strstr(a, b) != NULL) ? 1 : 0;
}


/* =========================================================
   SECTION C : DATE HELPERS
   ========================================================= */

/* Is the given year a leap year? */
static int isLeapYear(int y)
{
    return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0));
}

/* Number of days in a given month of a given year */
static int daysInMonth(int month, int year)
{
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month < 1 || month > 12)
        return 0;
    if (month == 2 && isLeapYear(year))
        return 29;

    return days[month - 1];
}

/* Convert a Date to Julian Day Number (for date arithmetic) */
static long toJDN(Date d)
{
    long a = (14 - d.month) / 12;
    long y = d.year + 4800 - a;
    long m = d.month + 12 * a - 3;

    return d.day + (153 * m + 2) / 5 + 365 * y
           + y / 4 - y / 100 + y / 400 - 32045;
}

/* Convert a Julian Day Number back to a Date */
static Date fromJDN(long jdn)
{
    Date d;
    long a = jdn + 32044;
    long b = (4 * a + 3) / 146097;
    long c = a - (146097 * b) / 4;
    long dd = (4 * c + 3) / 1461;
    long e = c - (1461 * dd) / 4;
    long m = (5 * e + 2) / 153;

    d.day   = (int)(e - (153 * m + 2) / 5 + 1);
    d.month = (int)(m + 3 - 12 * (m / 10));
    d.year  = (int)(100 * b + dd - 4800 + m / 10);

    return d;
}

/* Get today's system date */
Date getToday(void)
{
    Date d;
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    d.day   = lt->tm_mday;
    d.month = lt->tm_mon + 1;
    d.year  = lt->tm_year + 1900;

    return d;
}

/* Validate a date */
int isValidDate(Date d)
{
    if (d.year < 2000 || d.year > 2100)
        return 0;
    if (d.month < 1 || d.month > 12)
        return 0;
    if (d.day < 1 || d.day > daysInMonth(d.month, d.year))
        return 0;

    return 1;
}

/* Ask the user for a date. Pressing 0 for day uses today's date. */
Date inputDate(const char *prompt)
{
    Date d;

    printf("%s\n", prompt);

    while (1) {
        d.day = readInt("   Day   (0 = use today) : ", 0, 31);

        if (d.day == 0) {
            d = getToday();
            printf("   -> Using today's date: ");
            printDate(d);
            printf("\n");
            return d;
        }

        d.month = readInt("   Month (1-12)          : ", 1, 12);
        d.year  = readInt("   Year  (2000-2100)     : ", 2000, 2100);

        if (!isValidDate(d)) {
            printf("   [!] Invalid date. Please try again.\n");
            continue;
        }
        return d;
    }
}

/* Compare two dates.
   Returns -1 if a < b, 0 if equal, 1 if a > b */
int compareDate(Date a, Date b)
{
    if (a.year != b.year)
        return (a.year < b.year) ? -1 : 1;
    if (a.month != b.month)
        return (a.month < b.month) ? -1 : 1;
    if (a.day != b.day)
        return (a.day < b.day) ? -1 : 1;

    return 0;
}

/* Days from a to b. Positive if b is after a. */
int daysBetween(Date a, Date b)
{
    return (int)(toJDN(b) - toJDN(a));
}

/* Print a date as DD-MM-YYYY */
void printDate(Date d)
{
    printf("%02d-%02d-%04d", d.day, d.month, d.year);
}

/* Add n days to a date */
Date addDays(Date d, int n)
{
    return fromJDN(toJDN(d) + n);
}

/* Add n months to a date (day is clamped to month end) */
Date addMonths(Date d, int n)
{
    int total = (d.year * 12) + (d.month - 1) + n;
    int maxDay;
    Date r;

    r.year  = total / 12;
    r.month = (total % 12) + 1;

    maxDay = daysInMonth(r.month, r.year);
    r.day  = (d.day > maxDay) ? maxDay : d.day;

    return r;
}


/* =========================================================
   SECTION D : TRANSACTION TYPE HELPERS
   ========================================================= */

/* Order MUST match the TransactionType enum in budget.h */
static const char *txNames[TX_TYPE_COUNT] = {
    "INCOME",
    "EXPENSE",
    "EVENT_PAYMENT",
    "EVENT_SETTLEMENT",
    "FUND_CONTRIBUTION",
    "FUND_RECEIVED",
    "INTERNAL_TRANSFER"
};

/* Convert enum value -> readable string */
const char *typeToString(int type)
{
    if (type < 0 || type >= TX_TYPE_COUNT)
        return "UNKNOWN";
    return txNames[type];
}

/* Convert string -> enum value. Returns -1 if not found. */
int stringToType(const char *s)
{
    int i;
    for (i = 0; i < TX_TYPE_COUNT; i++) {
        if (strcmp(s, txNames[i]) == 0)
            return i;
    }
    return -1;
}
