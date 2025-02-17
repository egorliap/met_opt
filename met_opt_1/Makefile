CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lm

SRC = lp_problem_parser.cpp lp_problem_solver.cpp lp_problem.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
DEPS = lp_problem_parser.h lp_problem_solver.h lp_problem.h

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
