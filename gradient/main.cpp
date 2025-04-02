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

    return 4*x[0]*x[0] + x[1]*x[1] + cos(3*x[0] + 3*x[1]) - x[0] + 2*x[1];
}

void run_interface()
{
    int choice;
    MultivarFunction func(2);
    func.set_function(f);
    std::vector<double> x({0.01, 1.22});
    ExtremumFinder<vector<double>, double> *solver;
    vector<double> epss = {0.1, 0.01, 0.001};

    vector<double> x_real = {-1 / sqrt(2), 0};
    double f_real = f(x_real);
    while (true)
    {
        std::cout << "Choose method:\n 1 - split step gradient method\n 2 - Newton method\n 3 - Hook-Jeevs method" << std::endl;
        std::cin >> choice;
        if (choice == 1)
        {
            solver = new StepSplittingGradientMethod(10000);
        }
        else if (choice == 2)
        {
            solver = new NewtonMethod(10000);
        }
        else if (choice == 3)
        {
            solver = new HookJeevesMethod(10000);
        }
        else if (choice == 4)
        {
            solver = new StepSplittingGradientMethod(10000);
            std::pair<vector<double>, double> ans = solver->find(func, x, epss[0], 1);
            ans = solver->find(func, ans.first, epss[2], 1);

            std::cout << "epsilon = " << epss[2] << std::endl;

            std::cout << std::fixed;
            std::cout << std::setprecision(3);

            std::cout << "f(x*) = " << ans.second << std::endl;
            vector<double> res_x(ans.first);
            print_vector(res_x, "x*");

            auto sub = subtract(res_x, x_real);
            std::cout << "||x*_real - x*|| = " << norm(sub) << std::endl;
            std::cout << "||f*_real - f(x*)|| = " << fabs(f_real - f(res_x)) << std::endl;

            std::cout << "\n\n";
            continue;
        }
        else
        {
            std::cout << "choose 1, 2 or 3" << std::endl;
            continue;
        }

        for (int i = 0; i < epss.size(); i++)
        {
            std::pair<vector<double>, double> ans = solver->find(func, x, epss[i], 1);

            std::cout << "epsilon = " << epss[i] << std::endl;

            std::cout << std::fixed;
            std::cout << std::setprecision(i + 2);

            std::cout << "f(x*) = " << ans.second << std::endl;
            vector<double> res_x(ans.first);
            print_vector(res_x, "x*");

            auto sub = subtract(res_x, x_real);
            std::cout << "||x*_real - x*|| = " << norm(sub) << std::endl;
            std::cout << "||f*_real - f(x*)|| = " << fabs(f_real - f(res_x)) << std::endl;

            std::cout << "\n\n";
        }
    }
}

int main()
{
    run_interface();
    return 0;
}