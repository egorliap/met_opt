#include "solver.h"
#include <math.h>
#include <iostream>

double GoldenRatioExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
{
    double phi = (3 - sqrt(5)) / 2;
    double x1 = a, x2 = b, x1_new, x2_new;
    double y1, y2, t;
    int iter = 0;

    while (iter < max_iter)
    {
        t = (x2 - x1) * phi;

        x1_new = x1 + t;
        x2_new = x2 - t;

        y1 = funct->get_value(x1_new);
        y2 = funct->get_value(x2_new);
        if (abs(x1_new - x2_new) < eps)
        {
            return (x1_new + x2_new) / 2;
        }

        if ((y1 >= y2) == is_min)
        {
            x1 = x1_new;
        }
        else
        {
            x2 = x2_new;
        }

        iter++;
    }
    return b;
}

double UniformSearchExtremumFinder::find(Function<double, double> *funct, double a, double b, double eps, bool is_min)
{
    int n = static_cast<int>((b - a) / eps - 1);
    int iter = 0;
    int i = 0;
    double h = (b - a)/(n+1), y1, y2;
    while (iter < max_iter)
    {

        y1 = funct->get_value(a + i*h);
        y2 = funct->get_value(a + (i+1)*h);
        if ((y1 >= y2) == is_min){
            i += 1;
        }else{
            return a + (2*i+1)*h/2;
        }
    }
    return b;
}