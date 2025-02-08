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
    LPProblem(int n);
    LPProblem(vector<double> objective, ObjectiveType objective_type = ObjectiveType::MINIMIZE);
    void set_objective(const std::vector<double>& coeffs, ObjectiveType type);
    void add_constraint(const Constraint& c);
    void add_var_bound(const VariableBound& vb);
    void print_problem();
    virtual void convert()=0;
};

// Общая задача ЛП
class LPProblemGeneral : public LPProblem {
public:
    using LPProblem::LPProblem;
    void convert() override;
};

// Каноническая задача ЛП
class LPProblemSlack : public LPProblemGeneral {
public:
    using LPProblemGeneral::LPProblemGeneral;
    void convert() override;
};