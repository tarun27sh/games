/*
 * Grid draw could be printf, ncurses, sdl etc..
 */

#include<iostream>
#include "grid.h"

Grid::Grid () 
{
    /* randomly generate after 1st one gets eaten */
    this->foodLocation = std::make_pair(0,5);
    //fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}
void Grid::draw(int x, int y) {
    x = x%gridSize;
    y=y%gridSize;
    system("clear");
    for(int i=0; i< gridSize; ++i) {
        for(int j=0; j< gridSize; ++j) {
            if (i==x && j==y) {
                std::cout << " =>";
            } else
                std::cout << " . ";
        }
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
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
bool Grid::isFood(std::pair<int,int> objectLocation) {
    int x = objectLocation.first % gridSize;
    int y = objectLocation.second % gridSize;
    if ((foodLocation.first == x) && (foodLocation.second == y)) {
        this->foodLocation.first = getRandom() % gridSize;
        this->foodLocation.second = getRandom() % gridSize;
        return true;
    }
    return false;
}
void Grid::draw(std::deque<std::pair<int,int>> coor) {
    system("clear");
    for(int i=0; i< gridSize; ++i) {
        for(int j=0; j< gridSize; ++j) {
            if (isUserObjAtThisPoint(coor, i, j)) {
                std::cout << " =>";
            } else if (isFood(std::make_pair(i,j))) {
                std::cout << " FOOD ";
            } else {
                std::cout << " . ";
            }
        }
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
}
