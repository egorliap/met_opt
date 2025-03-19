#include "solver.h"
#include <math.h>
#include <iostream>

double GoldenRatioExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
{
    double phi = (3 - sqrt(5)) / 2;
    double x1, x2;
    double y1, y2, t;
    int iter = 0;

    t = (b - a) * phi;

    x1 = a + t;
    x2 = b - t;

    y1 = funct->get_value(x1);
    y2 = funct->get_value(x2);

    while (iter < max_iter)
    {

        if (abs(x1 - x2) < eps)
        {
            return (x1 + x2) / 2;
        }

        if ((y1 >= y2) == is_min)
        {
            a = x1;
            t = (b - a) * phi;
            x1 = x2;
            x2 = b - t;
            y1 = y2;
            y2 = funct->get_value(x2);
        }
        else
        {
            b = x2;
            t = (b - a) * phi;
            x2 = x1;
            x1 = a + t;
            y2 = y1;
            y1 = funct->get_value(x1);
        }

        iter++;
    }
    return b;
}

int GoldenRatioExtremumFinder::get_theor_calls_count(double a, double b, double eps)
{
    double phi = (1 + sqrt(5)) / 2;
    return static_cast<int>(ceil(log((b - a) / eps) / abs(log(1/phi)))) + 1;
}

double UniformSearchExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
{
    int n = static_cast<int>((b - a) / eps - 1);
    int iter = 0;
    int i = 0;
    double h = (b - a) / (n + 1), y1, y2;
    while (iter < max_iter)
    {

        y1 = funct->get_value(a + i * h);
        y2 = funct->get_value(a + (i + 1) * h);
        if ((y1 >= y2) == is_min)
        {
            i += 1;
        }
        else
        {
            return a + (2 * i + 1) * h / 2;
        }
    }
    return b;
}

int UniformSearchExtremumFinder::get_theor_calls_count(double a, double b, double eps)
{
    return static_cast<int>((b-a)/eps) - 1;
}