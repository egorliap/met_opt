#include "lp_problem_parser.h"
#include <iostream>

void run_interface()
{
	std::string filename;
	std::cout << "Enter input file name: ";
	std::cin >> filename;

	try
	{
		LPProblemSlack *problem = LPProblemParser::parse(filename);
		problem->print_problem();
		problem->convert();
		problem->print_problem();

		SimplexSolver solver;
		LPProblemSolution solution = solver.solve(*problem);
		solution.print_sol();

		delete problem;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

int main()
{
	run_interface();
	return 0;
}