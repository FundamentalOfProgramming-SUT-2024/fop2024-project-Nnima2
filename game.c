#include <ncurses.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
#include <stdbool.h>
#include "game.h"
#include "file_funcs.h"

#define MSG_HEIGHT 4
#define STATUS_WIDTH 35
#define ROOM_MIN_SIZE 5
#define ROOM_MAX_SIZE 15
#define MAX_ROOMS 10
#define MIN_ROOMS 5
#define MAX_MAP_SIZE 256
#define FLOOR_COUNT 256
#define MIN_DISTANCE_HALLWAYS 2

void init_windows();

typedef struct Point
{
    int x, y;
} point;

typedef struct Room
{
    int id;
    int x, y, width, height;
    int door_count;
    point *doors;
    bool has_connection;
    bool visited;
} room;

typedef struct Hallway
{
    point center;
    point *blocks[4];       // 0 up and saat gard ////block ha dar har jahat
    point *blocks_count[4]; // 0 up and saat gard /// tedad block dar har jahat
    int connected_room_count;
} hallway;

//   room1                               room3
//   room+17-16-15-14-0-1-2-3-4-5-6-7-8-9+oom3
//   room1            1                  room3
//                    2
//                    3
//                    4
//                  roo+2

wchar_t map[MAX_MAP_SIZE][MAX_MAP_SIZE];
bool map_see[MAX_MAP_SIZE][MAX_MAP_SIZE];
room rooms[MAX_ROOMS];
hallway *hallways;
static WINDOW *msg_win;
static WINDOW *status_win;
static WINDOW *map_win;
int map_height, map_width;
int num_room /*[4]*/; // tedad otagh dar in tabaghe
int num_hallways = 0;

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

int is_on_wall(point p)
{
    for (int i = 0; i < num_room; i++)
    {
        if ((p.x == rooms[i].x || p.x == rooms[i].x + rooms[i].width - 1) &&
            (p.y >= rooms[i].y && p.y <= rooms[i].y + rooms[i].height - 1))
            return 1;
        if ((p.y == rooms[i].y || p.y == rooms[i].y + rooms[i].height - 1) &&
            (p.x >= rooms[i].x && p.x <= rooms[i].x + rooms[i].width - 1))
            return 1;

        // if( (p.x>=rooms[i].x&&p.x<=rooms[i].x+rooms[i].x )&&
        //     (p.y>=rooms[i].y&&p.y<=rooms[i].y+rooms[i].y ))
        //     return 1;
    }
    return 0;
}

int add_hallway()
{
    int try = 0;

    num_hallways = 0;
    hallways = malloc(sizeof(hallway));

    map_height = getmaxy(stdscr) - MSG_HEIGHT - 2;
    map_width = getmaxx(stdscr) - STATUS_WIDTH - 2;

    int room_count = num_room;

    int first_hallway = 0;
    while (!first_hallway)
    {
        point new_center;
        do
        {
            new_center.x = rand() % (map_width - 1) + 1;
            new_center.y = rand() % (map_height - 1) + 1;
        } while (is_on_wall(new_center));

        int bad_place = 0;
        for (int i = 0; i < room_count; i++)
        {
            if ((new_center.x >= rooms[i].x && new_center.x <= rooms[i].x + rooms[i].width) &&
                (new_center.y >= rooms[i].y && new_center.y <= rooms[i].y + rooms[i].height))
            {
                bad_place = 1;
                break;
            }
        }
        // for (int i = 0; i < num_hallways; i++)
        // {
        //     if(abs(new_center.x-hallways[i].center.x)<3 ||abs(new_center.x-hallways[i].center.x)<3)
        //     {
        //         bad_place = 1;
        //         break;
        //     }
        // }
        if (bad_place)
        {
            continue;
        }

        bool is_room_in_each_direction[4] = {false}; // 0:up/1:right/2:down/3:left

        int upper_room_max_y = -1; // up

        int right_room_min_x = map_width + 2; // right

        int lowwer_room_min_y = map_height + 2; // down

        int left_room_max_x = -1; // left

        int nearest_room_index_each_diretion[4] = {-1};

        // TODO: find nearest room in each direction if exists
        for (int i = 0; i < room_count; i++)
        {
            if (rooms[i].y < new_center.y &&
                (new_center.x > rooms[i].x && new_center.x < rooms[i].x + rooms[i].width - 1))
            {
                if (rooms[i].y > upper_room_max_y)
                {
                    int path_is_clear = 1;
                    int x = new_center.x;
                    for (int y = new_center.y; y > rooms[i].y + rooms[i].height; y--)
                    {
                        if (map[y][x] == L'└' || map[y][x] == L'┘')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        upper_room_max_y = rooms[i].y;
                        is_room_in_each_direction[0] = TRUE;
                        nearest_room_index_each_diretion[0] = i;
                    }
                }
            }
            // 2:right
            // check if there is room right of new center;find nearest then
            if (rooms[i].x > new_center.x &&
                (new_center.y > rooms[i].y && new_center.y < rooms[i].y + rooms[i].height - 1))
            {
                // mvwprintw(status_win,20,3,"i see a room y:%d x:%d \n from center y:%d x:%d//i=%d",rooms[i].y,rooms[i].x,new_center.y,new_center.x,i);
                if (rooms[i].x < right_room_min_x)
                {
                    int path_is_clear = 1;
                    int y = new_center.y;
                    for (int x = new_center.x; x < rooms[i].x; x++)
                    {
                        if (map[y][x] == L'┌' ||
                            map[y][x] == L'└')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        right_room_min_x = rooms[i].x;
                        is_room_in_each_direction[1] = TRUE;
                        nearest_room_index_each_diretion[1] = i;
                    }
                }
                try++;
                if (try >= 10000)
                {
                    free(hallways);
                    new_game();
                }
            }
            // 3: down
            // check if there is room upon new center;find nearest then
            if (rooms[i].y > new_center.y &&
                (new_center.x > rooms[i].x && new_center.x < rooms[i].x + rooms[i].width - 1))
            {
                if (rooms[i].y < lowwer_room_min_y)
                {
                    int path_is_clear = 1;
                    int x = new_center.x;
                    for (int y = new_center.y; y < rooms[i].y; y++)
                    {
                        if (map[y][x] == L'┌' || map[y][x] == L'┐')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        lowwer_room_min_y = rooms[i].y;
                        is_room_in_each_direction[2] = TRUE;
                        nearest_room_index_each_diretion[2] = i;
                    }
                }
            }
            // 4:left
            // check if there is room right of new center;find nearest then
            if (rooms[i].x < new_center.x &&
                (new_center.y > rooms[i].y && new_center.y < rooms[i].y + rooms[i].height - 1))
            {

                if (rooms[i].x > left_room_max_x)
                {
                    int path_is_clear = 1;

                    int y = new_center.y;
                    for (int x = new_center.x; x > rooms[i].x + rooms[i].width - 1; x--)
                    {
                        if (map[y][x] == L'┐' ||
                            map[y][x] == L'┘')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        left_room_max_x = rooms[i].x;
                        nearest_room_index_each_diretion[3] = i;
                        is_room_in_each_direction[3] = TRUE;
                    }
                }
            }
        }

        int counter_4_jahat = 0;
        for (int i = 0; i < 4; i++) // 4 jahat
        {
            if (is_room_in_each_direction[i])
            {
                counter_4_jahat++;
            }
        }

        if (counter_4_jahat >= 2)
        {
            map[new_center.y][new_center.x] = L'#';
            first_hallway = 1;
            for (int i = 0; i < 4; i++)
            {
                if (is_room_in_each_direction[i])
                {
                    if (i == 0)
                    {
                        int x = new_center.x;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int y;
                        for (y = new_center.y; y > droom.y + droom.height - 1; y--)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 1)
                    {
                        int y = new_center.y;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int x;
                        for (x = new_center.x; x < droom.x; x++)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 2)
                    {
                        int x = new_center.x;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int y;
                        for (y = new_center.y; y < droom.y; y++)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 3)
                    {
                        int y = new_center.y;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int x;
                        for (x = new_center.x; x > droom.x + droom.width - 1; x--)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    rooms[nearest_room_index_each_diretion[i]].has_connection = TRUE;
                }
            }
            hallways[0].center = new_center;
            num_hallways++;
        }
    }

    bool all_rooms_has_connection = 0;
    while (!all_rooms_has_connection)
    {
        point new_center;
        new_center.x = rand() % (map_width - 1) + 1;
        new_center.y = rand() % (map_height - 1) + 1;

        int bad_place = 0;
        for (int i = 0; i < room_count; i++)
        {
            if ((new_center.x >= rooms[i].x && new_center.x <= rooms[i].x + rooms[i].width) &&
                (new_center.y >= rooms[i].y && new_center.y <= rooms[i].y + rooms[i].height))
            {
                bad_place = 1;
                break;
            }
        }

        for (int i = 0; i < num_hallways; i++)
        {
            if (abs(new_center.x - hallways[i].center.x) < MIN_DISTANCE_HALLWAYS || abs(new_center.y - hallways[i].center.y) < MIN_DISTANCE_HALLWAYS)
            {
                bad_place = 1;
                break;
            }
        }
        if (bad_place)
        {
            continue;
        }

        bool is_room_in_each_direction[4] = {false}; // 0:up/1:right/2:down/3:left

        int upper_room_max_y = -1; // up

        int right_room_min_x = map_width + 2; // right

        int lowwer_room_min_y = map_height + 2; // down

        int left_room_max_x = -1; // left

        int nearest_room_index_each_diretion[4] = {-1};

        // TODO: find nearest room in each direction if exists
        for (int i = 0; i < room_count; i++)
        {
            if (rooms[i].y < new_center.y &&
                (new_center.x > rooms[i].x && new_center.x < rooms[i].x + rooms[i].width - 1))
            {
                if (rooms[i].y > upper_room_max_y)
                {
                    int path_is_clear = 1;
                    int x = new_center.x;
                    for (int y = new_center.y; y > rooms[i].y + rooms[i].height; y--)
                    {
                        if (map[y][x] == L'└' || map[y][x] == L'┘')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        upper_room_max_y = rooms[i].y;
                        is_room_in_each_direction[0] = TRUE;
                        nearest_room_index_each_diretion[0] = i;
                    }
                }
            }
            // 2:right
            // check if there is room right of new center;find nearest then
            if (rooms[i].x > new_center.x &&
                (new_center.y > rooms[i].y && new_center.y < rooms[i].y + rooms[i].height - 1))
            {
                // mvwprintw(status_win,20,3,"i see a room y:%d x:%d \n from center y:%d x:%d//i=%d",rooms[i].y,rooms[i].x,new_center.y,new_center.x,i);
                if (rooms[i].x < right_room_min_x)
                {
                    int path_is_clear = 1;
                    int y = new_center.y;
                    for (int x = new_center.x; x < rooms[i].x; x++)
                    {
                        if (map[y][x] == L'┌' ||
                            map[y][x] == L'└')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        right_room_min_x = rooms[i].x;
                        is_room_in_each_direction[1] = TRUE;
                        nearest_room_index_each_diretion[1] = i;
                    }
                }
            }
            // 3: down
            // check if there is room upon new center;find nearest then
            if (rooms[i].y > new_center.y &&
                (new_center.x > rooms[i].x && new_center.x < rooms[i].x + rooms[i].width - 1))
            {
                if (rooms[i].y < lowwer_room_min_y)
                {
                    int path_is_clear = 1;
                    int x = new_center.x;
                    for (int y = new_center.y; y < rooms[i].y; y++)
                    {
                        if (map[y][x] == L'┌' || map[y][x] == L'┐')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        lowwer_room_min_y = rooms[i].y;
                        is_room_in_each_direction[2] = TRUE;
                        nearest_room_index_each_diretion[2] = i;
                    }
                }
            }
            // 4:left
            // check if there is room right of new center;find nearest then
            if (rooms[i].x < new_center.x &&
                (new_center.y > rooms[i].y && new_center.y < rooms[i].y + rooms[i].height - 1))
            {

                if (rooms[i].x > left_room_max_x)
                {
                    int path_is_clear = 1;

                    int y = new_center.y;
                    for (int x = new_center.x; x > rooms[i].x + rooms[i].width - 1; x--)
                    {
                        if (map[y][x] == L'┐' ||
                            map[y][x] == L'┘')
                        {
                            path_is_clear = 0;
                        }
                    }
                    if (path_is_clear)
                    {
                        left_room_max_x = rooms[i].x;
                        nearest_room_index_each_diretion[3] = i;
                        is_room_in_each_direction[3] = TRUE;
                    }
                }
            }
        }

        int counter_4_jahat = 0;
        bool exist_a_false_room = FALSE;
        bool exist_a_true_room = FALSE;
        for (int i = 0; i < 4; i++) // 4 jahat
        {
            if (is_room_in_each_direction[i])
            {
                counter_4_jahat++;
                if (rooms[nearest_room_index_each_diretion[i]].has_connection)
                {
                    exist_a_true_room = TRUE;
                }
                else
                {
                    exist_a_false_room = TRUE;
                }
            }
        }

        if (counter_4_jahat >= 2 && exist_a_false_room && exist_a_true_room)
        {
            map[new_center.y][new_center.x] = L'#';

            for (int i = 0; i < 4; i++)
            {
                if (is_room_in_each_direction[i])
                {
                    if (i == 0)
                    {
                        int x = new_center.x;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int y;
                        for (y = new_center.y; y > droom.y + droom.height - 1; y--)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 1)
                    {
                        int y = new_center.y;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int x;
                        for (x = new_center.x; x < droom.x; x++)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 2)
                    {
                        int x = new_center.x;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int y;
                        for (y = new_center.y; y < droom.y; y++)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }
                    else if (i == 3)
                    {
                        int y = new_center.y;
                        room droom = rooms[nearest_room_index_each_diretion[i]];
                        int x;
                        for (x = new_center.x; x > droom.x + droom.width - 1; x--)
                        {
                            map[y][x] = L'#';
                        }
                        map[y][x] = L'+';
                    }

                    rooms[nearest_room_index_each_diretion[i]].has_connection = TRUE;
                }
            }

            hallways = realloc(hallways, num_hallways + 1 * sizeof(hallway));
            hallways[num_hallways].center = new_center;
            num_hallways++;
        }

        all_rooms_has_connection = true;
        for (int i = 0; i < room_count; i++)
        {
            if (!rooms[i].has_connection)
            {
                all_rooms_has_connection = false;
            }
        }
    }
    return 1;
}

void generate_map()
{
    init_map();
    num_room = (rand() % MAX_ROOMS) + MIN_ROOMS + 1;
    int room_count = 0;

    srand(time(NULL));
    int map_height = getmaxy(stdscr) - MSG_HEIGHT - 2, map_width = getmaxx(stdscr) - STATUS_WIDTH - 2;

    while (room_count < num_room)
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
        // new_room.id = floor * 10 + room_count;
        new_room.door_count = 0;
        new_room.has_connection = FALSE;
        new_room.visited = FALSE;
        add_room(new_room);

        rooms[room_count] = new_room;
        room_count++;
    }
}


wchar_t hero = L'☻';
int hero_color = 2;
int emergancy_m = 0;
#pragma region draw🏰
void draw_map(WINDOW *win, int mh, int mw)
{
    for (int y = 0; y < mh; y++)
    {
        for (int x = 0; x < mw; x++)
        {
            int in_room = -1;
            for (int i = 0; i < num_room; i++)
            {
                if (
                    (x >= rooms[i].x && x <= rooms[i].x + rooms[i].width) &&
                    (y >= rooms[i].y && y <= rooms[i].y + rooms[i].height))
                {
                    in_room = i;
                    break;
                }
            }
            if (in_room != -1)
            {
                if (!rooms[in_room].visited && !emergancy_m)
                    wattron(map_win, A_INVIS);
            }
            else
            {
                wattroff(map_win, A_INVIS);
            }

            if (map[y][x] == L'#' && map_see[y][x] == 0)
            {
                if (!emergancy_m)
                {
                    wattron(map_win, A_INVIS);
                }
            }
            if (map[y][x] == hero)
            {
                wattron(map_win, COLOR_PAIR(hero_color));
            }

            if (emergancy_m)
            {
                wattroff(map_win, A_INVIS);
            }

            wchar_t str[2];
            str[0] = map[y][x];
            str[1] = L'\0';
            wmove(win, y, x);
            waddwstr(win, str);
            wattroff(map_win, A_INVIS);
            wattroff(map_win, COLOR_PAIR(hero_color));
            wrefresh(map_win);
        }
    }
    wrefresh(win);
}

void drawch(WINDOW *win, int y, int x, wchar_t ch)
{
    wchar_t str[2];
    str[0] = ch;
    str[1] = L'\0';
    wmove(win, y, x);
    waddwstr(win, str);
    wrefresh(win);
}
point current_cordination;


void massage(char *x)
{
    wclear(msg_win);

    wattron(msg_win, COLOR_PAIR(2));
    box(msg_win, 0, 0);
    mvwprintw(msg_win, 0, 2, " Message ");
    wattroff(msg_win, COLOR_PAIR(2));
    wrefresh(msg_win);
    mvwprintw(msg_win, 2, 2, x);
    wrefresh(msg_win);
}

int move_charechter(int y1, int x1, int y2, int x2, wchar_t *under_char)
{
    if (map[y2][x2] == L'.' || map[y2][x2] == L'#' || map[y2][x2] == L'+')
    {
        map[y1][x1] = *under_char;
        *under_char = map[y2][x2];
        map[y2][x2] = hero;
        // add color to charecter
        // atron()

        int in_room = -1;
        for (int i = 0; i < num_room; i++)
        {
            if (
                (x2 >= rooms[i].x && x2 <= rooms[i].x + rooms[i].width) &&
                (y2 >= rooms[i].y && y2 <= rooms[i].y + rooms[i].height))
            {
                in_room = i;
                break;
            }
        }

        if (rooms[in_room].visited == FALSE)
        {
            rooms[in_room].visited = TRUE;
            draw_map(map_win, map_height, map_width);
        }

        drawch(map_win, y1, x1, map[y1][x1]);

        wattron(map_win, COLOR_PAIR(hero_color));
        drawch(map_win, y2, x2, hero);
        wattroff(map_win, COLOR_PAIR(hero_color));

        map_see[y2][x2] = 1;
        return 1;
    }
    else
    {
        massage("can not move here           ");
        return 0;
    }
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
    int hallway_state = add_hallway();

    map_height = getmaxy(stdscr) - MSG_HEIGHT - 2, map_width = getmaxx(stdscr) - STATUS_WIDTH - 2;

#pragma region newgame
    point spawn_cord;
    rooms[0].visited = TRUE;
    for (int i = 0; i < MAX_MAP_SIZE; i++)
    {
        for (int j = 0; j < MAX_MAP_SIZE; j++)
        {
            map_see[j][i] = 0;
        }
    }
    spawn_cord.x = rooms[0].x + 1 + rand() % (rooms[0].width - 2);
    spawn_cord.y = rooms[0].y + 1 + rand() % (rooms[0].height - 2);

    wchar_t under_char = L'.';
    map[spawn_cord.y][spawn_cord.x] = hero;
    int x = spawn_cord.x, y = spawn_cord.y;
    char ch;
    draw_map(map_win, map_height, map_width);

    wattron(map_win, COLOR_PAIR(hero_color));
    drawch(map_win, y, x, hero);
    wattroff(map_win, COLOR_PAIR(hero_color));

    wrefresh(map_win);
    do
    {
        ch = getch();
        switch (ch)
        {
        case 'q':
        {
            int moved = move_charechter(y, x, y - 1, x - 1, &under_char);
            if (moved)
            {
                x--;
                y--;
            }
        }
        break;
        case 'w':
        {
            int moved = move_charechter(y, x, y - 1, x, &under_char);
            if (moved)
            {
                y--;
            }
        }
        break;
        case 'e':
        {
            int moved = move_charechter(y, x, y - 1, x + 1, &under_char);
            if (moved)
            {
                x++;
                y--;
            }
        }
        break;
        case 'a':
        {
            int moved = move_charechter(y, x, y, x - 1, &under_char);
            if (moved)
            {
                x--;
            }
        }
        break;
        case 's':
        {
            int moved = move_charechter(y, x, y + 1, x, &under_char);
            if (moved)
            {
                y++;
            }
        }
        break;
        case 'd':
        {
            int moved = move_charechter(y, x, y, x + 1, &under_char);
            if (moved)
            {
                x++;
            }
        }
        break;
        case 'z':
        {
            int moved = move_charechter(y, x, y + 1, x - 1, &under_char);
            if (moved)
            {
                x--;
                y++;
            }
        }
        break;
        case 'c':
        {
            int moved = move_charechter(y, x, y + 1, x + 1, &under_char);
            if (moved)
            {
                x++;
                y++;
            }
        }
        break;
        case 'm':
        {
            if (emergancy_m == 0)
            {
                emergancy_m = 1;
                massage("m is on");
                draw_map(map_win, map_height, map_width);
            }
            else
            {
                emergancy_m = 0;
                massage("m is off");
                draw_map(map_win, map_height, map_width);
            }
        }
        break;
        case 'f':
        {
            massage("In wich direction?");
            ch = getch();
            bool can_move = 1;
            bool fast_move = 0;
            do
            {
                switch (ch)
                {
                case 'q':
                {
                    int moved = move_charechter(y, x, y - 1, x - 1, &under_char);
                    if (moved)
                    {
                        x--;
                        y--;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'w':
                {
                    int moved = move_charechter(y, x, y - 1, x, &under_char);
                    if (moved)
                    {
                        y--;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'e':
                {
                    int moved = move_charechter(y, x, y - 1, x + 1, &under_char);
                    if (moved)
                    {
                        x++;
                        y--;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'a':
                {
                    int moved = move_charechter(y, x, y, x - 1, &under_char);
                    if (moved)
                    {
                        x--;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 's':
                {
                    int moved = move_charechter(y, x, y + 1, x, &under_char);
                    if (moved)
                    {
                        y++;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'd':
                {
                    int moved = move_charechter(y, x, y, x + 1, &under_char);
                    if (moved)
                    {
                        x++;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'z':
                {
                    int moved = move_charechter(y, x, y + 1, x - 1, &under_char);
                    if (moved)
                    {
                        x--;
                        y++;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                case 'c':
                {
                    int moved = move_charechter(y, x, y + 1, x + 1, &under_char);
                    if (moved)
                    {
                        x++;
                        y++;
                        fast_move = TRUE;
                    }
                    can_move = moved;
                }
                break;
                default:
                {
                    massage("wrong direction");
                    can_move = FALSE;
                }
                break;
                }
            } while (can_move);

            if (fast_move)
                wclear(msg_win);
            else
                massage("can not move in this direction");
        }
        break;
        default:
        {
            mvwprintw(msg_win, 2, 2, "Invalid key.what you wanna do?");
            wrefresh(msg_win);
        }
        break;
        }

        current_cordination.x = x;
        current_cordination.y = y;
    } while (ch != 27);
    refresh();
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