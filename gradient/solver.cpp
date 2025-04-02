#include <utility>
#include "solver.h"
#include "vector_utils.h"

std::pair<std::vector<double>, double> StepSplittingGradientMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    int iter = 0;
    vector<double> d = funct.gradient(x0);
    vector<double> x_cur(x0);
    vector<double> x_next;
    double step = alpha;
    bool success = false;
    double cur_f;

    while (iter < max_iter)
    {
        success = false;
        cur_f = funct.get_value(x_cur);
        d = funct.gradient(x_cur);
        if (norm(d) < eps)
        {
            return {x_cur, cur_f};
        }

        while (!success)
        {
            x_next = subtract(x_cur, multiply(step, d));

            if (cur_f > funct.get_value(x_next))
            {
                x_cur = x_next;
                success = true;
            }
            else
            {
                step = step / 2;
            }
        }
        iter++;
    }
    return {x_cur, cur_f};
}

std::pair<std::vector<double>, double> NewtonMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    int iter = 0;

    vector<double> g = funct.gradient(x0);
    vector<vector<double>> h = funct.hessian(x0);

    vector<double> x_cur(x0);
    vector<double> x_new;
    double cur_f;
    
    while (iter < max_iter)
    {
        cur_f = funct.get_value(x_cur);
        g = funct.gradient(x_cur);
        h = funct.hessian(x_cur);
        
        if (norm(g) < eps)
        { 
            return {x_cur, cur_f};
        }
        double det = determinant(h);
        if (det < 1e-5)
        {
            x_new = subtract(x_cur, multiply(1e-2, g));
        }
        else
        {
            vector<vector<double>> inv_h = get_inverse_matrix(h);
            vector<double> mult = matrix_vector_multiply(inv_h, g);
            x_new = subtract(x_cur, mult);
        }
        x_cur = x_new;
        iter++;
    }
    return {x_cur, cur_f};
}

std::pair<std::vector<double>, double> HookJeevesMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    return std::pair<std::vector<double>, double>();
}