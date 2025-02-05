#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include "pregame_menus.h"
#include "ascii_graphics.h"
#include "global_vars.h"
#include "file_funcs.h"
#include "init_menus.h"
#include "scoreboard.h"
#include "file_funcs.h"
#define PGMW_WIDTH 65  // pregame menu window
#define PGMW_HEIGHT 20 // pregame menu window

typedef struct
{
    char username[64];
    char email[128];
    char password[64];
    int max_gold;
    int game_count;
    int total_score;
} Profile;

#define SETTINGS_DIR "setting/"
#define TOTAL_OPTIONS 4

typedef struct
{
    int difficulty; // 0 = easy 1 = meduim 2 =hard
    int music_enabled;
    int music_track;     // 1 = music1.mp3, 2 = music2.mp3, 3 = music3.mp3
    int character_color; // 1 = green 2 = blue 3 = red
} Settings;

void save_settings(const char *username, Settings *settings)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s.bin", SETTINGS_DIR, username);

    FILE *file = fopen(filepath, "wb");
    if (!file)
    {
        perror("Error saving settings");
        return;
    }

    fwrite(settings, sizeof(Settings), 1, file);
    fclose(file);
}

int load_settings(const char *username, Settings *settings)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s.bin", SETTINGS_DIR, username);

    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        settings->difficulty = 1;
        settings->music_enabled = 1;
        settings->music_track = 1;
        settings->character_color = 1;
        return 0;
    }

    fread(settings, sizeof(Settings), 1, file);
    fclose(file);
    return 1;
}

void show_settings()
{
    Settings settings;
    load_settings(username, &settings);

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    int selected_option = 0;
    int ch;

    while (1)
    {
        clear();
        setlocale(LC_ALL, "");
        mvaddwstr(1, 10, L"🎮 Game Settings");
        mvprintw(3, 5, "Use Up/Down Arrow To chose setting and Right/Left to change value");

        for (int i = 0; i < TOTAL_OPTIONS; i++)
        {
            if (i == selected_option)
            {
                attron(A_REVERSE);
            }

            switch (i)
            {
            case 0:
            {
                move(5, 10);
                addwstr(L"📌");
                printw(" Difficulity %s",
                       settings.difficulty == 0 ? "Easy" : settings.difficulty == 1 ? "Medium"
                                                                                    : "Hard");
            }
            break;
            case 1:
            {
                move(6, 10);
                addwstr(L"🎵");
                printw(" Play music: %s", settings.music_enabled ? "On" : "Off");
            }
            break;
            case 2:
            {
                move(7, 10);
                addwstr(L"🎼");
                printw(" Chosen music: music%d.mp3", settings.music_track);
            }
            break;
            case 3:
            {
                move(8, 10);
                addwstr(L"🎨 Character color: ");
                addstr(settings.character_color == 1 ? "Red" : settings.character_color == 2 ? "Blue"
                                                                                             : "Green");
            }
            break;
            }

            if (i == selected_option)
            {
                attroff(A_REVERSE);
            }
        }

        move(10, 10);
        addwstr(L"⚠ ");
        printw("Press Escape to return to menu...");

        refresh();
        ch = getch();

        switch (ch)
        {
        case KEY_UP:
            selected_option = (selected_option - 1 + TOTAL_OPTIONS) % TOTAL_OPTIONS;
            break;
        case KEY_DOWN:
            selected_option = (selected_option + 1) % TOTAL_OPTIONS;
            break;
        case KEY_LEFT:
            switch (selected_option)
            {
            case 0:
                settings.difficulty = (settings.difficulty + 2) % 3;
                break;
            case 1:
            {
                settings.music_enabled = !settings.music_enabled;
                if (settings.music_enabled)
                {
                    char current_music[256];
                    sprintf(current_music, "music/music_%d.mp3", settings.music_track);
                    play_music(current_music);
                }
                else
                {
                    stop_music();
                }
            }
            break;
            case 2:
            {
                settings.music_track = (settings.music_track + 1) % 3 + 1;

                if (settings.music_enabled)
                {
                    stop_music();
                    char current_music[256];
                    sprintf(current_music, "music/music_%d.mp3", settings.music_track);
                    play_music(current_music);
                }
                else
                {
                    stop_music();
                }
            }
            break;
            case 3:
                settings.character_color = ((settings.character_color + 1) % 3) + 1;
                break;
            }
            break;
        case KEY_RIGHT:
            switch (selected_option)
            {
            case 0:
                settings.difficulty = (settings.difficulty + 1) % 3;
                break;
            case 1:
            {
                settings.music_enabled = !settings.music_enabled;
                if (settings.music_enabled)
                {
                    char current_music[256];
                    sprintf(current_music, "music/music_%d.mp3", settings.music_track);
                    play_music(current_music);
                }
                else
                {
                    stop_music();
                }
            }
            break;
            case 2:
            {
                settings.music_track = (settings.music_track % 3) + 1;
                if (settings.music_enabled)
                {
                    stop_music();
                    char current_music[256];
                    sprintf(current_music, "music/music_%d.mp3", settings.music_track);
                    play_music(current_music);
                }
                else
                {
                    stop_music();
                }
            }
            break;
            case 3:
                settings.character_color = (settings.character_color % 3) + 1;
                break;
            }
            break;
        case 27:
            save_settings(username, &settings);
            clear();
            Open_Pregame_Menus();
            return;
        }
    }
}

void load_profile(Profile *user)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s.txt", USER_DIR, username);

    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        perror("Error opening user file");
        return;
    }

    strcpy(user->username, username);
    strcpy(user->email, "N/A");
    strcpy(user->password, "N/A");
    user->max_gold = 0;
    user->game_count = 0;
    user->total_score = 0;

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line + 5, "%s", user->username);
        }
        else if (strncmp(line, "Email:", 6) == 0)
        {
            sscanf(line + 6, "%s", user->email);
        }
        else if (strncmp(line, "Password:", 9) == 0)
        {
            sscanf(line + 9, "%s", user->password);
        }
        else if (strncmp(line, "max_gold:", 9) == 0)
        {
            sscanf(line + 9, "%d", &user->max_gold);
        }
        else if (strncmp(line, "game_count:", 11) == 0)
        {
            sscanf(line + 11, "%d", &user->game_count);
        }
        else if (strncmp(line, "total_score:", 12) == 0)
        {
            sscanf(line + 12, "%d", &user->total_score);
        }
    }

    fclose(file);
}

void show_profile()
{
    Profile user;
    load_profile(&user);

    initscr();
    setlocale(LC_ALL, "");
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    clear();
    attron(A_BOLD | COLOR_PAIR(1));
    move(2, 5);
    addwstr(L"🎮 Profile Page 🎮");
    attroff(A_BOLD | COLOR_PAIR(1));

    mvprintw(4, 5, "Username:   %s", user.username);
    mvprintw(5, 5, "Email:      %s", user.email);
    mvprintw(6, 5, "Password:   %s", user.password);
    mvprintw(8, 5, "Max Gold:   %d", user.max_gold);
    mvprintw(9, 5, "Games: %d   | Total Score: %d", user.game_count, user.total_score);
    mvprintw(11, 5, "Press any key to go back...");

    refresh();
    getch();
    clear();
    Open_Pregame_Menus();
}

int Open_Pregame_Menus()
{
    initscr();
    refresh();
    clear();
    if (guest)
        mvprintw(1, 1, "i playyed this game before you are %s", username);
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
        "Settings", // local for guests dont save
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
        state = -1;
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
                    delwin(menu_win);
                    clear();
                    refresh();
                    state =1;
                    return 0;
                }
                break;
                case 2:
                {
                    int player_count = 0;
                    score *scoreboard = Read_Scoreboard(&player_count);
                    display_scoreboard(scoreboard, player_count);
                }
                break;
                case 3:
                {
                    show_settings();
                }
                break;
                case 4:
                {
                    stop_music();
                    clear();
                    refresh();
                    main();
                }
                break;
                case 5:
                {
                    Exit_Program(0, NULL);
                }
                break;
                default:
                {
                    endwin();
                    printf("Not Possible wtf pgm <choice=%d>", choice);
                    Exit_Program(1, "Error in pre game menu choice");
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
                    delwin(menu_win);
                    clear();
                    refresh();
                    state = 1;
                    return 0;//new game;
                }
                break;
                case 2:
                {
                    delwin(menu_win);
                    clear();
                    refresh();
                    state = 2; //state 2 = continue game
                    return 1;  //coutinue
                }
                break;
                case 3:
                {
                    int player_count = 0;
                    score *scoreboard = Read_Scoreboard(&player_count);
                    display_scoreboard(scoreboard, player_count);
                }
                break;
                case 4:
                {
                    show_settings();
                }
                break;
                case 5:
                {
                    show_profile();
                }
                break;

                case 6:
                {
                    stop_music();
                    clear();
                    refresh();
                    int guest = 0;
                    memset(username, 0, MAX_USERNAME_LEN);
                    main();
                }
                case 7:
                {
                    Exit_Program(0, NULL);
                }
                break;
                default:
                {
                    endwin();
                    printf("Not Possible wtf pgm <choice=%d>", choice);
                    Exit_Program(1, "Error in pre game menu choice");
                }
                break;
                }
            }
        }
    }
}