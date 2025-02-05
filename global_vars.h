#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_
    #define MAX_USERNAME_LEN 100
    #define MAX_FILE_LINE 256
    extern char username[MAX_USERNAME_LEN];
    extern char *  username_ptr;
    extern int guest;
    #define USER_DIR "users/"
    #define MUSIC_DIR "music/"
    #define GAMES_DIR "games/"
    extern int main();
    extern int state;
#endif