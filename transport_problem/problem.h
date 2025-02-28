#pragma once

#include <vector>
#include <stdexcept>
#include <limits>

using std::vector;

const int infinity = std::numeric_limits<int>::max();

class TransportProblem
{
public:
    int n, m;                               // n - amount of lines in cost (amount of providers); m - amount of columns in cost (amount of consumers)
    vector<vector<double>> cost;            // matrix of costs of delivery per unit of delivery
    vector<vector<double>> restrictions;    // matrix of restrictions imposed on amount of units in each delivery way
    vector<double> providers;               // vector of limits of provision for each of n providers
    vector<double> consumers;               // vector of consumption demand for each of m consumers
    vector<std::pair<int, int>> restricted; // indexes of COLUMNS in which we put additional, difference consumers (x[i][j] - limit[i][j])

    TransportProblem(const vector<vector<double>> &cost, const vector<double> &providers, const vector<double> &consumers)
    {
        this->cost = cost;
        this->providers = providers;
        this->consumers = consumers;
        n = providers.size();
        m = consumers.size();
        if (cost.size() != n || cost[0].size() != m)
        {
            throw std::runtime_error("Invalid data: dim cost is not compatable with dims of providers and consumers");
        }
        this->restrictions = vector<vector<double>>(n, vector<double>(m, infinity));
    }

    void print();
    void add_restriction(int from, int to, double limit);
    void convert_restrictions();
    vector<vector<double>> get_initial_plan(vector<vector<double>> plan);
};