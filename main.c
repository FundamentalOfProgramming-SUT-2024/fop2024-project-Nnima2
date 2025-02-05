#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "init_menus.h"
#include "pregame_menus.h"
#include "global_vars.h"
#include "file_funcs.h"
#include "game.h"


int main()
{
    char * default_music = "music/music_1.mp3";
    play_music(default_music);

    //Init_Menus();

    strcpy(username, "Not_Nima2");

    while(state != -1)
    {
        if(state==0)
        {
            Open_Pregame_Menus();
        }
        if(state ==1)
        {
            new_game();
            break;
            Exit_Program(0,NULL);
        }
        if(state ==2)
        {
            printf(" continue game ");
        }
    }
    endwin();
    return 0;
}