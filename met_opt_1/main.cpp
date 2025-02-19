#include "lp_problem_parser.h"
#include <iostream>

void run_interface()
{
	std::string filename;
	std::cout << "Enter input file name: ";
	std::cin >> filename;
	LPProblem *problem;
	try
	{
		problem = LPProblemParser::parse(filename);
		std::cout << "Parsed problem:" << std::endl;
		problem->print_problem();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	while (true)
	{
		int opt;
		std::cout << "Choose option:\n1 - print problem\n2 - convert problem to cannonical from\n3 - get dual problem (and set as main problem)\n4 - solve problem with step-by-step logs\n5 - solve problem without step-by-step logs\n6 - exit\n"
				  << std::endl;
		std::cin >> opt;
		switch (opt)
		{
		case 1:
		{
			problem->print_problem();
			break;
		}
		case 2:
		{
			problem->convert();
			break;
		}
		case 3:
		{
			problem = &problem->dual();

			break;
		}
		case 4:
		{
			SimplexSolver solver;
			LPProblemSolution solution = solver.solve(*problem);
			vector<double> init_sol = problem->get_initial_solution(solution.solution);
			solution.set_solution(init_sol);
			solution.print_sol();
			break;
		}
		case 6:
		{
			exit(1);
		}
		default:
			break;
		}
	}

	problem->convert();
	problem->print_problem();

	SimplexSolver solver;
	LPProblemSolution solution = solver.solve(*problem);
	solution.print_sol();
}

int main()
{
	run_interface();
	return 0;
}