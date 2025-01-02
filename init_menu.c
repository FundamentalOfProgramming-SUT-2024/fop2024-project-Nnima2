///*
#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include<form.h>
#include<ctype.h>

#define LEFT_ALIGNED 0
#define CENTER_ALIGNED 1
#define RIGHT_ALIGNED 2
#define TOP_ALIGNED 0
#define DOWN_ALIGNED 2

#define FM_CHOICES 3//first menu choices
#define FMW_WIDTH 65//first menu window
#define FMW_HEIGHT 20//first menu window

char *fm_choices[] = {
    "Login(already played)",
    "Sign up(new player)",
    "Exit",
};
char *rm_choices[] = {
    "Random Password",
    "Back",
};
int rm_choices_count =2;
char * rogue_plus_text[] = {
" _____                                         ",
"|  __ \\                                    _   ",
"| |__) |   ___     __ _   _   _    ___   _| |_ ",
"|  _  /   / _ \\   / _` | | | | |  / _ \\ |_   _|",
"| | \\ \\  | (_) | | (_| | | |_| | |  __/   |_|  ",
"|_|  \\_\\  \\___/   \\__, |  \\__,_|  \\___|        ",
"                   __/ |                       ",
"                  |___/                        "
};
int rogue_plus_text_lines = 8;
char * wellcome_text[]={
    " _    _        _  _                                _ ",
    "| |  | |      | || |                              | |",
    "| |  | |  ___ | || |  ___   ___   _ __ ___    ___ | |",
    "| |/\\| | / _ \\| || | / __| / _ \\ | '_ ` _ \\  / _ \\| |",
    "\\  /\\  /|  __/| || || (__ | (_) || | | | | ||  __/|_|",
    " \\/  \\/  \\___||_||_| \\___| \\___/ |_| |_| |_| \\___|(_)"                                          
};
int wellcome_text_lines = 6;


void Print_Ascci_Art(WINDOW*win,char* txt[],int txt_lines,int vertical_alignment
                    ,int side_alignment,int vertical_margin,int side_margin)
{
    int WIN_WIDTH=getmaxx(win),WIN_HEIGHT=getmaxy(win);
    int x,y;

    if(txt_lines>=WIN_HEIGHT-2)
    {
        perror("can not print text in this windows,Beacuse of Y");
        exit(1);
    }
    switch (vertical_alignment)
    {
        case TOP_ALIGNED:
                y = vertical_margin+1;
            break;
        case CENTER_ALIGNED:
                y = (WIN_HEIGHT-txt_lines)/2;
            break;
        case DOWN_ALIGNED:
                y = (WIN_HEIGHT-vertical_margin-txt_lines)-1;
            break;
        default:
            Print_Ascci_Art(win,txt,txt_lines,TOP_ALIGNED,side_alignment,side_margin,vertical_margin);
            break;
    }
    for (int i = 0; i < txt_lines; i++)
    {
        if(strlen(txt[i])>=WIN_WIDTH-2)
        {
            perror("can not print text in this windows,Beacuse of X");
            exit(1);
        }

        switch (side_alignment)
        {
            case LEFT_ALIGNED:{
                x = side_margin+1;
                
            }break;

            case CENTER_ALIGNED:{
                x = (WIN_WIDTH-strlen(txt[i]))/2;
            }break;

            case RIGHT_ALIGNED:{
                    x = WIN_WIDTH-strlen(txt[i])-side_margin-1;
            }break;

            default:{
                x = side_margin;
            }break;
        }

        mvwprintw(win,y,x,"%s",txt[i]);
        y++;
    }
    wrefresh(win);
}

void Print_Menu(WINDOW *menu_win,char * options[],int option_count,int highligh/*show which option is focused*/
                ,int top_margin,int side_alignment,int side_margin,int need_box)
{

    int x,y=top_margin,i;
    int WIN_WIDTH = getmaxx(menu_win);
    y+=1;
    if(need_box)    
        box(menu_win,0,0);
    refresh();

    for (int i = 0; i < option_count; i++)
    {
        switch (side_alignment)
        {
            case LEFT_ALIGNED:
                x= side_margin+1;
            break;
            case CENTER_ALIGNED:
                x= (WIN_WIDTH-strlen(options[i]))/2;
            break;
            case RIGHT_ALIGNED:
                x= WIN_WIDTH-strlen(options[i])-1;
            break;
            default:
                x=side_margin;
            break;
        }
        if(highligh == i+1)
        {
            wattron(menu_win,A_REVERSE);
            mvwprintw(menu_win,y,x,"%s",options[i]);
            wattroff(menu_win,A_REVERSE);
        } 
        else
            mvwprintw(menu_win,y,x,"%s",options[i]);
        y++;
    }
    wrefresh(menu_win);
}

int Setup_First_Menu()//returns 1 if already has an acount -->(login =1,sign up = 0)
{
    int SCR_WIDTH,SCR_HEIGHT;
    getmaxyx(stdscr,SCR_HEIGHT,SCR_WIDTH);
    WINDOW *menu_win;
    int WIN_WIDHT= FMW_WIDTH,WIN_HEIGH = FMW_HEIGHT;
    int highlight=1;
    int choice=0;
    int c;
    int menu_top_margin = rogue_plus_text_lines+6;
    initscr();

    if(SCR_WIDTH<2*WIN_HEIGH||SCR_HEIGHT<2*WIN_HEIGH)
    {//make sure screen has minimum size
        //resize_term(3*WIN_HEIGH,3*WIN_WIDHT);
        refresh();
        getmaxyx(stdscr, SCR_HEIGHT, SCR_WIDTH);
    }
    clear();
    noecho();
    int WIN_START_X = (SCR_WIDTH-WIN_WIDHT)/2;
    int WIN_START_Y = (SCR_HEIGHT-WIN_HEIGH)/2;
    menu_win = newwin(WIN_HEIGH,WIN_WIDHT,WIN_START_Y,WIN_START_X);
    keypad(menu_win,TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press (Enter/Space) to select a choice.\n"
    "Press Esc to quit...");//attron(A_BLINK);//printw("(↑, ↓)");//attroff(A_BLINK);
    Print_Ascci_Art(menu_win,rogue_plus_text,rogue_plus_text_lines,TOP_ALIGNED,CENTER_ALIGNED,1,0);
    Print_Menu(menu_win,fm_choices,FM_CHOICES,1,menu_top_margin,CENTER_ALIGNED,0,1);
    refresh();
    while(1)
    {
        c = wgetch(menu_win);
        mvprintw(getmaxy(stdscr)-1, 0, "%s","                       ");
        clrtoeol();
        wrefresh(menu_win);
        switch (c)
        {
            case KEY_UP:
                if(highlight==1)
                    highlight = FM_CHOICES;
                else
                    highlight--;
            break;

            case KEY_DOWN:
                if(highlight==FM_CHOICES)
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
                choice =3;
            break;
            default:
                mvprintw(getmaxy(stdscr)-1, 0, "%s","Invalid Key Pressed...");
				wrefresh(menu_win);
                break;
        }
        Print_Menu(menu_win,fm_choices,FM_CHOICES,highlight,menu_top_margin,CENTER_ALIGNED,0,1);
		if(choice != 0)
		{
            if(choice ==1)
            {
                return 1;
            }
            else if(choice==2)
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

/*
//line 1 for what your name
    char guide1[] ="--username can only contain (a-z/A-Z/1-9/(_))";
    mvwprintw(win,12,(WIN_WIDTH-strlen(guide1))/2,"%s",guide1);
    //last lines for guide

    char name_massage[] ="Enter Your Username";
    mvwprintw(win,1,(WIN_WIDTH-(strlen(name_massage)))/2,"%s",name_massage);
    wattr_on(win,A_UNDERLINE,NULL);
    char name[]= {"                        "}; 
    int name_textbox_startX = (WIN_WIDTH-24)/2;
    mvwprintw(win,3,name_textbox_startX,"%s",name);

    //line 3 for name text box
    

    int x = name_textbox_startX;
    int y= 3;
    wmove(win,y,x);
    int ch;
    
    while ((ch = wgetch(win)) != '\n')
    {
        switch (ch)
        {
            case 127:
            case KEY_BACKSPACE:
            {
                if(name_textbox_startX<x)
                {
                    mvwprintw(win,y,x-1," ");
                    wmove(win,y,--x);
                    name[x-name_textbox_startX] = '\0';

                }
            }break;
            default:
                if(x<name_textbox_startX+24&&((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||(ch=='_')))
                {
                    name[x-name_textbox_startX] =ch;
                    waddch(win,ch);
                    wmove(win,y,x+1);
                    x++;
                }
                break;
        }
        wrefresh(win);
    }
    name[x-name_textbox_startX] = '\0';
    mvprintw(10,0,"%s",name);


    char Password_massage[] ="Enter Your Password:";
    mvwprintw(win,5,(WIN_WIDTH-(strlen(name_massage)))/2,"%s",name_massage);
    wattr_on(win,A_UNDERLINE,NULL);
    char password[]= {"                        "}; 
    int name_textbox_startX = (WIN_WIDTH-24)/2;
    mvwprintw(win,7,name_textbox_startX,"%s",name);

    //line 3 for name text box
    

    int x = name_textbox_startX;
    int y= 3;
    wmove(win,y,x);
    int ch;
    
    while ((ch = wgetch(win)) != '\n')
    {
        switch (ch)
        {
            case 127:
            case KEY_BACKSPACE:
            {
                if(name_textbox_startX<x)
                {
                    mvwprintw(win,y,x-1," ");
                    wmove(win,y,--x);
                    name[x-name_textbox_startX] = '\0';

                }
            }break;
            default:
                if(x<name_textbox_startX+24&&((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||(ch=='_')))
                {
                    name[x-name_textbox_startX] =ch;
                    waddch(win,ch);
                    wmove(win,y,x+1);
                    x++;
                }
                break;
        }
        wrefresh(win);
    }
    name[x-name_textbox_startX] = '\0';
    mvprintw(10,0,"%s",name);
    wattr_off(win,A_UNDERLINE,NULL);


    getch();
    delwin(win);
    endwin();
*/
int Setup_Register_Page()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int WIN_WIDTH=60,WIN_HEIGH=20;
    WINDOW *win = newwin(WIN_HEIGH,WIN_WIDTH, (getmaxy(stdscr)-WIN_HEIGH)/2, (getmaxx(stdscr)-WIN_WIDTH)/2);
    box(win, 0, 0);
    refresh();
    wrefresh(win);

    
    //step1:fill page with blank labels

    char guide1[] ="--username can only contain (a-z/A-Z/1-9/(_))";
    mvwprintw(win,18,(WIN_WIDTH-strlen(guide1))/2,"%s",guide1);
    //guide label line 18

    char name_massage[] ="Enter Your Username:";
    mvwprintw(win,1,(WIN_WIDTH-(strlen(name_massage)))/2,"%s",name_massage);
    wattr_on(win,A_UNDERLINE,NULL);
    char name[]= {"                        "};//len = 24
    int name_maxlen = 24;
    int name_textbox_startX = (WIN_WIDTH-name_maxlen)/2;
    mvwprintw(win,3,name_textbox_startX,"%s",name);//name textbox
    wattr_off(win,A_UNDERLINE,NULL);
    //name labels line (1-3)

    char email_massage[] ="Enter Your Email:";
    mvwprintw(win,5,(WIN_WIDTH-(strlen(email_massage)))/2,"%s",email_massage);
    wattr_on(win,A_UNDERLINE,NULL);
    char email[]= {"                                        "};//len =40
    int email_maxlen = 40;
    int email_textbox_startX = (WIN_WIDTH-email_maxlen)/2;//name text_box
    mvwprintw(win,7,email_textbox_startX,"%s",email);//email textbox
    //email labels line (5-7)

    char password_massage[] ="Enter Your Password:";
    mvwprintw(win,9,(WIN_WIDTH-(strlen(password_massage)))/2,"%s",password_massage);
    wattr_on(win,A_UNDERLINE,NULL);
    char password[]= {"                                        "};//len =40
    int password_maxlen = 40;
    int password_textbox_startX = (WIN_WIDTH-password_maxlen)/2;//password text_box
    mvwprintw(win,11,password_textbox_startX,"%s",password);//password textbox
    //password labels line (9-11)
    
    wrefresh(win);
    int rm_top_margin =12;//register menu
    Print_Menu(win,rm_choices,rm_choices_count,0,rm_top_margin,CENTER_ALIGNED,0,0);
    //make text box as an underlined empty space
    //make it behave as menu
    //focus algorithm
    int x=name_textbox_startX,y=3;
    wmove(win,y,x);
    int focus =0;
    int highligh = 1;
    int ch;
    int name_x=name_textbox_startX;
    int pass_x= password_textbox_startX;
    int email_x=email_textbox_startX;
    while (1)
    {
        if((ch = wgetch(win)) == '\n')
        {exit(1);}
        if(ch==KEY_DOWN||ch==KEY_UP)
        {
           if(ch==KEY_DOWN)
                if(focus<=0)
                    focus=4;
                else
                    focus--;
            else if(ch==KEY_UP)
                if(focus<=0)
                    focus=4;
                else
                    focus--;
            switch (focus)
            {
                case 0:{
                    wmove(win,3,name_x);
                    highligh =0;
                }break;
                case 1:{
                    wmove(win,7,name_x);
                    highligh =0;
                }break;
                case 2:{
                    wmove(win,7,name_x);
                    highligh =0;
                }break;
                case 3:{
                    highligh =1;
                }break;
                case 4:{
                    highligh=2;
                }break;
            }
                Print_Menu(win,rm_choices,rm_choices_count,highligh,rm_top_margin,CENTER_ALIGNED,0,0);
        }
        switch (ch)
        {
            {case 127:
            case KEY_BACKSPACE:
            {
                if(name_textbox_startX<x)
                {
                    mvwprintw(win,y,x-1," ");
                    wmove(win,y,--x);
                    name[x-name_textbox_startX] = '\0';

                }
            }break;
            default:
                if(x<name_textbox_startX+24&&((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||(ch=='_')))
                {
                    name[x-name_textbox_startX] =ch;
                    waddch(win,ch);
                    wmove(win,y,x+1);
                    x++;
                }
                break;}
        }
        wrefresh(win);
    }
    getch();
}

int Setup_Login_Page()
{

}

int main()
{
    initscr();
    int has_acount = Setup_First_Menu();
    clear();
    if(has_acount ==-1)
    {
        endwin();
        return 0;
    }
    else if(has_acount)
    {
        Setup_Login_Page();
    }
    else
    {
        Setup_Register_Page();
    }
    //getch();
    endwin();
    return 0;

}

//*/