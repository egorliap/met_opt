#include "include/parser.h"
#include "include/solver.h"
#include <iostream>

void run_interface()
{
    std::string filename;
    std::cout << "Enter input file name: ";
    std::cin >> filename;
    TransportProblem *problem;
    TransportProblemParser parser;
    TransportProblemSolver solver;

    try
    {
        problem = parser.parse(filename);
        std::cout << "Parsed problem:" << std::endl;
        problem->print();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        abort();
    }
    while (true)
    {
        int opt;
        std::cout << "Choose option:\n1 - print problem\n2 - add restriction\n3 - solve problem with step-by-step logs\n4 - solve problem without step-by-step logs\n5 - exit\n"
                  << std::endl;
        std::cin >> opt;
        switch (opt)
        {
        case 1:
        {
            problem->print();
            break;
        }
        case 2:
        {
            int from, to;
            double limit;
            std::cout << "Provder:" << std::endl;
            std::cin >> from;

            std::cout << "Consumer:" << std::endl;
            std::cin >> to;

            std::cout << "Limit:" << std::endl;
            std::cin >> limit;

            problem->add_restriction(from, to, limit);
            break;
        }
        case 3:
        {
            TransportProblemSolution solution = solver.solve(*problem, true);

            vector<vector<double>> initial_plan = problem->get_initial_plan(solution.solution);
            solution.set_solution(initial_plan);
            solution.print();
            break;
        }
        case 4:
        {
            TransportProblemSolution solution = solver.solve(*problem, false);

            vector<vector<double>> initial_plan = problem->get_initial_plan(solution.solution);
            solution.set_solution(initial_plan);
            solution.print();
            break;
        }
        case 5:
        {
            exit(1);
            break;
        }
        default:
            break;
        }
    }
}

int main()
{
    run_interface();

    return 0;
}