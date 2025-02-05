/*#include <ncurses.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, ""); // فعال‌سازی پشتیبانی از یونیکد
    initscr();             // راه‌اندازی ncurses
    noecho();
    curs_set(0);

    addwstr(L"⚒ Unicode Test ☀ ☯ ✔ 😀 👍"); 
#pragma region unicode test

    refresh();
    getch(); // منتظر ورودی بماند
    endwin(); // خروج از ncurses

    return 0;
}

*/

// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>

// #include "scoreboard.h"

// int main()
// {
//     int inputs = 20;
//     score * scoreboard = malloc(20* sizeof(score));
//     for(int i =0;i<20;i++)
//     {
//         printf("getting guy %d\n\n",i+1);

//         char name [256];
//         printf("Enter name:");
//         scanf("%s",name);
//         strcpy(scoreboard[i].username,name);

//         int gold_collected = 4000 - 100*i;
//         scoreboard[i].gold_collected = gold_collected;
        
//         int score = 6000-100*i;
//         scoreboard[i].score = score;

//         int game_count = 100;
//         scoreboard[i].game_count = game_count;

//         int exp = 100;
//         scoreboard[i].time_since_first_game = exp;

//     }
//     write_scoreboard(scoreboard,20);
// }