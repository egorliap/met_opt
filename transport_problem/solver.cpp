#include "solver.h"
#include <iostream>
#include <numeric>
#include <deque>
#include <queue>
#include <limits>
#include <utility>
#include <algorithm>

vector<pair<int, int>> find_way(vector<vector<double>> adjacency, int from, int to)
{
    vector<pair<int, int>> path;
    int n = adjacency.size();    // amount of Ai edges
    int m = adjacency[0].size(); // amount of Bi edges

    vector<vector<int>> g(n + m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (adjacency[i][j] != infinity)
            {
                g[i].push_back(n + j);
                g[n + j].push_back(i);
            }
        }
    }

    std::queue<int> q;
    q.push(from);

    vector<int> d(n + m, -1);
    vector<int> p(n + m);
    d[from] = 0;

    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int u : g[v])
        {
            if (d[u] == -1)
            {
                q.push(u);
                d[u] = d[v] + 1;
                p[u] = v;
            }
        }
    }
    int v = to + n;
    vector<int> path_simple;

    while (v != from)
    {

        path_simple.insert(path_simple.begin(), v);
        v = p[v];
    }
    path_simple.insert(path_simple.begin(), from);

    for (int i = 0; i < path_simple.size() - 1; i++)
    {
        if (path_simple[i] >= n)
        {
            path.push_back({path_simple[i + 1], path_simple[i] - n});
        }
        else
        {
            path.push_back({path_simple[i], path_simple[i + 1] - n});
        }
    }
    return path;
}

vector<vector<double>> TransportProblemSolver::ne_angle_method(TransportProblem &problem)
{
    vector<double> remaining_providers, remaining_consumers;

    std::copy(problem.providers.begin(), problem.providers.end(), back_inserter(remaining_providers));
    std::copy(problem.consumers.begin(), problem.consumers.end(), back_inserter(remaining_consumers));

    vector<std::pair<int, int>> stack = {{0, 0}};
    vector<vector<double>> op_plan(problem.n, vector<double>(problem.m, infinity));
    while (!stack.empty())
    {
        std::pair<int, int> cur = stack[stack.size() - 1];
        stack.pop_back();
        int cur_prov_index = cur.first;
        int cur_cons_index = cur.second;

        if (remaining_consumers[cur_cons_index] <= remaining_providers[cur_prov_index] &&
            remaining_consumers[cur_cons_index] > 0)
        {
            op_plan[cur_prov_index][cur_cons_index] = remaining_consumers[cur_cons_index];

            remaining_providers[cur_prov_index] -= remaining_consumers[cur_cons_index];
            remaining_consumers[cur_cons_index] = 0;

            if (remaining_providers[cur_prov_index] == 0 && cur_prov_index < problem.n - 1 && cur_cons_index < problem.m - 1)
            {
                op_plan[cur_prov_index][cur_cons_index + 1] = 0;

                stack.push_back({cur_prov_index + 1, cur_cons_index + 1});
            }
            else if (cur_cons_index < problem.m - 1)
            {
                stack.push_back({cur_prov_index, cur_cons_index + 1});
            }
        }
        else if (remaining_providers[cur_prov_index] <= remaining_consumers[cur_cons_index] &&
                 remaining_providers[cur_prov_index] > 0)
        {
            op_plan[cur_prov_index][cur_cons_index] = remaining_providers[cur_prov_index];

            remaining_consumers[cur_cons_index] -= remaining_providers[cur_prov_index];
            remaining_providers[cur_prov_index] = 0;

            if (remaining_consumers[cur_cons_index] == 0 && cur_prov_index < problem.n - 1 && cur_cons_index < problem.m - 1)
            {
                op_plan[cur_prov_index + 1][cur_cons_index] = 0;

                stack.push_back({cur_prov_index + 1, cur_cons_index + 1});
            }
            else if (cur_prov_index < problem.n - 1)
            {
                stack.push_back({cur_prov_index + 1, cur_cons_index});
            }
        }
    }
    return op_plan;
}

bool TransportProblemSolver::is_closed(TransportProblem &problem)
{
    if (std::reduce(problem.providers.begin(), problem.providers.end()) != std::reduce(problem.consumers.begin(), problem.consumers.end()))
    {
        return false;
    }
    return true;
}

void TransportProblemSolver::find_potentials(vector<double> *u, vector<double> *v, vector<vector<double>> c, vector<std::pair<int, int>> taken)
{
    (*u)[0] = 0;
    // u[i] + v[j] == c[i][j]
    std::deque<std::pair<int, int>> left;
    for (auto &el : taken)
    {
        left.push_back(el);
    }
    while (!left.empty())
    {
        auto [i, j] = left[0];
        left.pop_front();
        if ((*u)[i] != infinity)
        {
            (*v)[j] = c[i][j] - (*u)[i];
        }
        else if ((*v)[j] != infinity)
        {
            (*u)[i] = c[i][j] - (*v)[j];
        }
        else
        {
            left.push_back({i, j});
            // for (auto &el : left)
            // {
            //     std::cout << el.first << " " << el.second << std::endl;
            // }
        }
    }
}

TransportProblemSolution &TransportProblemSolver::solve(TransportProblem &problem, bool logs)
{
    problem.convert_restrictions();
    if (!is_closed(problem))
    {
        TransportProblemSolution *sol = new TransportProblemSolution(Status::OPEN_PROBLEM, {});
        return *sol;
    }
    vector<vector<double>> plan = ne_angle_method(problem); // support plan

    vector<std::pair<int, int>> taken;
    vector<std::pair<int, int>> not_taken;
    vector<double> u;
    vector<double> v;
    vector<double> delta;
    bool is_opt = true;
    bool valid = true;

    while (true)
    {
        if (logs)
        {
            std::cout << "Current Plan: " << std::endl;
            for (auto &line : plan)
            {
                for (auto &el : line)
                {
                    if (el == infinity)
                    {
                        std::cout << "X ";
                        continue;
                    }
                    std::cout << el << " ";
                }
                std::cout << std::endl;
            }
            std::cin.get();
        }
        u = vector<double>(problem.n, infinity);
        v = vector<double>(problem.m, infinity);
        taken.clear();
        not_taken.clear();
        is_opt = true;
        valid = true;
        for (int i = 0; i < problem.n; i++)
        {
            for (int j = 0; j < problem.m; j++)
            {
                if (plan[i][j] != infinity)
                {
                    taken.push_back({i, j});
                }
                else
                {
                    not_taken.push_back({i, j});
                }
            }
        }

        find_potentials(&u, &v, problem.cost, taken);

        if (logs)
        {
            std::cout << "u: ";
            for (auto &el : u)
            {
                std::cout << el << " ";
            }
            std::cout << std::endl;
            std::cout << "v: ";
            for (auto &el : v)
            {
                std::cout << el << " ";
            }
            std::cout << std::endl;
        }
        pair<int, int> max_delta_coords;
        double max_delta = -100;

        for (auto &el : not_taken)
        {
            auto [i, j] = el;
            if (u[i] + v[j] - problem.cost[i][j] > 0)
            {
                is_opt = false;
                if (u[i] + v[j] - problem.cost[i][j] >= max_delta)
                {
                    max_delta = u[i] + v[j] - problem.cost[i][j];
                    max_delta_coords = {i, j};
                }
            }
        }

        if (is_opt)
        {
            double obj = 0;
            for (int i = 0; i < problem.n; i++)
            {

                for (int j = 0; j < problem.m; j++)
                {
                    if (plan[i][j] < infinity)
                    {
                        obj += problem.cost[i][j] * plan[i][j];
                    }
                }
            }

            TransportProblemSolution *sol = new TransportProblemSolution(Status::OPTIMAL, plan, obj);
            return *sol;
        }

        auto [m_i, m_j] = max_delta_coords; // not taken route to maximise

        if (logs)
        {
            std::cout << "Max delta is " << max_delta << " with coordinates " << m_i << ", " << m_j << std::endl;
        }

        vector<pair<int, int>> path = find_way(plan, m_i, m_j);
        if (logs)
        {
            std::cout << "Loop found: " << std::endl;

            for (auto &el : path)
            {
                std::cout << el.first << " " << el.second << std::endl;
            }
        }

        double theta = infinity;
        for (int i = 0; i < path.size(); i++)
        {
            if (i % 2 == 0)
            {
                auto [k, j] = path[i];
                if (plan[k][j] < theta)
                {
                    theta = plan[k][j];
                }
            }
        }
        for (int i = 0; i < path.size(); i++)
        {
            auto [k, j] = path[i];

            if (i % 2 == 0)
            {
                plan[k][j] -= theta;
            }
            else
            {
                plan[k][j] += theta;
            }
            if (plan[k][j] == 0 && valid)
            {
                plan[k][j] = infinity;
                valid = false;
            }
        }
        plan[m_i][m_j] = theta;
        if (logs)
        {
            std::cout << "Theta: " << theta << std::endl;
            std::cin.get();
        }
    }
}

void TransportProblemSolution::set_solution(vector<vector<double>> solution)
{
    this->solution = solution;
}

void TransportProblemSolution::print()
{

    std::cout << "\n\n\tSolution to transport problem\n"
              << std::endl;
    std::cout << "Status: " << (status == Status::OPEN_PROBLEM ? "problem is open" : "optimal plan is found") << std::endl;
    if (status == Status::OPTIMAL)
    {
        std::cout << "\nOptimal plan: " << std::endl;
        for (int i = 0; i < solution.size(); i++)
        {
            for (int j = 0; j < solution[0].size(); j++)
            {
                if (solution[i][j] < infinity)
                {
                    std::cout << solution[i][j] << " ";
                }
                else
                {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "\nFound minimal cost value: " << std::endl;

        std::cout << objective_value << std::endl;
    }
}
