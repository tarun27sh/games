#include<deque>
#include<cstdint>
/*
 * Grid draw could be printf, ncurses, sdl etc..
 */
class Grid {
    protected:
        constexpr static int gridSize = 40;
        std::pair<int, int> foodLocation;
        int grid[gridSize][gridSize];
    public:
        Grid ();
        bool isUserObjAtThisPoint(std::deque<std::pair<int,int>> coor, int i, int j);
        bool isFoodHere(int i, int j);
        int getRandom();
        bool isFood(std::pair<int,int> objectLocation);
        virtual void draw(std::deque<std::pair<int,int>> coor, uint8_t op);
};
