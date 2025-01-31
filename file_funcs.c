#include "file_funcs.h"

    int create_users_folder() {
    const char *folder_name = "users";
    struct stat st = {0};
    if (stat(folder_name, &st) == -1) {
        if (mkdir(folder_name, 0777) == 0) {
            return 0; 
        } else {
            perror("Error creating folder");
            return -1;
        }
    } else {
        return 1;
    }
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0; 
}

void new_user_write_file(char*name,char*email,char*pass)//write file
{
    FILE *file;
    char filename[256];
    snprintf(filename,256,"users/%s.txt",name);
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating user file");
        return;
    }
    fprintf(file,"Name:%s\nEmail:%s\nPassword:%s\n", name, email, pass);
    fclose(file);
}

void new_guest_write_file(char*name)//write file
{
    FILE *file;
    char filename[256];
    snprintf(filename,256,"users/Guest_%s.txt",name);
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating user file");
        return;
    }
    fprintf(file,"Name:%s\n", name);
    fclose(file);
}