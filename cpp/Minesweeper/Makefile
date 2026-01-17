CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -Werror

all: mines

mines: board.cpp mines.cpp
	$(CXX) $(CXXFLAGS) -o mines board.cpp mines.cpp

clean:
	rm -f mines

.PHONY: all clean
