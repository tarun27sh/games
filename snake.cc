#include "./snake.h"
#include "logger.h"

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <deque>
#include <ncurses.h>

Snake::Snake() {
    snakePosition.push_back(grid.getRandomPoints());
    snakePosition.push_back(std::make_pair(snakePosition[0].first-1, snakePosition[0].second));
    snakePosition.push_back(std::make_pair(snakePosition[0].first-2, snakePosition[0].second));
    snakePosition.push_back(std::make_pair(snakePosition[0].first-3, snakePosition[0].second));
    snakePosition.push_back(std::make_pair(snakePosition[0].first-4, snakePosition[0].second));
    snakePosition.push_back(std::make_pair(snakePosition[0].first-5, snakePosition[0].second));
    snakePosition.push_back(std::make_pair(snakePosition[0].first-6, snakePosition[0].second));
    this->foodxy = std::make_pair(snakePosition[0].first + 3, snakePosition[0].second);
}

/* 
 * Based on the key, change snake shape and
 * pass on the grid for print 
 */
void Snake::nextStep(uint16_t key) {
    auto head = this->snakePosition.front();
    auto tail = this->snakePosition.back();
    switch (key) {
        case KEY_UP: {
                            this->snakePosition.pop_front();
                            this->snakePosition.push_back(
                            std::make_pair(tail.first, tail.second-1));
                         /*
                             this->snakePosition.pop_back();
                             this->snakePosition.push_front(
                                    std::make_pair(head.first, head.second+1));
                            */
                           break;
                       }
        case KEY_DOWN: {
                           this->snakePosition.pop_front();
                           this->snakePosition.push_back(
                                    std::make_pair(tail.first, tail.second+1));
                           /*
                             this->snakePosition.pop_back();
                             this->snakePosition.push_front(
                                    std::make_pair(head.first, head.second-1));
                                    */
                           break;
                       }
        case KEY_RIGHT: {
                             this->snakePosition.pop_back();
                             this->snakePosition.push_front(
                                    std::make_pair(head.first+1, head.second));
                             break;
                         }
        case KEY_LEFT: {
                            this->snakePosition.pop_back();
                            this->snakePosition.push_front(
                                    std::make_pair(head.first-1, head.second));
                            break;
                        }
        default:
                       return;
    }

    this->draw(key); // use enum, expose to grid_ncruses.h
    if (grid.isFood(this->snakePosition, this->foodxy)) {
        this->grow();
        // update this->food locaiton to new x,y - make random intead
        this->foodxy = 
            grid.getRandomPoints();
    }
}

void Snake::grow() {
    std::pair<int, int> lastDotLoc =
                    this->snakePosition[this->snakePosition.size() -1];
    this->snakePosition.push_back(std::make_pair(lastDotLoc.first,
                            lastDotLoc.second+1));
}

int Snake::getInput() {
    // read stdin char
    // reset event
    int ch = 0;
    if (read(STDIN_FILENO, &ch, 1) == -1) {
        perror("read error");
    }
    std::cout << "read: " << ch <<  std::endl;
    return ch;
}


void Snake::draw(uint16_t ch) {
    //LOG("Sdraw\n");
    grid.draw(this->snakePosition, ch, this->foodxy);
}

/* never return from this function */
void Snake::gameLoop() {
    uint16_t key;
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
    // int flags = fcntl(ev.data.fd, F_GETFL, 0);
    // fcntl(ev.data.fd, F_SETFL, flags | O_NONBLOCK);
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
    // int flags = fcntl(ev.data.fd, F_GETFL, 0);
    // fcntl(ev.data.fd, F_SETFL, flags | O_NONBLOCK);
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, timer_fd, &ev) == -1) {
        perror("epoll_create1");
        exit(1);
    }
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 100000000;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 100000000;
    if (timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &ts, NULL) == -1) {
        perror("timer_fd settime");
        exit(1);
    }

    uint16_t lastKey = KEY_RIGHT;
    while (TRUE)  {
        //usleep(1000);
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        LOG("NofEvents: %d\n", nfds);
        for (auto n = 0; n < nfds; ++n) {
            if (events[n].data.fd == STDIN_FILENO) {
                // yes, else epoll keeps on notifying :)
                lastKey = key;
                key = getch();
                if (key == lastKey) {
                    continue; // ignore if same direction
                    // Ah, it also helps keeping snake speed in control
                    // since otherwise it speeds up with multiple events
                    // triggered if key is kept pressed
                }
                this->nextStep(key);
            } else if (events[n].data.fd == timer_fd) {
                long int timersElapsed = 0;
                ssize_t rc = read(timer_fd, &timersElapsed, 8);
                LOG("Timer: read returned %lu\n", rc);
                this->nextStep(lastKey);
            }
        }
    }
}
