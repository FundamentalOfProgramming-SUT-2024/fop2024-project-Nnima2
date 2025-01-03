#include "str_processing.h"

int Is_Email_Valid(char *email)
{
    char *at_ptr = strchr(email, '@');
    char *dot_ptr = at_ptr ? strchr(at_ptr, '.') : NULL;
    if (!at_ptr || !dot_ptr) // check if has <@> and <.>
        return 0;
    if (at_ptr == email || (dot_ptr - at_ptr) <= 1 || *(dot_ptr + 1) == '\0')
    {
        return 0;
    }

    return 1;
}
int Is_Password_Valid(char *pass)
{
    if (strlen(pass) < 7)
    {
        return 0;
    }
    int has_upper = 0;
    int has_lower = 0;
    int has_digit = 0;
    for (int i = 0; i < strlen(pass); i++)
    {
        has_upper = (isupper(pass[i])) ? 1 : has_upper;
        has_lower = (islower(pass[i])) ? 1 : has_lower;
        has_digit = (isdigit(pass[i])) ? 1 : has_digit;
    }
    if (!has_digit)
        return -1;
    else if (!has_lower || !has_upper)
        return -2;

    return 1;
}

void Generate_Random_String(char *str, int length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.*[!@#$%%^&*()_+|~=`{}:\";'<>?,./-]"; // مجموعه کاراکترها
    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < length; i++)
    {
        int random_index = rand() % charset_size;
        str[i] = charset[random_index];
    }
    str[length] = '\0';
}