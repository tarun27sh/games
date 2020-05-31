SRCS = main.cc snake.cc grid.cc
CC=g++
CFLAGS = -g -Wall -std=c++11
LDFLAGS =
#LDLIBS = -lc -lrt

PROGS = snake

all:$(PROGS)

snake:
	$(CC) -g $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) $(PROGS) *core*
