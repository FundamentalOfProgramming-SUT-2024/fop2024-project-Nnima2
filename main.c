#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "init_menus.h"
#include "pregame_menus.h"



int main()
{
    char username[MAX_USERNAME_LEN];
    Init_Menus(username);
    int guest = (!strstr(username,"Guest_"))?0:1;
    int pgm_choice = Open_Pregame_Menus(username,guest);
    endwin();
    printf("%d",pgm_choice);
    return 0;
}