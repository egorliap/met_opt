#ifndef SOLVER_CPP
#define SOLVER_CPP
#include "include/solver.h"
#include <math.h>
#include <utility>
#include <iostream>

std::pair<double, double> GoldenRatioExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
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

        if (fabs(a - b) < eps)
        {
            return {x1, x2};
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
    return {b - eps, b};
}

int GoldenRatioExtremumFinder::get_theor_calls_count(double a, double b, double eps)
{
    double phi = (1 + sqrt(5)) / 2;
    return static_cast<int>(ceil( -log(eps / (b - a)) / log(phi) )) + 2;
}

std::pair<double, double> UniformSearchExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
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
            // y1 = y2;
        }
        else
        {
            return {a + i * h, a + (i + 1) * h};
        }
    }
    return {b - i * h, b};
}

int UniformSearchExtremumFinder::get_theor_calls_count(double a, double b, double eps)
{
    return static_cast<int>((b - a) / eps) - 1;
}
#endif