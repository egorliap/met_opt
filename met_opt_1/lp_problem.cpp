#include <iterator>
#include <iostream>
#include "lp_problem.h"
using std::cout;
using std::endl;


LPProblem::LPProblem() {
    n = 0;
}

LPProblem::LPProblem(int n)
{
    this->n = n;
}


LPProblem::LPProblem(vector<double> objective, ObjectiveType objective_type)
{
    this->n = objective.size();
    this->objective = objective;
    this->objective_type = objective_type;
}

void LPProblem::set_solution_dim(int n)
{
    this->n = n;
}


void LPProblem::set_objective(const std::vector<double>& coeffs, ObjectiveType type) 
{
    objective = coeffs;
    objective_type = type;
}


void LPProblem::add_constraint(const Constraint& c)
{
    if (c.coefficients.size() > n) {
        throw;
    }
    constraints.push_back(c);
}


void LPProblem::add_var_bound(const VariableBound& vb)
{
    if (vb.component > n || vb.component < 1) {
        throw;
    }
    bounds.push_back(vb);
}


void LPProblem::print_problem()
{
    cout << "Linear Programming Problem of " << ((objective_type == ObjectiveType::MINIMIZE) ? "Minimization" : "Maximization") << endl;
    cout << "Objective:" << endl;
    for (int i = 0; i < objective.size(); i++) {
        if (i != 0 && objective[i] >= 0) {
            cout << "+ ";
        }
        cout << objective[i] << "x" << i + 1 << " ";
    }
    cout << endl;


    cout << "Constraints:" << endl;
    for (auto& constraint : constraints) {
        for (int i = 0; i < constraint.coefficients.size(); i++) {
            if (i != 0 && constraint.coefficients[i] >= 0) {
                cout << "+ ";
            }
            cout << constraint.coefficients[i] << "x" << i+1 << " ";

        }
        if (constraint.type == InequalityType::LESS_EQUAL) cout << "<= " << constraint.b;
        else if (constraint.type == InequalityType::GREATER_EQUAL) cout << ">= " << constraint.b;
        else if (constraint.type == InequalityType::EQUAL) cout << "= " << constraint.b;
        cout << endl;
    }

    cout << "Bounds:" << endl;
    for (auto& bound : bounds) {
        if (bound.type != BoundType::NO) {
            cout << "x" << bound.component << (bound.type == BoundType::NOT_NEGATIVE ? " >= 0" : " <= 0") << endl;
        }
        else {
            cout << "x" << bound.component << endl;
        }
    }
    cout << "\n\n" << endl;
}

vector<double> LPProblem::get_objective()
{
    return objective;
}

vector<Constraint> LPProblem::get_constraints()
{
    return constraints;
}


void LPProblemGeneral::convert()
{
    // Objective generalization
    if (objective_type == ObjectiveType::MAXIMIZE) {
        for (auto& component : objective) {
            component *= -1;
        }
        objective_type = ObjectiveType::MINIMIZE;
    }

    // Constraint generalization
    for (auto& constraint : constraints) {
        if (constraint.type == InequalityType::LESS_EQUAL) {
            for (auto& component : constraint.coefficients) {
                component *= -1;
            }
            constraint.b *= -1;
            constraint.type = InequalityType::GREATER_EQUAL;
        }
    }

    // Bound generalization
    for (auto& bound : bounds) {
        if (bound.type == BoundType::NOT_POSITIVE) {
            bound.type = BoundType::NOT_NEGATIVE;
            int component = bound.component - 1;

            // Modifying constraints
            for (auto& constraint : constraints) {
                constraint.coefficients[component] *= -1;
            }

            // Modifying objective
            objective[component] *= -1;

        }
    }
    if (bounds.size() < n){
        bool fl = false;
        for (int i = 1; i <= n; i++) {
            for (auto& bound : bounds) {
                if (bound.component == i) {
                    fl = true;
                    break;
                }
            }

            if (!fl) {
                bounds.push_back({ i, BoundType::NO });
            }
            fl = false;
        }
    }
}


LPProblem& LPProblemGeneral::dual() {
    this->convert();

    int m = constraints.size();
    LPProblemGeneral* dual_problem = new LPProblemGeneral(m);
    

    vector<double> dual_objective;
    for (int i = 0; i < m; i++) {
        dual_objective.push_back(constraints[i].b);

        VariableBound dual_bound;
        if (constraints[i].type == InequalityType::GREATER_EQUAL) {
            dual_bound.component = i + 1;
            dual_bound.type = BoundType::NOT_NEGATIVE;
        }
        else {
            dual_bound.component = i + 1;
            dual_bound.type = BoundType::NO;
        }
        dual_problem->add_var_bound(dual_bound);

    }
    for (int i = 0; i < n; i++){
        Constraint dual_constraint;
        
        for (int j = 0; j < m; j++) {
            dual_constraint.coefficients.push_back(constraints[j].coefficients[i]);
        }
        for (auto& bound : bounds) {
            if (bound.component - 1 == i) {
                if (bound.type == BoundType::NOT_NEGATIVE) {
                    dual_constraint.type = InequalityType::LESS_EQUAL;
                    break;
                }
                else {
                    dual_constraint.type = InequalityType::EQUAL;
                    break;
                }
            }
        }
        dual_constraint.b = objective[i];


        dual_problem->add_constraint(dual_constraint);
    }

    dual_problem->set_objective(dual_objective, ObjectiveType::MAXIMIZE);

    return *dual_problem;
}


void LPProblemSlack::convert()
{
    LPProblemGeneral::convert();
    
    // Handling no-sign solution components
    for (int i = 0; i < bounds.size(); i++) {
        if (bounds[i].type == BoundType::NO) {
            int component = bounds[i].component - 1;

            bounds[i].type = BoundType::NOT_NEGATIVE;
            bounds.push_back({ n + 1, BoundType::NOT_NEGATIVE });
            n += 1;

            // Modifying constraints
            for (auto& constraint : constraints) {
                constraint.coefficients.push_back(-constraint.coefficients[component]);
            }

            // Modifying objective
            objective.push_back(-objective[component]);
        }
    }

    // Handling inequality constraints
    for (int i = 0; i < constraints.size(); i++) {
        if (constraints[i].type == InequalityType::GREATER_EQUAL) {
            constraints[i].type = InequalityType::EQUAL;
            for (int j = 0; j < constraints.size(); j++) {
                if (j != i) {
                    constraints[j].coefficients.push_back(0);
                }
                else {
                    constraints[j].coefficients.push_back(-1);
                }
            }
            bounds.push_back({ n + 1, BoundType::NOT_NEGATIVE });
            objective.push_back(0);
            n += 1;
        }
    }
}


LPProblem& LPProblemSlack::dual() {
    vector<double> dual_objective;
    int m = constraints.size();

    LPProblemSlack* dual_problem = new LPProblemSlack(m);
    this->convert();
    


    return *dual_problem;
}
