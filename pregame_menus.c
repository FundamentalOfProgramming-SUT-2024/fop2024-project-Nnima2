#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pregame_menus.h"
#include "ascii_graphics.h"

#define PGM_CHOICES 7 // pregame menu choices
#define PGMW_WIDTH 65  // pregame menu window
#define PGMW_HEIGHT 20 // pregame menu window

char *pgm_choices[] = {//pregame_menu
    "New Game",
    "Continue Last Game",
    "Scoreboard",
    "Settings",
    "User Profile",
    "SignOut",
    "Exit",
};

char *rogue_plus_text1[] = {
    " _____                                         ",
    "|  __ \\                                    _   ",
    "| |__) |   ___     __ _   _   _    ___   _| |_ ",
    "|  _  /   / _ \\   / _` | | | | |  / _ \\ |_   _|",
    "| | \\ \\  | (_) | | (_| | | |_| | |  __/   |_|  ",
    "|_|  \\_\\  \\___/   \\__, |  \\__,_|  \\___|        ",
    "                   __/ |                       ",
    "                  |___/                        "};


int Open_Pregame_Menus(char * username,int geust)
{
    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 33)
    {
        mvprintw(10, 10, "pls zomeout and resize your terminal \nthere is not enough space for elements in this page.\n and make sur page is runnig on fullscreen");
        getch();
        return -1;
    }

    int SCR_WIDTH, SCR_HEIGHT;
    getmaxyx(stdscr, SCR_HEIGHT, SCR_WIDTH);
    WINDOW *menu_win;
    int WIN_WIDHT = PGMW_WIDTH, WIN_HEIGH = PGMW_HEIGHT;
    int highlight = 1;
    int choice = 0;
    int c;
    int menu_top_margin = 10;
    initscr();

    if (SCR_WIDTH < 2 * WIN_HEIGH || SCR_HEIGHT < 2 * WIN_HEIGH)
    { // make sure screen has minimum size
        // resize_term(3*WIN_HEIGH,3*WIN_WIDHT);
        refresh();
        getmaxyx(stdscr, SCR_HEIGHT, SCR_WIDTH);
    }
    clear();
    noecho();
    int WIN_START_X = (SCR_WIDTH - WIN_WIDHT) / 2;
    int WIN_START_Y = (SCR_HEIGHT - WIN_HEIGH) / 2;
    menu_win = newwin(WIN_HEIGH, WIN_WIDHT, WIN_START_Y, WIN_START_X);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use (arrow keys) to go up and down, Press (Enter) to select a choice.\n"
                   "Press Esc to quit..."); // attron(A_BLINK);//printw("(↑, ↓)");//attroff(A_BLINK);
    Print_Ascci_Art(menu_win, rogue_plus_text1, 8, TOP_ALIGNED, CENTER_ALIGNED, 1, 0);
    Print_Menu(menu_win, pgm_choices, PGM_CHOICES, 1, menu_top_margin, CENTER_ALIGNED, 0,1);
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
                highlight = PGM_CHOICES;
            else
                highlight--;
            break;

        case KEY_DOWN:
            if (highlight == PGM_CHOICES)
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

        Print_Menu(menu_win,pgm_choices, PGM_CHOICES, highlight, menu_top_margin, CENTER_ALIGNED, 0, 1);
        if (choice != 0)
        {
            return choice;
        }
    }
}