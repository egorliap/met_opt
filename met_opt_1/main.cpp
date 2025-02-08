#include <stdio.h>
#include "lp_problem.h"

int main() {
	LPProblemGeneral problem(4);
	problem.set_objective({ 3, 2, 5, -1 }, ObjectiveType::MAXIMIZE);
	problem.add_constraint({ {1, 1, 1, 1}, 4, InequalityType::LESS_EQUAL });
	problem.add_constraint({ {2, 1, 4, 5}, 5, InequalityType::LESS_EQUAL });
	problem.add_constraint({ {124, 14, 12, 1}, 0, InequalityType::EQUAL });
	problem.add_constraint({ {1, 1, -1, -23}, 4, InequalityType::GREATER_EQUAL });
	problem.add_var_bound({ 1, BoundType::NOT_NEGATIVE });
	problem.add_var_bound({ 2, BoundType::NOT_POSITIVE });

	problem.print_problem();
	problem.convert();
	problem.print_problem();

	LPProblem& dual_problem = problem.dual();
	dual_problem.print_problem();
	return 0;
}