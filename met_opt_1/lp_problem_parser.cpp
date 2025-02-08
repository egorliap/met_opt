#include <exception>
#include <iostream>
#include "lp_problem_parser.h"

std::pair<int, int> LPProblemParser::parse_dims(string str_dims)
{
	return std::pair<int, int>();
}

ObjectiveType LPProblemParser::parse_objective_type(string str_objective_type)
{
	return ObjectiveType();
}

vector<double> LPProblemParser::parse_objective(string str_objective)
{
	return vector<double>();
}

Constraint LPProblemParser::parse_constraint(string str_constraint)
{
	return Constraint();
}

VariableBound LPProblemParser::parse_var_bound(string str_var_bound)
{
	return VariableBound();
}

void LPProblemParser::parse(LPProblem& problem) {
	string line;
	line = reader.read_line();
	std::pair<int, int> dims = parse_dims(line);
	int n = dims.first; int m = dims.second;
	problem.set_solution_dim(n);

	line = reader.read_line();
	ObjectiveType objective_type = parse_objective_type(line);
	
	line = reader.read_line();
	vector<double> objective = parse_objective(line);

	for (int i = 0; i < m; i++) {
		line = reader.read_line();
		problem.add_constraint(parse_constraint(line));
	}
	for (int i = 0; i < n; i++) {
		try {
			line = reader.read_line();
		}
		catch (std::iostream::failure) {
			break;
		}
		problem.add_var_bound(parse_var_bound(line));
	}
}

string FileReader::read_line()
{
	return string();
}

string STDInputReader::read_line()
{
	return string();
}
