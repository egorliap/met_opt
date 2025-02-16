#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "lp_problem_parser.h"

std::pair<int, int> LPProblemParser::parse_dims(const string& str_dims)
{
	std::istringstream iss(str_dims);
	int first, second;
	iss >> first >> second;
	return std::make_pair(first, second);

}

ObjectiveType LPProblemParser::parse_objective_type(const string& str_objective_type)
{
	return (str_objective_type.compare("max") != 0) ? ObjectiveType::MAXIMIZE : ObjectiveType::MINIMIZE;
}

vector<double> LPProblemParser::parse_objective(const string& str_objective)
{
	vector<double> numbers;
	std::istringstream iss(str_objective);
	double number;

	while (iss >> number) {
		numbers.push_back(number);
	}

	return numbers;
}

Constraint LPProblemParser::parse_constraint(const string& str_constraint, int n)
{
	std::istringstream iss(str_constraint);
	std::vector<double> coefs;
	double b;

	for (int i = 0; i < n; ++i) {
		double a;
		iss >> a;
		coefs.push_back(a);
	}

	string ineq;
	iss >> ineq;
	iss >> b;
	InequalityType constraint_type{};

	if (ineq.compare("<=") == 0) {
		constraint_type = InequalityType::LESS_EQUAL;
	}else if (ineq.compare(">=") == 0) {
		constraint_type = InequalityType::GREATER_EQUAL;
	}else if (ineq.compare("=") == 0) {
		constraint_type = InequalityType::EQUAL;
	}
	return Constraint({ coefs, b, constraint_type });
}

VariableBound LPProblemParser::parse_var_bound(const string& str_var_bound)
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
		problem.add_constraint(parse_constraint(line, n));
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
