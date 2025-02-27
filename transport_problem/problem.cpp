#include "problem.h"
#include <iostream>
#include <string>
using std::cout, std::endl;

void TransportProblem::print()
{
    cout << "\tTransport Problem" << endl;
    int a_max_len = 0;
    int c_max_len = 0;
    int b_max_len = 0;
    for (int i = 0; i < n; i++)
    {
        a_max_len = std::max(static_cast<int>(std::to_string(static_cast<int>(providers[i])).size()), a_max_len);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            c_max_len = std::max(static_cast<int>(std::to_string(static_cast<int>(cost[i][j])).size()), c_max_len);
        }
    }
    for (int i = 0; i < m; i++)
    {
        b_max_len = std::max(static_cast<int>(std::to_string(static_cast<int>(consumers[i])).size()), b_max_len);
    }

    for (int i = 0; i < a_max_len + 4 + static_cast<int>(std::to_string(n).size()); i++)
    {
        cout << " ";
    }
    int b_line_len = static_cast<int>(std::to_string(m).size()) + 1;
    int column_spaces = std::max(std::max(c_max_len, b_line_len), b_max_len);
    for (int i = 0; i < m; i++)
    {
        for (int k = 0; k < column_spaces; k++)
        {
            cout << " ";
        }
        cout << "B" << i + 1;
    }
    cout << endl;
    for (int i = 0; i < a_max_len + 4 + static_cast<int>(std::to_string(n).size()); i++)
    {
        cout << " ";
    }

    for (int i = 0; i < m; i++)
    {
        for (int k = 0; k < column_spaces + 2 - static_cast<int>(std::to_string(static_cast<int>(consumers[i])).size()); k++)
        {
            cout << " ";
        }
        cout << consumers[i];
    }

    cout << endl;
    for (int i = 0; i < a_max_len + 4 + static_cast<int>(std::to_string(n).size()); i++)
    {
        cout << " ";
    }
    for (int i = 0; i < m; i++)
    {
        for (int k = 0; k < column_spaces + 2; k++)
        {
            cout << "-";
        }
    }
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "A" << i + 1 << ": " << providers[i];

        for (int k = 0; k < a_max_len - static_cast<int>(std::to_string(static_cast<int>(providers[i])).size()); k++)
            cout << " ";
        cout << "|";
        for (int j = 0; j < m; j++)
        {
            for (int o = 0; o < column_spaces + 2 - static_cast<int>(std::to_string(static_cast<int>(cost[i][j])).size()); o++)
            {
                cout << " ";
            }
            cout << cost[i][j];
        }
        cout << endl;
    }
}

void TransportProblem::add_restriction(int from, int to, double limit)
{
}

void TransportProblem::add_restrictions(const vector<vector<double>> &restrs)
{
}
