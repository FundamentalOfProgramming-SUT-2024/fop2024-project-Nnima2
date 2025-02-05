#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#define SCOREBOARD_PATH "scoreboard.bin"
#define TEMP_SCOREBOARD_PATH "scoreboard-temp.bin"
#define ITEMS_PER_PAGE 5

typedef struct ScoreBoard_Tuple
{
    char username[256];
    int score;  
    int gold_collected;
    // int rank; it makes my job harder
    int game_count;
    int time_since_first_game;
} score;

score *Read_Scoreboard(int *player_count_ptr);

void write_scoreboard(score *score_list, int player_count);

void Insert_To_Scoreboard(score current_score);

void display_scoreboard(score *scoreboard, int player_count);
#endif