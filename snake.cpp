#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>
using namespace std;


/*
 * Grid draw could be printf, ncurses, sdl etc.. 
 */
class Grid {
    private:
        constexpr static int gridSize = 40;
        int grid[gridSize][gridSize];
        pair<int, int> foodLocation;
    public:
        Grid () {
            /* randomly generate after 1st one gets eaten */
            this->foodLocation = make_pair(0,5);
            //fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
        }
        void draw(int x, int y) {
            x = x%gridSize;
            y=y%gridSize;
            system("clear");
            for(int i=0; i< gridSize; ++i) {
                for(int j=0; j< gridSize; ++j) {
                    if (i==x && j==y) {
                        cout << " =>";
                    } else
                        cout << " . ";
                }
                cout <<  endl;
            }
            cout <<  endl;
        }
        bool isUserObjAtThisPoint(deque<pair<int,int>> coor, int i, int j) {
            for (auto& c: coor) {
                if ((c.first%gridSize == i) && (c.second%gridSize == j)) {
                    return true;
                }
            }
            return false;
        }
        bool isFoodHere(int i, int j) {
            
        }
        int getRandom() {
            return 0;
        }
        bool isFood(pair<int,int> objectLocation) {
            int x = objectLocation.first % gridSize;
            int y = objectLocation.second % gridSize;
            if ((foodLocation.first == x) && (foodLocation.second == y)) {
                this->foodLocation.first = getRandom() % gridSize;
                this->foodLocation.second = getRandom() % gridSize;
                return true;
            }
            return false;
        }
        void draw(deque<pair<int,int>> coor) {
            system("clear");
            for(int i=0; i< gridSize; ++i) {
                for(int j=0; j< gridSize; ++j) {
                    if (isUserObjAtThisPoint(coor, i, j)) {
                        cout << " =>";
                    } else if (isFood(make_pair(i,j))) {
                        cout << " FOOD ";
                    } else {
                        cout << " . ";
                    }
                }
                cout <<  endl;
            }
            cout <<  endl;
        }
};

enum class Ops {
    Fwd,
    Left,
    Right,
};
class Snake {
    private:
        deque<pair<int, int>> snakePosition;
        Grid grid;
        void nextStep(Ops op) {
            switch(op) {
                case Ops::Fwd: {
                                   auto head = this->snakePosition.front();
                                   this->snakePosition.pop_back();
                                   this->snakePosition.push_front(make_pair(head.first, head.second+1));
                                   break;
                               }
                case Ops::Right: {
                                     auto head = this->snakePosition.front();
                                     this->snakePosition.pop_back();
                                     this->snakePosition.push_front(make_pair(head.first+1, head.second));
                                     break;
                                 }
                case Ops::Left: {
                                    auto head = this->snakePosition.front();
                                    this->snakePosition.pop_back();
                                    this->snakePosition.push_front(make_pair(head.first-1, head.second));
                                    break;
                                }
                default:
                                printf("invalid move\n");
            }

            this->draw();
            if (grid.isFood(snakePosition[0]))
                this->grow();
        }
        void grow() {
            pair<int,int> lastDotLoc = snakePosition[snakePosition.size() -1];
            snakePosition.push_back(make_pair(lastDotLoc.first, lastDotLoc.second+1));
        }
        int getInput() {
            // read stdin char
            // reset event
            int ch = 0;
            if (read(STDIN_FILENO, &ch, 1) == -1) {
                perror("read error");
            }
            printf("read: %d\n", ch);
            return ch;
        }
    public:
        Snake() {
            snakePosition.push_back(make_pair(0, 0));
        }
        void draw() {
            grid.draw(this->snakePosition);
        }

        /* never return from this function */ 
        void gameLoop() {
            int key;
#define MAX_EVENTS 2
            struct epoll_event ev, events[MAX_EVENTS];
            int epollfd, nfds, timer_fd;
            epollfd = epoll_create1(0);
            if (epollfd == -1) {
                perror("epoll_create1");
                exit(1);
            }

            /* user input */
            ev.events = EPOLLIN;
            ev.data.fd = STDIN_FILENO;
            //int flags = fcntl(ev.data.fd, F_GETFL, 0);
            //fcntl(ev.data.fd, F_SETFL, flags | O_NONBLOCK);
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
                perror("epoll_create1");
                exit(1);
            }

            /* game clock */
            struct itimerspec ts;
            timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
            if (timer_fd == -1) {
                perror("timer_fd");
                exit(1);
            }
            ev.events = EPOLLIN;
            ev.data.fd = timer_fd;
            //int flags = fcntl(ev.data.fd, F_GETFL, 0);
            //fcntl(ev.data.fd, F_SETFL, flags | O_NONBLOCK);
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, timer_fd, &ev) == -1) {
                perror("epoll_create1");
                exit(1);
            }
            int time_in_sec = 1;
            ts.it_interval.tv_sec = time_in_sec;
            ts.it_interval.tv_nsec = 0;
            ts.it_value.tv_sec = time_in_sec;
            ts.it_value.tv_nsec = 0;
            if (timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &ts, NULL) == -1) {
                perror("timer_fd settime");
                exit(1);
            }

            /* requires user to press arrow and then enter */
            printf("start game!!!\n");
            bool clock_ev = false;
            while (true) {
                //usleep(100000);
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                for (auto n = 0; n < nfds; ++n) {
                    if (events[n].data.fd == STDIN_FILENO) {
                        printf("input detected!\n");  // yes, else epoll keeps on notifying :)
                        key = getInput();
                        if(key == 68) {         // left
                            this->nextStep(Ops::Left);
                        } else if (key == 67) {        // right
                            this->nextStep(Ops::Right);
                        } else {
                            this->nextStep(Ops::Fwd);
                        }
                        printf("keyboard event\n");
                    } else if (events[n].data.fd == timer_fd) { 
                        this->nextStep(Ops::Fwd);
                        printf("clock event\n");
                    }
                    printf("#events=%d\n", nfds);
                }
            }

        }
};

int main() {
    Snake s;
    s.gameLoop();
    return 0;
}
