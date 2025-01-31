#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "file_funcs.h"
#include "ascii_graphics.h"
#include "str_processing.h"
#include "init_menus.h"

#define FM_CHOICES 3  // first menu choices
#define FMW_WIDTH 65  // first menu window
#define FMW_HEIGHT 20 // first menu window
#define MAX_FILE_LINE 256
#define MAX_USER_PASS 100

int Init_Menus();

char *  username_ptr;
char final_username[MAX_USER_PASS];

char *fm_choices[] = {
    "Login+Guest",
    "Sign up(new player)",
    "Exit",
};
char *rm_choices[] = {
    "Random Password",
    "Submit",
    "Back",
};
int rm_choices_count = 3;

char *lm_choices[] = {
    "Login in as Guest",
    "Sign in",
    "Back",
};
int lm_choices_count = 3;
char *rogue_plus_text[] = {
    " _____                                         ",
    "|  __ \\                                    _   ",
    "| |__) |   ___     __ _   _   _    ___   _| |_ ",
    "|  _  /   / _ \\   / _` | | | | |  / _ \\ |_   _|",
    "| | \\ \\  | (_) | | (_| | | |_| | |  __/   |_|  ",
    "|_|  \\_\\  \\___/   \\__, |  \\__,_|  \\___|        ",
    "                   __/ |                       ",
    "                  |___/                        "};
int rogue_plus_text_lines = 8;

char *Register_text[] = {
    "______         __ _  _       _                ",
    "| ___ \\       / _` |(_)     | |               ",
    "| |_/ /  ___ | (_| | _  ___ | |_   ___  _ __  ",
    "|    /  / _ \\ \\__, || |/ __|| __| / _ \\| '__| ",
    "| |\\ \\ |  __/  __/ || |\\__ \\| |_ |  __/| |    ",
    "\\_| \\_| \\___| |___/ |_||___/ \\__| \\___||_|    "};
int register_text_lines = 6;

char *login_Text[] = {
    " _              __ _  _        ",
    "| |            / _` |(_)       ",
    "| |      ___  | (_| | _  _ __  ",
    "| |     / _ \\  \\__, || || '_ \\ ",
    "| |____| (_) |  __/ || || | | |",
    "\\_____/ \\___/  |___/ |_||_| |_|"

};
int login_text_lines = 6;
int Setup_First_Menu() // returns 1 if already has an acount -->(login =1,sign up = 0)
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
    int WIN_WIDHT = FMW_WIDTH, WIN_HEIGH = FMW_HEIGHT;
    int highlight = 1;
    int choice = 0;
    int c;
    int menu_top_margin = rogue_plus_text_lines + 6;
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
    Print_Ascci_Art(menu_win, rogue_plus_text, rogue_plus_text_lines, TOP_ALIGNED, CENTER_ALIGNED, 1, 0);
    Print_Menu(menu_win, fm_choices, FM_CHOICES, 1, menu_top_margin, CENTER_ALIGNED, 0, 1);
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
                highlight = FM_CHOICES;
            else
                highlight--;
            break;

        case KEY_DOWN:
            if (highlight == FM_CHOICES)
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
            choice = 3;
            break;
        default:
            mvprintw(getmaxy(stdscr) - 1, 0, "%s", "Invalid Key Pressed...");
            wrefresh(menu_win);
            break;
        }
        Print_Menu(menu_win, fm_choices, FM_CHOICES, highlight, menu_top_margin, CENTER_ALIGNED, 0, 1);
        if (choice != 0)
        {
            if (choice == 1)
            {
                return 1;
            }
            else if (choice == 2)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
    }
}

void Print_Error1(char *error_text)
{
    WINDOW *error_win = newwin(4, getmaxx(stdscr) - 8, getmaxy(stdscr) - 4, 5);
    box(error_win, 0, 0);
    mvwprintw(error_win, 2, 1, "%s", error_text);
    wrefresh(error_win);
    refresh();
}

int Setup_Register_Page()
{
    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 33)
    {
        mvprintw(10, 10, "pls zomeout and resize your terminal \nthere is not enough space for elements in this page.\n and make sur page is runnig on fullscreen");
        getch();
        return -1;//error
    }
#pragma region Reg_Lables
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int WIN_WIDTH = 70, WIN_HEIGH = 27;
    WINDOW *win = newwin(WIN_HEIGH, WIN_WIDTH, (getmaxy(stdscr) - WIN_HEIGH) / 2 + 4, (getmaxx(stdscr) - WIN_WIDTH) / 2);
    keypad(win, TRUE);
    box(win, 0, 0);
    refresh();
    wrefresh(win);

    // step1:fill page with blank labels

    char *rm_guides[] =
        {
            "--username is +3 chars including(a-z/A-Z/1-9/(_))--",
            "--Email shoud have this form [USER]@[DOMAIN].[TLD]--",
            "--password shoud contain:--",
            "--1.at least 7 charecter--",
            "--2.include at least one digit --",
            "--3.include CAPITAL and small letters--",
        };
    int rm_guides_lines = 6;
    Print_Menu(win, rm_guides, 6, 0, 19, CENTER_ALIGNED, 0, 0);
    // guide label line is printed down_aligned

    char name_massage[] = "Enter Your Username:";
    mvwprintw(win, 1, (WIN_WIDTH - (strlen(name_massage))) / 2, "%s", name_massage);
    wattr_on(win, A_UNDERLINE, NULL);
    char name[] = {"                        "}; // len = 24
    int name_maxlen = 24;
    int name_textbox_startX = (WIN_WIDTH - name_maxlen) / 2;
    mvwprintw(win, 3, name_textbox_startX, "%s", name); // name textbox
    wattr_off(win, A_UNDERLINE, NULL);
    // name labels line (1-3)

    char email_massage[] = "Enter Your Email:";
    mvwprintw(win, 5, (WIN_WIDTH - (strlen(email_massage))) / 2, "%s", email_massage);
    wattr_on(win, A_UNDERLINE, NULL);
    char email[] = {"                                        "}; // len =40
    int email_maxlen = 40;
    int email_textbox_startX = (WIN_WIDTH - email_maxlen) / 2; // name text_box
    mvwprintw(win, 7, email_textbox_startX, "%s", email);      // email textbox
    wattr_off(win, A_UNDERLINE, NULL);
    // email labels line (5-7)

    char password_massage[] = "Enter Your Password:";
    mvwprintw(win, 9, (WIN_WIDTH - (strlen(password_massage))) / 2, "%s", password_massage);
    wattr_on(win, A_UNDERLINE, NULL);
    char password[] = {"                                        "}; // len =40
    int password_maxlen = 40;
    int password_textbox_startX = (WIN_WIDTH - password_maxlen) / 2; // password text_box
    mvwprintw(win, 11, password_textbox_startX, "%s", password);     // password textbox
    wattr_off(win, A_UNDERLINE, NULL);
    // password labels line (9-11)

    wrefresh(win);
    int rm_top_margin = 12; // register menu
    Print_Menu(win, rm_choices, rm_choices_count, 0, rm_top_margin, CENTER_ALIGNED, 0, 0);
    // make text box as an underlined empty space
    // make it behave as menu
    // focus algorithm
    int x = name_textbox_startX, y = 3;
    wmove(win, y, x);
    int focus = 0;
    int highligh = 1;
    int ch;
    int name_x = name_textbox_startX;
    int password_x = password_textbox_startX;
    int email_x = email_textbox_startX;

    int name_str_len = 0;
    int password_str_len = 0;
    int email_str_len = 0;

#pragma endregion

    Print_Ascci_Art(stdscr, Register_text, register_text_lines, TOP_ALIGNED, CENTER_ALIGNED, 0, 0); // title

    name[0] = '\0';
    email[0] = '\0';
    password[0] = '\0';
    int submited = 0;
    while (!submited)
    {
        ch = wgetch(win);

        /*cleaning mess
        int temp_x,temp_y;
        getyx(win,temp_y,temp_x);
        char *rm_error1[] = {"                         "};
        Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
        Print_Menu(win, rm_guides, 6,0, 19, CENTER_ALIGNED, 0, 0);
        wmove(win,temp_y,temp_x);
        */

        if (ch == '\n') // enter-acctions
        {

            switch (focus)
            {
            case 0: // go to next textbox
            {
                highligh = 0;
                focus = 1;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 7, email_x);
            }
            break;
            case 1: // go to next textbox
            {
                highligh = 0;
                focus = 2;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 11, password_x);
            }
            break;
            case 2: // go to next radom pass gen button
            {
                focus = 3;
                highligh = 1;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 3: // random pass gen
            {
                srand(time(NULL));
                int length = 7;
                length += rand() % 23;
                char random_string[length + 1];
                random_string[length] = '\0';
                do
                {
                    Generate_Random_String(random_string, length);
                } while (Is_Password_Valid(random_string) != 1);
                wattr_on(win, A_UNDERLINE, NULL);
                memset(password, ' ', 39);
                password_x = password_textbox_startX;
                mvwprintw(win, 11, password_textbox_startX, "%s", password);
                mvwprintw(win, 11, password_textbox_startX, "%s", random_string);
                password_x = password_textbox_startX + strlen(random_string);
                focus = 2;
                highligh = 0;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 11, password_x);
                strcpy(password, random_string);
                //
            }
            break;
#pragma region validation
            case 4: // validating and filing
            {
                // is username valid?
                int password_case = Is_Password_Valid(password);
                char user_file_name[256];
                snprintf(user_file_name, sizeof(user_file_name), "users/%s.txt", name);
                if (!file_exists(user_file_name))
                {
                    mvprintw(0, (getmaxx(stdscr) - 24) / 2, "%s", "                       ");
                    refresh();
                }
                if (strlen(name) < 3)
                {
                    Print_Menu(win, rm_guides, 6, 1, 19, CENTER_ALIGNED, 0, 0);
                    focus = 0;
                    wmove(win, 3, name_x);
                }
                // is usernmae unique?
                else if (file_exists(user_file_name))
                {
                    attron(A_REVERSE);
                    mvprintw(0, (getmaxx(stdscr) - 24) / 2, "%s", "username already exists");
                    refresh();
                    attroff(A_REVERSE);
                    focus = 0;
                    wmove(win, 3, name_x);
                }
                // is email valid?
                else if (!Is_Email_Valid(email))
                {
                    Print_Menu(win, rm_guides, 6, 2, 19, CENTER_ALIGNED, 0, 0);
                    focus = 1;
                    wmove(win, 7, email_x);
                }
                // is password valid?
                else if (password_case != 1)
                {
                    if (password_case == 0)
                        Print_Menu(win, rm_guides, 6, 4, 19, CENTER_ALIGNED, 0, 0);
                    if (password_case == -1)
                        Print_Menu(win, rm_guides, 6, 5, 19, CENTER_ALIGNED, 0, 0);
                    if (password_case == -2)
                        Print_Menu(win, rm_guides, 6, 6, 19, CENTER_ALIGNED, 0, 0);
                    focus = 2;
                    wmove(win, 11, password_x);
                }
                else
                {
                    new_user_write_file(name, email, password);
                    Print_Error1("Registration Successfull,prees any key to go to next page");
                    getch();
                    delwin(win);
                    clear();
                    refresh();
                    return 1;//acount created
                }
            }
#pragma endregion
            break;
            case 5: // back
            {
                strcpy(final_username, name);
                wclear(win);
                clear();
                Init_Menus(username_ptr);
                return 0;//back normally
            }
            break;
            }
        }
        else if (ch == 27) // escape
        {
            wclear(win);
            clear();
            Init_Menus(username_ptr);
            return 0;//back normally
        }
        else if (ch == KEY_DOWN || ch == KEY_UP || ch == 9 /*Tab*/) // switch betweencontrols
        {
            if (ch == KEY_DOWN || ch == 9) // tab
            {
                focus = (focus + 1) % 6;
            }
            else if (ch == KEY_UP)
            {
                focus = (focus - 1 + 6) % 6;
            }
            // mvprintw(10, 10, "%d", focus);

            switch (focus)
            {
            case 0:
            {
                highligh = 0;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 3, name_x);
            }
            break;
            case 1:
            {
                highligh = 0;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 7, email_x);
            }
            break;
            case 2:
            {
                highligh = 0;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 11, password_x);
            }
            break;
            case 3:
            {
                highligh = 1;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 4:
            {
                highligh = 2;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 5:
            {
                highligh = 3;
                Print_Menu(win, rm_choices, rm_choices_count, highligh, rm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            }
        }
        else if (ch == KEY_BACKSPACE) // remove a char
        {
            switch (focus)
            {
            case 0:
            {
                y = 3;
                if (name_textbox_startX < name_x)
                {
                    wattron(win, A_UNDERLINE);
                    int space_holder_len = 24 - (name_x - name_textbox_startX);
                    char space_holder[space_holder_len + 1];
                    space_holder[space_holder_len] = '\0';
                    memset(space_holder, ' ', space_holder_len);

                    mvwprintw(win, y, name_x - 1, "%s", space_holder);
                    wmove(win, y, --name_x);
                    name[name_x - name_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    name_str_len = name_x - name_textbox_startX;
                }
                break;
            }
            break;
            case 1:
            {
                y = 7;
                wmove(win, 7, email_x);
                if (email_textbox_startX < email_x)
                {
                    wattron(win, A_UNDERLINE);
                    int space_holder_len = 40 - (email_x - email_textbox_startX);
                    char space_holder[space_holder_len + 1];
                    space_holder[space_holder_len] = '\0';
                    memset(space_holder, ' ', space_holder_len);
                    mvwprintw(win, y, email_x - 1, "%s", space_holder);
                    wmove(win, y, --email_x);
                    email[email_x - email_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    email_str_len = email_x - email_textbox_startX;
                }
            }
            break;
            case 2:
            {
                y = 11;
                wmove(win, 11, password_x);
                if (password_textbox_startX < password_x)
                {
                    wattron(win, A_UNDERLINE);
                    int space_holder_len = 40 - (password_x - password_textbox_startX);
                    char space_holder[space_holder_len + 1];
                    memset(space_holder, ' ', space_holder_len);
                    space_holder[space_holder_len] = '\0';
                    mvwprintw(win, y, password_x - 1, "%s", space_holder);
                    wmove(win, y, --password_x);
                    password[password_x - password_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    password_str_len = password_x - password_textbox_startX;
                }
            }
            break;
            case 3:
            case 4:
            case 5:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else if (ch == KEY_LEFT) // move back
        {
            switch (focus)
            {
            case 0:
            {
                y = 3;
                if (name_textbox_startX < name_x)
                {
                    wmove(win, y, --name_x);
                }
            }
            break;
            case 1:
            {
                y = 7;
                if (email_textbox_startX < email_x)
                {
                    wmove(win, y, --email_x);
                }
            }
            break;
            case 2:
            {
                y = 11;
                wmove(win, 11, password_x);
                if (password_textbox_startX < password_x)
                {
                    wmove(win, y, --password_x);
                }
            }
            break;
            case 3:
            case 4:
            case 5:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else if (ch == KEY_RIGHT) // move forward
        {
            switch (focus)
            {
            case 0:
            {
                y = 3;
                if (name_textbox_startX + 23 > name_x && name_x < name_str_len + name_textbox_startX)
                {
                    wmove(win, y, ++name_x);
                }
            }
            break;
            case 1:
            {
                y = 7;
                if (email_textbox_startX + 39 > email_x && email_x < email_str_len + email_textbox_startX)
                {
                    wmove(win, y, ++email_x);
                }
            }
            break;
            case 2:
            {
                y = 11;
                wmove(win, 11, password_x);
                if (password_textbox_startX + 39 > password_x && password_x < password_str_len + password_textbox_startX)
                {
                    wmove(win, y, ++password_x);
                }
            }
            break;
            case 3:
            case 4:
            case 5:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else // add char
        {
            switch (focus)
            {
            case 0:
            {
                if (name_x < name_textbox_startX + 23 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_')))
                {
                    wattron(win, A_UNDERLINE);
                    name[name_x - name_textbox_startX] = ch;
                    waddch(win, ch);
                    wmove(win, y, name_x + 1);
                    name_x++;
                    name[name_x - name_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    name_str_len++;
                }
                wmove(win, 3, name_x);
            }
            break;
            case 1:
            {
                y = 7;
                if (email_x < email_textbox_startX + 39 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_') || ispunct(ch)))
                {
                    wattron(win, A_UNDERLINE);
                    email[email_x - email_textbox_startX] = ch;
                    waddch(win, ch);
                    wmove(win, y, email_x + 1);
                    email_x++;
                    email[email_x - email_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    email_str_len++;
                }
                wmove(win, 7, email_x);
            }
            break;
            case 2:
            {
                y = 11;
                if (password_x < password_textbox_startX + 39 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_') || ispunct(ch)))
                {
                    wattron(win, A_UNDERLINE);
                    password[password_x - password_textbox_startX] = ch;
                    waddch(win, ch);
                    wmove(win, y, password_x + 1);
                    password_x++;
                    password[password_x - password_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    password_str_len++;
                }
                wmove(win, 11, password_x);
            }
            break;
            case 3:
            case 4:
            case 5:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }

        wrefresh(win);
    }
    delwin(win);
    endwin();
}

int Setup_Login_Page()// return 2 for guest / 1 for user / -1 for error 0 for exit normally
{
    if (getmaxx(stdscr) < 80 || getmaxy(stdscr) < 33)
    {
        mvprintw(10, 10, "pls zomeout and resize your terminal \nthere is not enough space for elements in this page.\n and make sur page is runnig on fullscreen");
        getch();
        return -1;
    }

#pragma region Log_Lables
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int WIN_WIDTH = 60, WIN_HEIGH = 18;
    WINDOW *win = newwin(WIN_HEIGH, WIN_WIDTH, (getmaxy(stdscr) - WIN_HEIGH) / 2 + 4, (getmaxx(stdscr) - WIN_WIDTH) / 2);
    keypad(win, TRUE);
    box(win, 0, 0);
    refresh();
    wrefresh(win);

    char name_massage[] = "Enter Your Username:";
    mvwprintw(win, 3, (WIN_WIDTH - (strlen(name_massage))) / 2, "%s", name_massage);
    wattr_on(win, A_UNDERLINE, NULL);
    char name[] = {"                        "}; // len = 24
    int name_maxlen = 24;
    int name_textbox_startX = (WIN_WIDTH - name_maxlen) / 2;
    mvwprintw(win, 5, name_textbox_startX, "%s", name); // name textbox
    wattr_off(win, A_UNDERLINE, NULL);
    // name labels line (1-3)

    char password_massage[] = "Enter Your Password:";
    mvwprintw(win, 7, (WIN_WIDTH - (strlen(password_massage))) / 2, "%s", password_massage);
    wattr_on(win, A_UNDERLINE, NULL);
    char password[] = {"                                        "}; // len =40
    int password_maxlen = 40;
    int password_textbox_startX = (WIN_WIDTH - password_maxlen) / 2; // password text_box
    mvwprintw(win, 9, password_textbox_startX, "%s", password);      // password textbox
    wattr_off(win, A_UNDERLINE, NULL);
    // password labels line (5-7)

    wrefresh(win);
    int lm_top_margin = 11; // register menu
    Print_Menu(win, lm_choices, lm_choices_count, 0, lm_top_margin, CENTER_ALIGNED, 0, 0);
    // make text box as an underlined empty space
    // make it behave as menu
    // focus algorithm
    int x = name_textbox_startX, y = 5;
    wmove(win, y, x);
    int focus = 0;
    int highligh = 1;
    int ch;
    int name_x = name_textbox_startX;
    int password_x = password_textbox_startX;

    int name_str_len = 0;
    int password_str_len = 0;
    int email_str_len = 0;
    Print_Ascci_Art(stdscr, login_Text, login_text_lines, TOP_ALIGNED, CENTER_ALIGNED, 2, 0); // title

#pragma endregion

    name[0] = '\0';
    password[0] = '\0';
    int submited = 0;
    while (!submited)
    {
        ch = wgetch(win);
        /*cleaning mess
        int temp_x,temp_y;
        getyx(win,temp_y,temp_x);
        char *rm_error1[] = {"                         "};
        Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
        Print_Menu(win, rm_guides, 6,0, 19, CENTER_ALIGNED, 0, 0);
        wmove(win,temp_y,temp_x);
        */
        if (ch == '\n') // enter-acctions
        {
            switch (focus)
            {
            case 0: // go to next textbox
            {
                highligh = 0;
                focus = 1;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 9, password_x);
            }
            break;
            case 1: // go to next textbox
            {
                highligh = 1;
                focus = 2;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 2: // config as a guest
            {
                if (strlen(name) == 0)
                {
                    Print_Error1("         pls fill the text boxes");
                }
                else
                {
                    char filename[MAX_FILE_LINE];
                    snprintf(filename, MAX_FILE_LINE, "users/Guest_%s.txt", name);

                    if (file_exists(filename))
                    {
                        Print_Error1("please choose another username");
                        refresh();
                    }
                    else
                    {

                        sprintf(final_username,"Guest_%s",name);
                        new_guest_write_file(name);
                        Print_Error1("Registration Successfull,prees any key to go to next page");
                        getch();
                        delwin(win);
                        clear();
                        refresh();
                        return 2;
                    }
                }
            }
            break;
            case 3: // validating and filing
            {
                if (strlen(name) == 0 || strlen(password) == 0)
                {
                    Print_Error1("         pls fill the text boxes");
                    refresh();
                }
                else
                {
#pragma region login validation

                    char filename[MAX_FILE_LINE];
                    snprintf(filename, MAX_FILE_LINE, "users/%s.txt", name);

                    if (file_exists(filename))
                    {
                        FILE *user_file = fopen(filename, "r");
                        char line[MAX_FILE_LINE];
                        char real_password[MAX_USER_PASS];
                        fgets(line, MAX_FILE_LINE, user_file);
                        fgets(line, MAX_FILE_LINE, user_file);
                        fgets(line, MAX_FILE_LINE, user_file);
                        char *fptr = strchr(line, ':');
                        fptr++;
                        strncpy(real_password, fptr, MAX_USER_PASS);
                        real_password[strcspn(real_password, "\n")] = '\0';
                        if (strcmp(real_password, password) == 0)
                        {
                            strcpy(final_username, name);
                            Print_Error1("Login Successfull.Press anykey to go to next page;");
                            getch();
                            delwin(win);
                            clear();
                            refresh();
                            return 1;
                        }
                        else
                        {
                            Print_Error1("Password Wrong Try again");
                        }
                        refresh();
                    }
                    else
                    {
                        char error414[MAX_FILE_LINE];
                        sprintf(error414, "Username <%s> is not found...", name);
                        Print_Error1(error414);
                        refresh();
                    }
#pragma endregion
                }
            }
            break;
            case 4:
            {
                strcpy(final_username, name);
                wclear(win);
                clear();
                Init_Menus(username_ptr);
                return 0;
            }
            break;
            }
        }
        else if (ch == 27) // escape
        {
            wclear(win);
            clear();
            Init_Menus(username_ptr);
            return 0;
        }
        else if (ch == KEY_DOWN || ch == KEY_UP || ch == 9 /*Tab*/) // switch betweencontrols
        {
            if (ch == KEY_DOWN || ch == 9) // tab
            {
                focus = (focus + 1) % 5;
            }
            else if (ch == KEY_UP)
            {
                focus = (focus - 1 + 5) % 5;
            }
            // mvprintw(10, 10, "%d", focus);

            switch (focus)
            {
            case 0:
            {
                highligh = 0;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 5, name_x);
            }
            break;
            case 1:
            {
                highligh = 0;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
                wmove(win, 9, password_x);
            }
            break;
            case 2:
            {
                highligh = 1;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 3:
            {
                highligh = 2;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            case 4:
            {
                highligh = 3;
                Print_Menu(win, lm_choices, lm_choices_count, highligh, lm_top_margin, CENTER_ALIGNED, 0, 0);
            }
            break;
            }
        }
        else if (ch == KEY_BACKSPACE) // remove a char
        {
            switch (focus)
            {
            case 0:
            {
                y = 5;
                if (name_textbox_startX < name_x)
                {
                    wattron(win, A_UNDERLINE);
                    int space_holder_len = 24 - (name_x - name_textbox_startX);
                    char space_holder[space_holder_len + 1];
                    space_holder[space_holder_len] = '\0';
                    memset(space_holder, ' ', space_holder_len);

                    mvwprintw(win, y, name_x - 1, "%s", space_holder);
                    wmove(win, y, --name_x);
                    name[name_x - name_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    name_str_len = name_x - name_textbox_startX;
                }
                break;
            }
            break;
            case 1:
            {
                y = 9;
                wmove(win, 9, password_x);
                if (password_textbox_startX < password_x)
                {
                    wattron(win, A_UNDERLINE);
                    int space_holder_len = 40 - (password_x - password_textbox_startX);
                    char space_holder[space_holder_len + 1];
                    memset(space_holder, ' ', space_holder_len);
                    space_holder[space_holder_len] = '\0';
                    mvwprintw(win, y, password_x - 1, "%s", space_holder);
                    wmove(win, y, --password_x);
                    password[password_x - password_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    password_str_len = password_x - password_textbox_startX;
                }
            }
            break;
            case 2:
            case 3:
            case 4:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else if (ch == KEY_LEFT) // move back
        {
            switch (focus)
            {
            case 0:
            {
                y = 5;
                wmove(win, 5, name_x);
                if (name_textbox_startX < name_x)
                {
                    wmove(win, y, --name_x);
                }
            }
            break;
            case 1:
            {
                y = 9;
                wmove(win, 9, password_x);
                if (password_textbox_startX < password_x)
                {
                    wmove(win, y, --password_x);
                }
            }
            break;
            case 2:
            case 3:
            case 4:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else if (ch == KEY_RIGHT) // move forward
        {
            switch (focus)
            {
            case 0:
            {
                y = 5;
                wmove(win, 5, name_x);
                if (name_textbox_startX + 23 > name_x && name_x < name_str_len + name_textbox_startX)
                {
                    wmove(win, y, ++name_x);
                }
            }
            break;
            case 1:
            {
                y = 9;
                wmove(win, 9, password_x);
                if (password_textbox_startX + 39 > password_x && password_x < password_str_len + password_textbox_startX)
                {
                    wmove(win, y, ++password_x);
                }
            }
            break;
            case 2:
            case 3:
            case 4:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }
        else // add char
        {
            switch (focus)
            {
            case 0:
            {
                y = 5;
                if (name_x < name_textbox_startX + 23 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_')))
                {
                    wattron(win, A_UNDERLINE);
                    name[name_x - name_textbox_startX] = ch;
                    waddch(win, ch);
                    wmove(win, y, name_x + 1);
                    name_x++;
                    name[name_x - name_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    name_str_len++;
                }
                wmove(win, 5, name_x);
            }
            break;
            case 1:
            {
                y = 9;
                if (password_x < password_textbox_startX + 39 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_') || ispunct(ch)))
                {
                    wattron(win, A_UNDERLINE);
                    password[password_x - password_textbox_startX] = ch;
                    waddch(win, ch);
                    wmove(win, y, password_x + 1);
                    password_x++;
                    password[password_x - password_textbox_startX] = '\0';
                    wattroff(win, A_UNDERLINE);
                    password_str_len++;
                }
                wmove(win, 9, password_x);
            }
            break;
            case 2:
            case 3:
            case 4:
            {
                // char *rm_error1[] = {"Invalid input for buttons"};
                // Print_Ascci_Art(stdscr, rm_error1, 1, DOWN_ALIGNED, LEFT_ALIGNED, 1, 1);
            }
            break;
            }
        }

        wrefresh(win);
    }
    delwin(win);
    endwin();
}

int Init_Menus(char * username)
{
    username_ptr = username;
    create_users_folder();
    initscr();
    int has_acount = Setup_First_Menu();
    clear();

    if (has_acount == -1)
    {
        endwin();
        exit(0);
    }
    else if (has_acount)
    {
        int login = Setup_Login_Page(); // 2==guest 1 = user 0 = not logined
        clear();
        refresh();
        if(login == -1||login == 0)
        {
            endwin();
            exit(0);
        }
        else if(login == 1||login == 2)
        {
            int guest = login -1;
            endwin();
            strcpy(username,final_username);
            return 0;
        }
    }
    else
    {
        int account_made = Setup_Register_Page();
        if(account_made ==0 || account_made == -1)
        {
            endwin();
            exit(0);
        }
        else
        {
            int guest = 0;
            clear();
            strcpy(username,final_username);
            return 0;
        }
    }

    clear();
    endwin();
    return 0;
}

//*/