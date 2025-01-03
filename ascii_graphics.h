#ifndef _ASCII_GRAPHICS_H_
#define _ASCII_GRAPHICS_H_
    #define LEFT_ALIGNED 0
    #define CENTER_ALIGNED 1
    #define RIGHT_ALIGNED 2
    #define TOP_ALIGNED 0
    #define DOWN_ALIGNED 2
    #include<ncurses.h>
    #include <ncurses.h>
    #include <string.h>
    #include <stdlib.h>
    void Print_Ascci_Art(WINDOW *win, char *txt[], int txt_lines, int vertical_alignment,
                int side_alignment, int vertical_margin, int side_margin);
            
    void Print_Menu(WINDOW *menu_win, char *options[], int option_count, int highligh /*show which option is focused*/,
                int top_margin, int side_alignment, int side_margin, int need_box);

#endif