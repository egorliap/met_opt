#include "solver.h"
#include <iostream>

int main()
{
    int n = 4;
    int m = 5;
    vector<vector<double>> cost = {{3, 5, 6, 8, 11},
                                   {4, 7, 3, 5, 14},
                                   {9, 3, 4, 4, 10},
                                   {1, 5, 16, 3, 8}};
    vector<double> providers = {10, 3, 8, 10};
    vector<double> consumers = {2, 8, 2, 8, 11};

    TransportProblem problem(cost, providers, consumers);


    problem.add_restriction(3, 4, 4);


    problem.print();

    TransportProblemSolver solver;
    TransportProblemSolution solution = solver.solve(problem);
    solution.print();

    vector<vector<double>> initial_plan = problem.get_initial_plan(solution.solution);
    solution.set_solution(initial_plan);
    solution.print();
    return 0;
}