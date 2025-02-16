#include <vector>
#include "lp_problem.h"
using std::vector;

enum Status {
	OPTIMAL, INFEASABLE, UNBOUNDED
};

class LPProblemSolution {
private:
	Status status;
	vector<double> solution;
	double objective_value;
public:
	LPProblemSolution(Status status, vector<double> solution = {}, double objective_value = 0) {
		this->status = status;
		if (status == Status::OPTIMAL) {
			this->solution = solution;
			this->objective_value = objective_value;
		}
		else if (status == Status::INFEASABLE) {
			this->solution = {};
			this->objective_value = 0;
		}
		else if (status == Status::UNBOUNDED) {
			this->solution = {};
			this->objective_value = INFINITY;
		}
	}

	void print();
};

class LPProblemSolver {
public:
	virtual LPProblemSolution& solve(LPProblem& problem) = 0;
};

class SimplexSolver : LPProblemSolver {
private:
	vector<double> get_support_vector();
public:
	LPProblemSolution& solve(LPProblem& problem) override;
};

