#include <deque>
#include "grid_ncurses.h"
enum class Ops {
    Fwd,
    Left,
    Right,
};
class Snake {
    private:
        std::deque<std::pair<int, int>> snakePosition;
        NcursesGrid grid;
        std::pair<int, int> foodxy;
        void nextStep(uint16_t key);
        void grow();
        int getInput();
    public:
        Snake();
        void draw(uint16_t ch);

        /* never return from this function */ 
        void gameLoop();
};

