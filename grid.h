#include<deque>
/*
 * Grid draw could be printf, ncurses, sdl etc..
 */
class Grid {
    private:
        constexpr static int gridSize = 40;
        int grid[gridSize][gridSize];
        std::pair<int, int> foodLocation;
    public:
        Grid ();
        void draw(int x, int y) ;
        bool isUserObjAtThisPoint(std::deque<std::pair<int,int>> coor, int i, int j);
        bool isFoodHere(int i, int j);
        int getRandom();
        bool isFood(std::pair<int,int> objectLocation);
        void draw(std::deque<std::pair<int,int>> coor);
};
