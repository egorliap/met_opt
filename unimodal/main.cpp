#include "function.h"
#include "solver.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>

double funct(double x)
{
    return x * x - 2 * x - 2 * cos(x);
}

void run_interface()
{
    std::pair<double, double> result;
    double a = 0.5, b = 1, exact = 0.5109734293;
    std::vector<double> epss = {1e-1, 1e-2, 1e-3};

    Function<double, double> f;
    f.set_function(*funct);

    GoldenRatioExtremumFinder gref(10000);
    UniformSearchExtremumFinder usef(10000);

    std::cout << "Calculated extremum (exact form): x* = " << exact << std::endl;
    for (int i = 0; i < epss.size(); i++)
    {
        double eps = epss[i];

        result = gref.find(&f, a, b, eps);
        std::cout << std::fixed;
        std::cout << std::setprecision(i + 1);
        std::cout << "Result of golden ratio method for eps=" << eps << ": " << "x1: " << result.first << " x2: " << result.first;

        std::cout << std::fixed;
        std::cout << std::setprecision(i + 3);
        std::cout << "\treal error is: " << abs(exact - (result.first + result.second) / 2) << std::endl;

        std::cout << std::fixed;
        std::cout << std::setprecision(i + 1);
        std::cout << "Number of calls of function: " << f.get_counter()
                  << std::endl;
        std::cout << "Theoretical number of calls of function: " << gref.get_theor_calls_count(0.5, 1, eps) << "\n"
                  << std::endl;
        f.reset_counter();

        result = usef.find(&f, a, b, eps);
        std::cout << "Result of uniform search method for eps=" << eps << ": " << "x1: " << result.first << " x2: " << result.first;
        std::cout << std::fixed;
        std::cout << std::setprecision(i + 3);
        std::cout << "\treal error is: " << abs(exact - (result.first + result.second) / 2) << std::endl;
        std::cout << "Number of calls of function: " << f.get_counter()
                  << std::endl;
        // std::cout << "Theoretical number of calls of function: " << usef.get_theor_calls_count(a, b, eps) << "\n\n\n\n"
        //           << std::endl;
        std::cout << "\n\n\n\n"
                  << std::endl;
        f.reset_counter();
    }
}

int main()
{
    run_interface();

    return 0;
}