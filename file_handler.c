#include "budget.h"

void save_user(User *user)
{
    FILE *file;

    file = fopen(USER_FILE, "ab");

    if (file == NULL)
    {
        printf("Error opening user file.\n");
        return;
    }

    fwrite(user, sizeof(User), 1, file);

    fclose(file);
}

int load_user(const char username[], User *user)
{
    FILE *file;

    file = fopen(USER_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user->username, username) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}