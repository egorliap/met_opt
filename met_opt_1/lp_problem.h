#include <stdlib.h>
#include <vector>
using std::vector;


enum class ObjectiveType { MINIMIZE, MAXIMIZE };
enum class InequalityType { LESS_EQUAL, GREATER_EQUAL, EQUAL };
enum class BoundType {NOT_NEGATIVE, NOT_POSITIVE, NO};

// Ограничение вида: a1*x1 + ... + an*xn ≤/≥/= b
struct Constraint {
	vector<double> coefficients;
	double b;
	InequalityType type;
};

// Ограничение на знак компоненты решения
struct VariableBound {
    int component;
    BoundType type;
};

// Базовый класс задачи ЛП
class LPProblem {
protected:
    int n;

    ObjectiveType objective_type;
    vector<double> objective;
    vector<Constraint> constraints;
    vector<VariableBound> bounds;
public:
    LPProblem();
    LPProblem(int n);
    LPProblem(vector<double> objective, ObjectiveType objective_type = ObjectiveType::MINIMIZE);
    void set_solution_dim(int n);
    void set_objective(const std::vector<double>& coeffs, ObjectiveType type);
    void add_constraint(const Constraint& c);
    void add_var_bound(const VariableBound& vb);
    void print_problem();
    virtual void convert() = 0;
    virtual LPProblem& dual() = 0;
};

// Общая задача ЛП
class LPProblemGeneral : public LPProblem {
public:
    using LPProblem::LPProblem;

    // Converts base linear problem to general
    void convert() override;

    // Converts general linear problem to its dual
    LPProblem& dual() override;
};

// Каноническая задача ЛП
class LPProblemSlack : public LPProblemGeneral {
public:
    using LPProblemGeneral::LPProblemGeneral;

    // Converts base (or general) linear problem to slack
    void convert() override;

    // Converts slack linear problem to its dual
    LPProblem& dual() override;
};