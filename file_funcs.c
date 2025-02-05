#include <ncurses.h>
#include "file_funcs.h"


void Exit_Program(int code, char *massege)
{
    endwin();
    if (code == 0)
    {
        printf("program exit with code %d",code);
    }
    else
    {
        printf("program exit with code %d\nmassege%s", code, massege);
    }
    system("pkill -9 mpg321");
    exit(1);
}


void play_music(const char *file)
{
    stop_music();
    char command[256];

    snprintf(command, sizeof(command), "mpg321 -q %s </dev/null >/dev/null 2>&1 &", file);
    system(command);
}


void stop_music()
{
    system("pkill -9 mpg321");
}


int create_users_folder()
{
    const char *folder_name = "users";
    struct stat st = {0};
    if (stat(folder_name, &st) == -1)
    {
        if (mkdir(folder_name, 0777) == 0)
        {
            return 0;
        }
        else
        {
            perror("Error creating folder");
            return -1;
        }
    }
    else
    {
        return 1;
    }
}


int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}


void new_user_write_file(char *name, char *email, char *pass) // write file
{
    FILE *file;
    char filename[256];
    snprintf(filename, 256, "users/%s.txt", name);
    file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error creating user file");
        return;
    }
    fprintf(file, "Name:%s\nEmail:%s\nPassword:%s\n", name, email, pass);
    fclose(file);
}


void new_guest_write_file(char *name) // write file
{
    FILE *file;
    char filename[256];
    snprintf(filename, 256, "users/Guest_%s.txt", name);
    file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error creating user file");
        return;
    }
    fprintf(file, "Name:%s\n", name);
    fclose(file);
}