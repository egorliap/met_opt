#include "function.h"
#include "solver.h"
#include <iostream>
#include <math.h>
#include <vector>

double funct(double x)
{
    return x * x - 2 * x - 2 * cos(x);
}

void run_interface()
{
    double result, a = 0.5, b = 1, exact = 0.5109734293;
    std::vector<double> epss = {1e-1, 1e-2, 1e-3, 1e-5};

    Function<double, double> f;
    f.set_function(*funct);

    GoldenRatioExtremumFinder gref(1000);
    UniformSearchExtremumFinder usef(1000);

    std::cout << "Calculated extremum (exact form): x* = " << exact << std::endl;
    for (auto &eps : epss)
    {
        result = gref.find(&f, a, b, eps);

        std::cout << "Result of golden ratio method for eps=" << eps << ": " << result << "\treal error is: " << abs(exact - result) << std::endl;
        std::cout << "Number of calls of function: " << f.get_counter()
                  << std::endl;
        std::cout << "Theoretical number of calls of function: " << f.get_counter() << "\n"
                  << std::endl;
        f.reset_counter();

        result = usef.find(&f, a, b, eps);
        std::cout << "Result of uniform search method for eps=" << eps << ": " << result << "\treal error is: " << abs(exact - result) << std::endl;
        std::cout << "Number of calls of function: " << f.get_counter()
                  << std::endl;
        std::cout << "Theoretical number of calls of function: " << f.get_counter() << "\n\n\n\n"
                  << std::endl;
        f.reset_counter();
    }
}

int main()
{
    run_interface();

    return 0;
}