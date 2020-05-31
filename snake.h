#include <deque>
#include "grid.h"
enum class Ops {
    Fwd,
    Left,
    Right,
};
class Snake {
    private:
        std::deque<std::pair<int, int>> snakePosition;
        Grid grid;
        void nextStep(Ops op);
        void grow();
        int getInput();
    public:
        Snake();
        void draw();

        /* never return from this function */ 
        void gameLoop();
};

