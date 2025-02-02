#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "init_menus.h"
#include "pregame_menus.h"
#include "global_vars.h"


void stop_music() {
    system("pkill -9 mpg321"); // متوقف کردن آهنگ در حال پخش
}

void play_music(const char *file) {
    stop_music(); // ابتدا آهنگ قبلی رو قطع کن
    char command[256];

    snprintf(command, sizeof(command), "mpg321 -q %s </dev/null >/dev/null 2>&1 &", file); // اجرای آهنگ جدید در پس‌زمینه
    system(command);
}


int main()
{
    char * default_music = "music/music_1.mp3";
    play_music(default_music);
    Init_Menus();

    if(!guest)
    {
    mvprintw(getmaxy(stdscr)/2,(getmaxx(stdscr)-20)/2,"wellcome back hero.");
    mvprintw(getmaxy(stdscr)/2+1,(getmaxx(stdscr)-18-strlen(username))/2,"There's been while %s",username);
    getch();
    }

    int pgm_choice = Open_Pregame_Menus();

    

    endwin();
    printf("%d",pgm_choice);
    return 0;
}