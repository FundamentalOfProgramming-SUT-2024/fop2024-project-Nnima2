#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
#include "game.h"
#include "file_funcs.h"

#define MSG_HEIGHT 4
#define STATUS_WIDTH 35
#define ROOM_MIN_SIZE 5
#define ROOM_MAX_SIZE 15
#define MAX_ROOMS 10
#define MIN_ROOMS 5
#define MAX_MAP_SIZE 256

static WINDOW *msg_win;
static WINDOW *status_win;
static WINDOW *map_win;
int map_height, map_width;
int num_room; // tedad otagh dar in tabaghe

void init_windows();

typedef struct Room
{
    int id;
    int x, y, width, height;
    int door_count;
} room;

typedef struct Point
{
    int x,y;
}point;

typedef struct Hallway
{
    point center;
    point * blocks[4];//0 up and saat gard ////block ha dar har jahat
    point * blocks_count[4];//0 up and saat gard /// tedad block dar har jahat
    int connected_room_count;
}hallway;


//   room1                               room3
//   room+17-16-15-14-0-1-2-3-4-5-6-7-8-9+oom3
//   room1            1                  room3
//                    2 
//                    3        
//                    4
//                  roo+2

wchar_t map[MAX_MAP_SIZE][MAX_MAP_SIZE];

void init_map()
{
    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            map[y][x] = L' ';
        }
    }
}

void add_room(room added_room)
{
    for (int y = added_room.y; y < added_room.y + added_room.height; y++)
    {
        for (int x = added_room.x; x < added_room.x + added_room.width; x++)
        {

            if (y == added_room.y && x == added_room.x)
                map[y][x] = L'┌';
            else if (y == added_room.y && x == added_room.x + added_room.width - 1)
                map[y][x] = L'┐';
            else if (y == added_room.y + added_room.height - 1 && x == added_room.x)
                map[y][x] = L'└';
            else if (y == added_room.y + added_room.height - 1 && x == added_room.x + added_room.width - 1)
                map[y][x] = L'┘';

            else if (x == added_room.x || x == added_room.x + added_room.width - 1)
                map[y][x] = L'│';

            else if (y == added_room.y || y == added_room.y + added_room.height - 1)
                map[y][x] = L'─';

            else
                map[y][x] = L'.';
        }
    }
}

void add_hallway(int room_count)
{
    map_height = getmaxy(stdscr)-MSG_HEIGHT-2;
    map_width = getmaxx(stdscr)-STATUS_WIDTH-2;
    room_count = num_room;
    int first_hallway = 0;
    while(!first_hallway)
    {
        point rondom_hallway_center;
        int bad_place = 0;

    }
}

void generate_map()
{
    init_map();
    num_room = (rand() % MAX_ROOMS) + MIN_ROOMS + 1;
    room rooms[num_room];
    int room_count = 0;

    srand(time(NULL));
    int map_height = getmaxy(stdscr) - MSG_HEIGHT - 2, map_width = getmaxx(stdscr) - STATUS_WIDTH - 2;

    while (room_count < MAX_ROOMS)
    {
        room new_room;
        new_room.width = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        new_room.height = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        new_room.x = rand() % (map_width - new_room.width - 1) + 1;
        new_room.y = rand() % (map_height - new_room.height - 1) + 1;

        int tadakhol = 0;
        for (int i = 0; i < room_count; i++)
        {
            room other_room = rooms[i];

            if (
                (other_room.x < new_room.x + new_room.width + 3 && new_room.x < other_room.x + other_room.width + 3) &&
                (other_room.y < new_room.y + new_room.height + 3 && new_room.y < other_room.y + other_room.height + 3))
            {
                tadakhol = 1;
                break;
            }
        }

        if (tadakhol)
        {
            continue;
        }
        //new_room.id = floor * 10 + room_count;
        new_room.door_count = 0;
        add_room(new_room);

        if (room_count > 0)
        {
            // add_hallway(rooms[room_count - 1].x + rooms[room_count - 1].width / 2,
            //             rooms[room_count - 1].y + rooms[room_count - 1].height / 2,
            //             new_room.x + new_room.width / 2,
            //             new_room.y + new_room.height / 2);
        }

        rooms[room_count] = new_room;
        room_count++;
    }
}

#pragma region draw🏰
void draw_map(WINDOW *win, int mh, int mw)
{
    for (int y = 0; y < mh; y++)
    {
        for (int x = 0; x < mw; x++)
        {
            wchar_t str[2];
            str[0] = map[y][x];
            str[1] = L'\0';
            wmove(win, y, x);
            waddwstr(win, str);
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
    map_height = getmaxy(stdscr) - MSG_HEIGHT - 2, map_width = getmaxx(stdscr) - STATUS_WIDTH - 2;
    draw_map(map_win, map_height, map_width);
    wrefresh(map_win);
    refresh();
    getch();
    endwin();
    Exit_Program(0, NULL);
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

    wmove(status_win, 8, 2);
    waddstr(status_win, "Movement");
    wmove(status_win, 10, 2);
    waddwstr(status_win, L"Q  W  E");
    wmove(status_win, 11, 2);
    waddwstr(status_win, L" ↖ ↑ ↗ ");
    wmove(status_win, 12, 2);
    waddwstr(status_win, L"A← @ →D");
    wmove(status_win, 13, 2);
    waddwstr(status_win, L" ↙ ↓ ↘ ");
    wmove(status_win, 14, 2);
    waddwstr(status_win, L"Z  S  C");
    wattroff(status_win, COLOR_PAIR(3));

    wattron(status_win, COLOR_PAIR(6));
    mvwprintw(status_win, 5, 2, "Gold: 0&");
    wattroff(status_win, COLOR_PAIR(6));

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