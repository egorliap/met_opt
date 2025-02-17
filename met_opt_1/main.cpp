#include "lp_problem_solver.h"

void run_interface() {

}

int main() {
	LPProblem* problem = new LPProblemSlack(3);
	problem->set_objective({ 3, 4, 6 }, ObjectiveType::MAXIMIZE);

	problem->add_constraint({ {2, 5, 2}, 12, InequalityType::LESS_EQUAL });
	problem->add_constraint({ {7, 1, 2}, 18, InequalityType::LESS_EQUAL });

	problem->add_var_bound({ 1, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 2, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 3, BoundType::NOT_NEGATIVE });



	problem->convert();
	problem->print_problem();

	SimplexSolver solver;
	LPProblemSolution solution = solver.solve(*problem);
	solution.print();


	return 0;
}