CXX = g++
CXXFLAGS = -std=c++17 -I./headers
SRCDIR = src
BINDIR = bin
HEADERSDIR = headers
TARGET = $(BINDIR)/coffee_machine
HEADERS = $(HEADERSDIR)/coffee_machine.h $(HEADERSDIR)/ingredients.h $(HEADERSDIR)/recipes.h $(HEADERSDIR)/menu.h $(HEADERSDIR)/parser.h
SRC = $(SRCDIR)/coffee_machine.cpp $(SRCDIR)/ingredients.cpp $(SRCDIR)/main.cpp $(SRCDIR)/recipes.cpp $(SRCDIR)/menu.cpp $(SRCDIR)/parser.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)