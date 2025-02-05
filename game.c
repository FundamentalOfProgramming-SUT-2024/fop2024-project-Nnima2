#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "game.h"

#define MSG_HEIGHT 4
#define STATUS_WIDTH 35
#define ROOM_MIN_SIZE 3
#define ROOM_MAX_SIZE 8
#define MAX_ROOMS 6
#define MAX_MAP_SIZE 256

static WINDOW *msg_win;
static WINDOW *status_win;
static WINDOW *map_win;
static int map_height,map_width;
void init_windows();

typedef struct {
    int x, y, width, height;
} Room;

char map[MAX_MAP_SIZE][MAX_MAP_SIZE];

void init_map() {
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            map[y][x] = '#';
        }
    }
}

void add_room(Room room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            map[y][x] = '.';
        }
    }
}

void add_hallway(int x1, int y1, int x2, int y2) {
    while (x1 != x2) {
        map[y1][x1] = '.';
        x1 += (x1 < x2) ? 1 : -1;
    }
    while (y1 != y2) {
        map[y1][x1] = '.';
        y1 += (y1 < y2) ? 1 : -1;
    }
}


void generate_map() {
    init_map();
    Room rooms[MAX_ROOMS];
    int room_count = 0;

    srand(time(NULL));

    while (room_count < MAX_ROOMS) {
        Room new_room;
        new_room.width = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        new_room.height = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        new_room.x = rand() % (map_width - new_room.width - 1) + 1;
        new_room.y = rand() % (map_height - new_room.height - 1) + 1;

        add_room(new_room);
        
        if (room_count > 0) { 
            add_hallway(rooms[room_count - 1].x + rooms[room_count - 1].width / 2, 
                        rooms[room_count - 1].y + rooms[room_count - 1].height / 2,
                        new_room.x + new_room.width / 2, 
                        new_room.y + new_room.height / 2);
        }
        
        rooms[room_count] = new_room;
        room_count++;
    }
}

// نمایش نقشه در ncurses
void draw_map(WINDOW *win) {
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            mvwaddch(win, y, x, map[y][x]);
        }
    }
    wrefresh(win);
}


int new_game()
{
    initscr();
    noecho();
    curs_set(FALSE);
    setlocale(LC_ALL, "");

    start_color();

    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_BLUE, -1);
    init_pair(4, COLOR_WHITE, -1);
    init_pair(5, COLOR_BLACK, -1);
    init_pair(6, COLOR_YELLOW, -1);
    init_pair(7, COLOR_CYAN, -1);
    init_pair(8, COLOR_MAGENTA, -1);

    init_windows();

    generate_map();
    draw_map(map_win);

    refresh();
    getch();
    endwin();
    return 0;
}

void init_windows()
{
    int height, width;
    getmaxyx(stdscr, height, width);

    int map_width = width - STATUS_WIDTH;
    int msg_y = height - MSG_HEIGHT;

    // Message Window
    msg_win = newwin(MSG_HEIGHT, width, msg_y, 0);
    wattron(msg_win, COLOR_PAIR(2));
    box(msg_win, 0, 0);
    mvwprintw(msg_win, 0, 2, " Message ");
    wattroff(msg_win, COLOR_PAIR(2));
    wrefresh(msg_win);

    // Status Window
    status_win = newwin(height - MSG_HEIGHT, STATUS_WIDTH, 0, width - STATUS_WIDTH);
    wattron(status_win, COLOR_PAIR(3));
    box(status_win, 0, 0);
    mvwprintw(status_win, 0, 2, " Status ");
    mvwprintw(status_win, 2, 2, "Health: 100%%");
    mvwprintw(status_win, 3, 2, "Hunger: 0");

    mvwprintw(status_win, 8, 2, "Guides:");

    wmove(status_win,8,2);
    waddstr(status_win,"Movement");
    wmove(status_win,10, 2);
    waddwstr(status_win,L"Q  W  E");
    wmove(status_win,11, 2);
    waddwstr(status_win,L" ↖ ↑ ↗ ");
    wmove(status_win,12, 2);
    waddwstr(status_win,L"A← @ →D");
    wmove(status_win,13, 2);
    waddwstr(status_win,L" ↙ ↓ ↘ ");
    wmove(status_win,14, 2);
    waddwstr(status_win,L"Z  S  C");
    wattroff(status_win, COLOR_PAIR(3));

    wattron(status_win,COLOR_PAIR(6));
    mvwprintw(status_win, 5, 2, "Gold: 0&");
    wattroff(status_win,COLOR_PAIR(6));

    wrefresh(status_win);
    

    // Map Window
    map_win = newwin(height - MSG_HEIGHT, map_width, 0, 0);
    wattron(map_win, COLOR_PAIR(1));
    box(map_win, 0, 0);
    mvwprintw(map_win, 0, 2, " Map ");
    wattroff(map_win, COLOR_PAIR(1));
    wrefresh(map_win);

    map_height = height - MSG_HEIGHT - 2;
    map_width = width - STATUS_WIDTH - 2; 
}