#include <vector>
#include <cmath>
#include "function.h"
#include "solver.h"
#include "vector_utils.h"
#include <iomanip>

// e^{-(x_1^2 + x_2^2 + ..)}*||x||_2
double f(std::vector<double> &x)
{
    double sq_sum = 0;
    for (size_t i = 0; i < x.size(); i++)
    {
        sq_sum += x[i] * x[i];
    }

    return exp(sq_sum) * (sq_sum);
}

int main()
{
    MultivarFunction func(2);
    func.set_function(f);
    std::vector<double> x({-1, 1});

    StepSplittingGradientMethod ss_solver(10000);
    NewtonMethod nm_solver(10000);
    HookJeevesMethod hj_solver(10000);

    vector<double> epss = {0.1, 0.01, 0.001};

    vector<double> x_real = {0, 0};

    for (int i = 0; i < epss.size(); i++)
    {

        std::pair<vector<double>, double> ans = nm_solver.find(func, x, epss[i], 1);

        std::cout << "epsilon = " << epss[i] << std::endl;

        std::cout << std::fixed;
        std::cout << std::setprecision(i + 2);

        std::cout << "f(x*) = " << ans.second << std::endl;
        vector<double> res_x(ans.first);
        print_vector(res_x, "x*");

        auto sub = subtract(res_x, x_real);
        std::cout << "||x*_real - x*|| = " << norm(sub) << std::endl;

        std::cout << "\n\n";
    }
    return 0;
}