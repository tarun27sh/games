#include<deque>
#include"grid.h"
#include<stdint.h>
#include<ncurses.h>
/*
 * Grid draw could be printf, ncurses, sdl etc..
 */


typedef struct _win_border_struct {
    chtype     ls, rs, ts, bs,
               tl, tr, bl, br;
}WIN_BORDER;

/* ncurses window */
typedef struct _WIN_struct {
    int startx, starty;
    int height, width;
    WIN_BORDER border;
}WIN;

class NcursesGrid: public Grid {
    private:
        WIN *win;
    public:
        NcursesGrid ();
        ~NcursesGrid ();
        std::pair<int, int> getRandomPoints();
        void draw(std::deque<std::pair<int,int>>& coor, uint16_t ch, std::pair<int,int>& foodxy);
};
