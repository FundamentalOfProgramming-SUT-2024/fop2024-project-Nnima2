#include "ascii_graphics.h"

void Print_Ascci_Art(WINDOW *win, char *txt[], int txt_lines, int vertical_alignment, int side_alignment, int vertical_margin, int side_margin)
{
    int WIN_WIDTH = getmaxx(win), WIN_HEIGHT = getmaxy(win);
    int x, y;

    if (txt_lines >= WIN_HEIGHT - 2)
    {
        perror("can not print text in this windows,Beacuse of Y");
        exit(1);
    }
    switch (vertical_alignment)
    {
    case TOP_ALIGNED:
        y = vertical_margin + 1;
        break;
    case CENTER_ALIGNED:
        y = (WIN_HEIGHT - txt_lines) / 2;
        break;
    case DOWN_ALIGNED:
        y = (WIN_HEIGHT - vertical_margin - txt_lines) - 1;
        break;
    default:
        Print_Ascci_Art(win, txt, txt_lines, TOP_ALIGNED, side_alignment, side_margin, vertical_margin);
        break;
    }
    for (int i = 0; i < txt_lines; i++)
    {
        if (strlen(txt[i]) >= WIN_WIDTH - 2)
        {
            perror("can not print text in this windows,Beacuse of X");
            exit(1);
        }

        switch (side_alignment)
        {
        case LEFT_ALIGNED:
        {
            x = side_margin + 1;
        }
        break;

        case CENTER_ALIGNED:
        {
            x = (WIN_WIDTH - strlen(txt[i])) / 2;
        }
        break;

        case RIGHT_ALIGNED:
        {
            x = WIN_WIDTH - strlen(txt[i]) - side_margin - 1;
        }
        break;

        default:
        {
            x = side_margin;
        }
        break;
        }

        mvwprintw(win, y, x, "%s", txt[i]);
        y++;
    }
    wrefresh(win);
}

void Print_Menu(WINDOW *menu_win, char *options[], int option_count, int highligh /*show which option is focused*/,
                int top_margin, int side_alignment, int side_margin, int need_box)
{

    int x, y = top_margin, i;
    int WIN_WIDTH = getmaxx(menu_win);
    y += 1;
    if (need_box)
        box(menu_win, 0, 0);
    refresh();

    for (int i = 0; i < option_count; i++)
    {
        switch (side_alignment)
        {
        case LEFT_ALIGNED:
            x = side_margin + 1;
            break;
        case CENTER_ALIGNED:
            x = (WIN_WIDTH - strlen(options[i])) / 2;
            break;
        case RIGHT_ALIGNED:
            x = WIN_WIDTH - strlen(options[i]) - 1;
            break;
        default:
            x = side_margin;
            break;
        }
        if (highligh == i + 1)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", options[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", options[i]);
        y++;
    }
    wrefresh(menu_win);
}