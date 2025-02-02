#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pregame_menus.h"
#include "ascii_graphics.h"
#include "global_vars.h"
#include "file_funcs.h"



#define PGMW_WIDTH 65  // pregame menu window
#define PGMW_HEIGHT 20 // pregame menu window

char *pgm_global_username;
char *pgm_global_guestmode;


int Open_Pregame_Menus()
{
    if(guest)
        mvprintw(1,1,"i playyed this game before you are %s",username);
    #pragma region LABELS
    char *rogue_plus_text1[] = {
    " _____                                         ",
    "|  __ \\                                    _   ",
    "| |__) |   ___     __ _   _   _    ___   _| |_ ",
    "|  _  /   / _ \\   / _` | | | | |  / _ \\ |_   _|",
    "| | \\ \\  | (_) | | (_| | | |_| | |  __/   |_|  ",
    "|_|  \\_\\  \\___/   \\__, |  \\__,_|  \\___|        ",
    "                   __/ |                       ",
    "                  |___/                        "};
    char *pgm_choices_guest[] = {
        "New Game",
        "Scoreboard",
        "Settings",//local for guests dont save
        "Sign In",
        "Exit",
        NULL // برای پایان لیست
    };

    char *pgm_choices_user[] = {
        "New Game",
        "Continue Last Game",
        "Scoreboard",
        "Settings",
        "User Profile",
        "Sign Out",
        "Exit",
        NULL};
    #pragma endregion
    char **pgm_choices = (guest) ? pgm_choices_guest : pgm_choices_user;
    int pgm_choices_count = (guest) ? 5 : 7;

    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 33)
    {
        mvprintw(10, 10, "pls zomeout and resize your terminal \nthere is not enough space for elements in this page.\n and make sur page is runnig on fullscreen");
        getch();
        return -1;
    }

    int SCR_WIDTH, SCR_HEIGHT;

    getmaxyx(stdscr,SCR_HEIGHT, SCR_WIDTH);
    WINDOW *menu_win;
    int WIN_WIDHT = PGMW_WIDTH, WIN_HEIGH = PGMW_HEIGHT;
    int highlight = 1;
    int choice = 0;
    int c;
    int menu_top_margin = 10;
    initscr();

    clear();
    noecho();
    int WIN_START_X = (SCR_WIDTH - WIN_WIDHT) / 2;
    int WIN_START_Y = (SCR_HEIGHT - WIN_HEIGH) / 2;
    menu_win = newwin(WIN_HEIGH, WIN_WIDHT, WIN_START_Y, WIN_START_X);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use (arrow keys) to go up and down, Press (Enter) to select a choice.\n"
                   "Press Esc to quit..."); // attron(A_BLINK);//printw("(↑, ↓)");//attroff(A_BLINK);
    Print_Ascci_Art(menu_win, rogue_plus_text1, 8, TOP_ALIGNED, CENTER_ALIGNED, 1, 0);
    Print_Menu(menu_win, pgm_choices, pgm_choices_count, 1, menu_top_margin, CENTER_ALIGNED, 0, 1);
    refresh();
    while (1)
    {
        c = wgetch(menu_win);
        mvprintw(getmaxy(stdscr) - 1, 0, "%s", "                       ");
        clrtoeol();
        wrefresh(menu_win);
        switch (c)
        {
        case KEY_UP:
            if (highlight == 1)
                highlight = pgm_choices_count;
            else
                highlight--;
            break;

        case KEY_DOWN:
            if (highlight == pgm_choices_count)
                highlight = 1;
            else
                highlight++;
            break;

        case 10:
            choice = highlight;
            break;
        case 32:
            choice = highlight;
            break;
        case 27:
            choice = 7;
            break;
        default:
            refresh();
            mvprintw(getmaxy(stdscr) - 1, 0, "%s", "Invalid Key Pressed...");
            wrefresh(menu_win);
            break;
        }

        Print_Menu(menu_win, pgm_choices, pgm_choices_count, highlight, menu_top_margin, CENTER_ALIGNED, 0, 1);
        if (choice != 0)
        {
            if (guest)
            {
                switch (choice)
                {
                case 1:
                {
                    //newgmae
                }
                break;
                case 2:
                {
                    //scoreboard
                }
                break;
                case 3:
                {
                }
                break;
                case 4:
                {
                    //sign in
                }
                break;
                case 5:
                {
                    Exit_Program(0,NULL);
                }
                break;
                default:
                {
                    endwin();
                    printf("Not Possible wtf pgm <choice=%d>", choice);
                    Exit_Program(1,"Error in pre game menu choice");                
                }
                break;
                }
            }
            else
            {
                switch (choice)
                {
                case 1:
                {
                    //newgmae
                }
                break;
                case 2:
                {
                    //scoreboard
                }
                break;
                case 3:
                {
                }
                break;
                case 4:
                {
                    //sign in
                }
                break;
                case 5:
                {
                    Exit_Program(0,NULL);
                }
                break;
                default:
                {
                    endwin();
                    printf("Not Possible wtf pgm <choice=%d>", choice);
                    Exit_Program(1,"Error in pre game menu choice");                
                }
                break;
                }
            }
        }
    }
}