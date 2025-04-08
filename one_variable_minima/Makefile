CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lm

SRC = solver.cpp function.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
DEPS = solver.h function.h

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
