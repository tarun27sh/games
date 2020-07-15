/*
 * NcursesGrid
 *
 * TODO:
 *  1. initialize ncurses 
 *  2. link gameloop in game.h with ncurses loop
 *  3. destructor to cleanup
 *
 */

#include <stdio.h>
#include <time.h>
#include<iostream>
#include <stdint.h>


#include "grid_ncurses.h"
#include "logger.h"

const int window_width = 40;
const int window_height = 30;

void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void update_box(WIN *win, bool flag, std::deque<std::pair<int, int>>& xydeq, 
                std::pair<int, int>& foodxy);

void init_win_params(WIN *p_win)
{
    p_win->height = window_height;
    p_win->width  = window_width;
    p_win->starty = (LINES - p_win->height)/2;    
    p_win->startx = (COLS - p_win->width)/2;

    p_win->border.ls = '|';
    p_win->border.rs = '|';
    p_win->border.ts = '-';
    p_win->border.bs = '-';
    p_win->border.tl = '+';
    p_win->border.tr = '+';
    p_win->border.bl = '+';
    p_win->border.br = '+';

}
void print_win_params(WIN *p_win)
{
#ifdef _DEBUG
    mvprintw(25, 0, "%d %d %d %d", p_win->startx, p_win->starty, 
            p_win->width, p_win->height);
    //wrefresh(p_win);
    refresh();
#endif
}
void create_box(WIN *p_win, bool flag)
{    
    int i, j;
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    /* clear box */
    LOG("x=%d, y=%d, w=%d, h=%d\n", x,y,w,h);
    for(j = y; j <= y + h; ++j)
        for(i = x; i <= x + 2*w; ++i)
            mvaddch(j, i, ' ');

    if(flag == TRUE) {    
        /* draw character '+' at corners */
        mvaddch(y, x, p_win->border.tl);
        mvaddch(y, x + w, p_win->border.tr);
        mvaddch(y + h, x, p_win->border.bl);
        mvaddch(y + h, x + w, p_win->border.br);

        /* actual border */
        mvhline(y, x + 1, p_win->border.ts, 2*w - 1);
        mvhline(y + h, x + 1, p_win->border.bs, 2*w - 1);

        mvvline(y + 1, x, p_win->border.ls, h - 1);
        mvvline(y + 1, x + 2*w, p_win->border.rs, h - 1);
    }
    //wrefresh(p_win);
    refresh();
}

void 
update_box (WIN *p_win, bool flag, std::deque<std::pair<int, int>>& xydeq,
            std::pair<int, int>& foodxy)
{    
    char numbers[] = {'0','1','2','3','4','5','6','7','8','9'};
    //LOG("update\n");
    int i, j;
    int x, y, w, h;

    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    /* clear box, keep border */
    // or better clear deque points, and re create them
    for (auto i: xydeq) {
        LOG("____%d,%d\n", i.first, i.second);
    }
    LOG("x=%d, y=%d, w=%d, h=%d\n", x,y,w,h);
    for(j = y+1; j <= y + h - 1; ++j)
        for(i = x + 1; i <= x + 2*w -1 ; ++i)
            mvaddch(j, i, ' ');
    if(flag == TRUE) {    
        /* draw character ?? */
        int cntr=0;
        for (auto i: xydeq) {
            mvaddch(i.second, i.first, numbers[cntr++%10]);
        }
    }
    mvaddch(foodxy.second, foodxy.first, 'F');
    //LOG("refresh to update\n");
    refresh();
}

bool 
if_pulse() {
    //return false;
    return true;
}

/*
#define PRINT(...) 
*/
#define PRINT(...) do{\
    mvprintw(30,0, __VA_ARGS__); \
    refresh(); \
} while(0)

NcursesGrid::NcursesGrid () 
{
    /* randomly generate after 1st one gets eaten */
    //int _grid[gridSize][gridSize] = {'.'};
    this->foodLocation = std::make_pair(0,5);
    //fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    /* initialize ncurses */
    WIN *win = (WIN*) calloc(sizeof (WIN), 1);
    initscr();            /* Start curses mode         */
    start_color();            /* Start the color functionality */
    cbreak();            /* Line buffering disabled, Pass on
                          * everty thing to me         */
    keypad(stdscr, TRUE);        /* I need that nifty F1     */
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    curs_set(0);

    /* Initialize the window parameters */
    init_win_params(win);
    print_win_params(win);


    attron(COLOR_PAIR(1));
    printw("Press F1 to exit");
    //wrefresh(&win);
    refresh();
    attroff(COLOR_PAIR(1));

    create_box(win, TRUE);
    this->win = win;
}

NcursesGrid::~NcursesGrid () 
{
    endwin();            /* End curses mode          */
    free(this->win);
}


std::pair<int, int>
NcursesGrid::getRandomPoints()
{
    auto randx = rand()%this->win->width + this->win->startx ;
    auto randy = rand()% this->win->height + this->win->starty ;
    LOG("randome=(%d,%d)\n", randx, randy);
    return std::make_pair(randx, randy);
}


void NcursesGrid::draw(std::deque<std::pair<int,int>>& coor, uint16_t ch, 
                      std::pair<int,int>& foodxy) 
{
    WIN *win = this->win;
    // LOG("Ndraw, deq size=%lu\n", coor.size());
    //std::pair<int,int> init_value = coor.first();
    switch(ch)
    {    
        case KEY_LEFT:
            for (auto& i: coor) {
                if (i.first <= (win->startx + 1)) {
                    i.first = win->startx + 2*win->width - 1;
                } else {
                    --i.first;
                }
            }
            break;
        case KEY_RIGHT:
            for (auto& i: coor) {
                if (i.first >= (win->startx + 2*win->width - 1)) {
                    i.first = win->startx + 1;
                } else {
                    ++i.first;
                }
            }
            break;
        case KEY_DOWN:
            for (auto& i: coor) {
                if (i.second >= (win->starty + win->height -1)) {
                    i.second = win->starty + 1;
                } else {
                    ++i.second;
                }
            }
            break;
        case KEY_UP:
            for (auto& i: coor) {
                if (i.second <= (win->starty + 1)) {
                    i.second = win->starty +  win->height - 1;
                } else {
                    --i.second;
                }
            }
            break;    
        default:
            // handle
            LOG("unknown key\n");
            mvprintw(25, 0, "%d %d %d %d", win->startx, win->starty, 
                    win->width, win->height);
    }
    update_box(win, TRUE, coor, foodxy);
    LOG("key: %u\n", ch);
}
