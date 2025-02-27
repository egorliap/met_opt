CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lm

SRC = parser.cpp solver.cpp problem.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
DEPS = parser.h solver.h problem.h

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
