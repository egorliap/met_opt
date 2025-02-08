#include <string>
#include <vector>
#include "lp_problem.h"
using std::string;
using std::vector;


class Reader {
public:
	virtual string read_line()=0;
};

class FileReader : Reader{
public:
	string read_line() override;
};

class STDInputReader : Reader {
public:
	string read_line() override;
};

class LPProblemParser {
private:
	Reader& reader;
	std::pair<int, int> parse_dims(string str_dims);
	ObjectiveType parse_objective_type(string str_objective_type);
	vector<double> parse_objective(string str_objective);
	Constraint parse_constraint(string str_constraint);
	VariableBound parse_var_bound(string str_var_bound);
public:
	LPProblemParser(Reader&  reader_) : reader(reader_) {};

	void parse(LPProblem& problem);
};