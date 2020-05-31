#include "./snake.h"

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <deque>

Snake::Snake() {
    snakePosition.push_back(std::make_pair(0, 0));
}
void Snake::nextStep(Ops op) {
    switch (op) {
        case Ops::Fwd: {
                           auto head = this->snakePosition.front();
                           this->snakePosition.pop_back();
                           this->snakePosition.push_front(
                                    std::make_pair(head.first, head.second+1));
                           break;
                       }
        case Ops::Right: {
                             auto head = this->snakePosition.front();
                             this->snakePosition.pop_back();
                             this->snakePosition.push_front(
                                    std::make_pair(head.first+1, head.second));
                             break;
                         }
        case Ops::Left: {
                            auto head = this->snakePosition.front();
                            this->snakePosition.pop_back();
                            this->snakePosition.push_front(
                                    std::make_pair(head.first-1, head.second));
                            break;
                        }
        default:
                        std::cout << "invalid move" << std::endl;
    }

    this->draw();
    if (grid.isFood(snakePosition[0]))
        this->grow();
}

void Snake::grow() {
    std::pair<int, int> lastDotLoc =
                    snakePosition[snakePosition.size() -1];
    snakePosition.push_back(std::make_pair(lastDotLoc.first,
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


void Snake::draw() {
    grid.draw(this->snakePosition);
}

/* never return from this function */
void Snake::gameLoop() {
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
    std::cout << "start game!!!" << std::endl;
    while (true) {
        // usleep(100000);
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for (auto n = 0; n < nfds; ++n) {
            if (events[n].data.fd == STDIN_FILENO) {
                // yes, else epoll keeps on notifying :)
                std::cout << "input detected!" << std::endl;
                key = getInput();
                if (key == 68) {         // left
                    this->nextStep(Ops::Left);
                } else if (key == 67) {        // right
                    this->nextStep(Ops::Right);
                } else {
                    this->nextStep(Ops::Fwd);
                }
                std::cout << "keyboard event" << std::endl;
            } else if (events[n].data.fd == timer_fd) {
                this->nextStep(Ops::Fwd);
                std::cout << "clock event\n" << std::endl;
            }
            std::cout << "#events=%d" <<  nfds << std::endl;
        }
    }
}
