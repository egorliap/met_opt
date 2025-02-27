#include "problem.h"

int main()
{
    int n = 4;
    int m = 5;
    vector<vector<double>> cost = { {3, 5, 6, 8, 11},
                                    {4, 7, 3, 5, 14},
                                    {9, 3, 4, 4, 10},
                                    {1, 5, 16, 3, 8} };
    vector<double> providers = {10, 3, 8, 10};
    vector<double> consumers = {2, 8, 2, 8, 11};
    
    TransportProblem problem(cost, providers, consumers);
    problem.print();
    return 0;
}