/*
 * Grid draw could be printf, ncurses, sdl etc..
 */

#include<iostream>
#include "grid.h"
#include <ncurses.h>
#include<cstdint>

const int gridSize=50;
Grid::Grid () 
{
    /* randomly generate after 1st one gets eaten */
    //int _grid[gridSize][gridSize] = {'.'};
    this->foodLocation = std::make_pair(0,5);
    //fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    /* initialize ncurses */
}


bool Grid::isUserObjAtThisPoint(std::deque<std::pair<int,int>> coor, int i, int j) {
    for (auto& c: coor) {
        if ((c.first%gridSize == i) && (c.second%gridSize == j)) {
            return true;
        }
    }
    return false;
}

bool Grid::isFoodHere(int i, int j) {
    return false;
}

int Grid::getRandom() {
    return 0;
}

bool Grid::isFood(std::deque<std::pair<int,int>> snakexy, 
        std::pair<int,int> foodxy) 
{
    for (auto i: snakexy) {
        if (i.first == foodxy.first &&
                i.second == foodxy.second) {
            return true;
        }
    }
    return false;
}

/* grid with cout */
// make it virtual
// make child class for grid implementing these functions
void Grid::draw(std::deque<std::pair<int,int>> coor, uint8_t op) {
    system("clear");
    for(int i=0; i< gridSize; ++i) {
        for(int j=0; j< gridSize; ++j) {
            if (isUserObjAtThisPoint(coor, i, j)) {
                std::cout << " =>";
            //} else if (isFood(std::make_pair(i,j))) {
            //    std::cout << " FOOD ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
}

