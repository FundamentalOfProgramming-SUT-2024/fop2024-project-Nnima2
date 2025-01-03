#ifndef _FILE_FUNCS_H_
#define _FILE_FUNCS_H_
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/stat.h>
    int create_users_folder();
    int file_exists(const char *filename);
    void new_user_write_file(char*name,char*email,char*pass);
#endif 