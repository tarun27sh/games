#SRCS = main.cc easylogging++.cc snake.cc grid.cc grid_ncurses.cc 
SRCS = main.cc  snake.cc grid.cc grid_ncurses.cc 
CC=g++
#CC=clang-cpp-9
CFLAGS = -g -Wall -std=c++17 
LDFLAGS = -lcurses
#LDLIBS = -lc -lrt

PROGS = snake

all:$(PROGS)

snake:
	$(CC) -g $(SRCS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ 
	#$(CC) -g $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(PROGS) *core*
