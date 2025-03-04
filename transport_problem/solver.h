#pragma once

#include "problem.h"

using std::vector, std::pair;

enum Status
{
    OPTIMAL,
    OPEN_PROBLEM,
};

class TransportProblemSolution
{
public:
    Status status;
    vector<vector<double>> solution;
    double objective_value;
    TransportProblemSolution(Status status, const vector<vector<double>> & solution, double objective_value = 0)
    {
        this->status = status;
        if (status == Status::OPTIMAL)
        {
            this->solution = solution;
            this->objective_value = objective_value;
        }
    }
    void set_solution(vector<vector<double>> solution);
    void set_objective(double objective);
    void print();
};

class TransportProblemSolver
{
private:
    vector<vector<double>> ne_angle_method(TransportProblem &problem);
    bool is_closed(TransportProblem &problem);
    void find_potentials(vector<double> *u, vector<double> *v, vector<vector<double>> c, vector<std::pair<int, int>> taken);

public:
    TransportProblemSolution &solve(TransportProblem &problem, bool logs);
};
