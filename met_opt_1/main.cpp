#include "lp_problem_solver.h"

void run_interface() {

}

int main() {
	LPProblem* problem = new LPProblemSlack(2);
	//problem->set_objective({ 3, 2, 5, -1 }, ObjectiveType::MAXIMIZE);
	////problem.add_constraint({ {1, 1, 1, 1}, 4, InequalityType::LESS_EQUAL });
	//problem->add_constraint({ {2, 1, 4, 5}, 5, InequalityType::LESS_EQUAL });
	////problem.add_constraint({ {124, 14, 12, 1}, 0, InequalityType::EQUAL });
	//problem->add_constraint({ {1, 1, -1, -23}, 4, InequalityType::GREATER_EQUAL });

	//problem->add_var_bound({ 1, BoundType::NOT_NEGATIVE });
	//problem->add_var_bound({ 2, BoundType::NOT_POSITIVE });
	//problem.add_var_bound({ 3, BoundType::NOT_POSITIVE });
	//problem.add_var_bound({ 4, BoundType::NOT_POSITIVE });

	problem->set_objective({ 1,2 }, ObjectiveType::MAXIMIZE);
	problem->add_constraint({ {-1, 2}, 2, InequalityType::GREATER_EQUAL });
	problem->add_constraint({ {1, 1}, 4, InequalityType::GREATER_EQUAL });
	problem->add_constraint({ {1, -1}, 2, InequalityType::LESS_EQUAL });
	problem->add_constraint({ {0,1}, 6, InequalityType::LESS_EQUAL });
	problem->add_var_bound({ 1, BoundType::NOT_NEGATIVE });
	problem->add_var_bound({ 2, BoundType::NOT_POSITIVE });


	problem->convert();
	problem->print_problem();

	SimplexSolver solver;
	LPProblemSolution solution = solver.solve(*problem);
	solution.print();


	return 0;
}