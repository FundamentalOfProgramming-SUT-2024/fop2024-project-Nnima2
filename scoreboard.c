#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "global_vars.h"
#include "file_funcs.h"
#include "scoreboard.h"
#include "pregame_menus.h"

int main();

score *Read_Scoreboard(int *player_count_ptr)
{
    if (!file_exists(SCOREBOARD_PATH))
    {
        FILE *scoreboard_file = fopen(SCOREBOARD_PATH, "wb");
        int temp_player_count = 0;
        fwrite(&temp_player_count, sizeof(int), 1, scoreboard_file);
        fclose(scoreboard_file);
        *player_count_ptr = 0;
        return NULL;
    }

    FILE *scoreboard_file = fopen(SCOREBOARD_PATH, "rb");

    int player_count;
    fread(&player_count, sizeof(int), 1, scoreboard_file);
    *player_count_ptr = player_count;

    if (player_count == 0)
    {
        fclose(scoreboard_file);
        return NULL;
    }

    score *score_list = malloc(player_count * sizeof(score));
    fread(score_list, sizeof(score), player_count, scoreboard_file);

    fclose(scoreboard_file);
    return score_list;
}

void write_scoreboard(score *score_list, int player_count)
{
    FILE *scoreboard_file = fopen(SCOREBOARD_PATH, "wb");

    fseek(scoreboard_file, 0, SEEK_SET);
    fwrite(&player_count, sizeof(int), 1, scoreboard_file);
    fwrite(score_list, sizeof(score), player_count, scoreboard_file);
    fclose(scoreboard_file);
    return;
}

void Insert_To_Scoreboard(score current_score)
{
    int player_count;
    score *scoreboard = Read_Scoreboard(&player_count);

    if (scoreboard == NULL)
    {
        player_count = 0;
        scoreboard = NULL;
    }

    FILE *temp_scoreboard_file = fopen(TEMP_SCOREBOARD_PATH, "wb");
    int temp_player_count = 0;
    fwrite(&temp_player_count, sizeof(int), 1, temp_scoreboard_file);

    int found = 0;
    int inserted = 0;
    for (int i = 0; i < player_count; i++)
    {
        if (!inserted && strcmp(current_score.username, scoreboard[i].username) == 0 && scoreboard[i].score >= current_score.score)
        {
            found = 1;
            fclose(temp_scoreboard_file);

            if (remove(TEMP_SCOREBOARD_PATH) != 0)
            {
                perror("Error removing temp file");
            }
            free(scoreboard);
            return; // player has already better score
        }
        else if (inserted && strcmp(current_score.username, scoreboard[i].username) == 0)
        {
            found = 1;
            continue; // score ghabli ahamiat nadare chon jadidi behtare
        }
        else
        {
            if (current_score.score > scoreboard[i].score)
            {
                inserted = 1;
                fwrite(&current_score, sizeof(score), 1, temp_scoreboard_file);
            }
            fwrite(scoreboard + i, sizeof(score), 1, temp_scoreboard_file);
        }
    }
    if (!found)
        player_count++; // if player hasn't already a score
    if (!inserted)
        fwrite(&current_score, sizeof(score), 1, temp_scoreboard_file); // if player is last rank

    if (remove(SCOREBOARD_PATH) != 0)
    {
        perror("error to remove main scoreboard\n");
        fclose(temp_scoreboard_file);
        return;
    }

    fseek(temp_scoreboard_file, 0, SEEK_SET); // update player count
    fwrite(&player_count, sizeof(int), 1, temp_scoreboard_file);

    rename(TEMP_SCOREBOARD_PATH, SCOREBOARD_PATH);
    fclose(temp_scoreboard_file);
    free(scoreboard);
    return;
}


void display_scoreboard(score *scoreboard, int player_count)
{
    initscr();
    setlocale(LC_ALL, "");  // تنظیم برای پشتیبانی از یونیکد
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_pair(1, COLOR_YELLOW, COLOR_BLACK); //top-three
    init_pair(2, COLOR_CYAN, COLOR_BLACK);   //current users
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // others

    int ch, start = 0;
    do
    {
        clear();
        
        move(1, 10);
        addwstr(L"🏆 Leaderboard 🏆");

        mvprintw(3, 2, "\tRank\t| Username\t\t\t| Score\t| Gold\t| Games\t| Experience");
        mvprintw(4, 2, "----------------------------------------------------------------------");

        for (int i = start; i < start + ITEMS_PER_PAGE && i < player_count; i++)
        {
            if (strcmp(scoreboard[i].username, username) == 0)
            {
                attron(A_ITALIC | COLOR_PAIR(2));
            }
            else if (i == 0)
            {
                attron(A_ITALIC | COLOR_PAIR(1));
                
                move(6 + (i - start), 2);
                addwstr(L"🏆");   
            }
            else if (i == 1)
            {
                attron(A_ITALIC | COLOR_PAIR(1));

                move(6 + (i - start), 2);
                addwstr(L"🥈");
            }
            else if (i == 2)
            {
                attron(A_ITALIC | COLOR_PAIR(1));

                move(6 + (i - start), 2);
                addwstr(L"🥉");
            }
            else
            {
                attrset(COLOR_PAIR(3));
            }
            char * dot2 = "..";
            char printing_name[18];
            memset(printing_name,0,18);
            if(i==0)
            {
                strcpy(printing_name,"goat ");
                if(strlen(scoreboard[i].username)>12)
                {
                    strncat(printing_name,scoreboard[i].username,10);
                    strncat(printing_name,dot2,2);
                }
                else
                {
                    strncat(printing_name,scoreboard[i].username,12);
                }
            }
            else if(i==1)
            {
                strcpy(printing_name,"legend ");
                if(strlen(scoreboard[i].username)>10)
                {
                    strncat(printing_name,scoreboard[i].username,8);
                    strncat(printing_name,dot2,2);
                }
                else
                {
                    strncat(printing_name,scoreboard[i].username,10);
                }
            }
            else if(i== 2)
            {
                strcpy(printing_name,"hero ");
                if(strlen(scoreboard[i].username)>12)
                {
                    strncat(printing_name,scoreboard[i].username,10);
                    strncat(printing_name,dot2,2);
                }
                else
                {
                    strncat(printing_name,scoreboard[i].username,12);
                }
            }
            else
            {
                if(strlen(scoreboard[i].username)>16)
                {
                    strncat(printing_name,scoreboard[i].username,15);
                    strncat(printing_name,dot2,2);
                }
                else
                {
                    strncat(printing_name,scoreboard[i].username,16);
                }
            }
            mvprintw(6 + (i - start), 10, "%3d\t| %-18s\t| %5d\t| %4d\t| %5d\t| %10d",
                     i+1,
                     printing_name, scoreboard[i].score,
                     scoreboard[i].gold_collected, scoreboard[i].game_count,
                     scoreboard[i].time_since_first_game);

            attroff(A_BOLD);
            attroff(A_ITALIC);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));

        }
        refresh();

        

        mvprintw(8+ITEMS_PER_PAGE, 2, "Use UP/DOWN to scroll, ESC to exit.");
        mvprintw(10+ITEMS_PER_PAGE, 2, "Left/Right Arrow to change page");


        ch = getch();

        if(ch == KEY_DOWN)
        {
            start=(start<player_count-1)?start+1:start;
        }
        else if(ch == KEY_UP)
        {
            start=(start>1)?start-1:0;
        }
        if(ch == KEY_RIGHT)
        {
            start=(start<player_count-ITEMS_PER_PAGE)?start+ITEMS_PER_PAGE:start;
        }
        else if(ch == KEY_LEFT)
        {
            start=(start>ITEMS_PER_PAGE)?start-ITEMS_PER_PAGE:0;
        }

        refresh();

    } while (ch != 27);

    clear();
    free(scoreboard);
    refresh();
    Open_Pregame_Menus();    
}

