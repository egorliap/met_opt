#include "lp_problem_solver.h"

void run_interface() {

}

int main() {
	LPProblem* problem = new LPProblemSlack(5);
	problem->set_objective({ -3, 1, 4, 0, 0 }, ObjectiveType::MINIMIZE);

	problem->add_constraint({ {0, -1, 1, 1, 0 }, 1, InequalityType::EQUAL });
	problem->add_constraint({ {-5, 1, 1, 0, 0}, 2, InequalityType::EQUAL });
	problem->add_constraint({ {-8, 1, 2, 0, -1}, 3, InequalityType::EQUAL });
	problem->add_constraint({ {2, 3, 5, 7, 1}, 40, InequalityType::LESS_EQUAL });
	//problem->add_constraint({ {2, 3, -5, 7, 1}, 1, InequalityType::GREATER_EQUAL });

	problem->add_constraint({ {1, 4, -6, 3, -5}, -11, InequalityType::GREATER_EQUAL});

	problem->add_var_bound({ 1, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 2, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 3, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 4, BoundType::NOT_NEGATIVE });



	problem->convert();
	problem->print_problem();

	SimplexSolver solver;
	LPProblemSolution solution = solver.solve(*problem);
	solution.print();


	return 0;
}