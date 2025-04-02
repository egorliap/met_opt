#pragma once
#include <vector>
#include <iostream>

using namespace std;

template <typename ArgType, typename RetType>
class Function
{
protected:
    RetType (*function)(ArgType);
    int counter;

public:
    Function()
    {
        counter = 0;
    }
    void set_function(RetType (*funct)(ArgType))
    {
        function = funct;
    }

    void reset_counter()
    {
        counter = 0;
    }

    int get_counter()
    {
        return counter;
    }

    RetType get_value(ArgType x)
    {
        counter += 1;
        return function(x);
    }
};

class MultivarFunction : public Function<std::vector<double> &, double>
{
private:
    int arg_dim;

public:
    MultivarFunction(int arg_dim)
    {
        this->arg_dim = arg_dim;
    }

    double partial_derivative(vector<double> &x, int j, double dx = 1e-10)
    {
        if (j > arg_dim - 1)
        {
            std::cerr << "j'th component out of vector's size" << std::endl;
            return 1;
        }
        vector<double> xdx(x);
        xdx[j] += dx;
        double y1 = function(xdx);
        double y2 = function(x);
        double delta_y = y1 - y2;
        return delta_y / dx;
    }

    double partial_second_derivative(vector<double> &x, int j1, int j2, double dx = 1e-10)
    {
        if (j1 > arg_dim - 1 || j2 > arg_dim - 1)
        {
            std::cerr << "j'th component out of vector's size" << std::endl;
        }
        vector<double> xdx11(x);
        vector<double> xdx22(x);
        vector<double> xdx12(x);
        vector<double> xdx21(x);

        xdx11[j1] += dx;
        xdx11[j2] += dx;

        xdx12[j1] += dx;
        xdx12[j2] -= dx;

        xdx21[j1] -= dx;
        xdx21[j2] += dx;

        xdx22[j1] -= dx;
        xdx22[j2] += dx;

        double y11 = function(xdx11);
        double y12 = function(xdx12);
        double y21 = function(xdx21);
        double y22 = function(xdx22);

        double delta_y = y11 - y12 - y21 + y22;
        return delta_y / (4 * dx * dx);
    }

    // вычисление градиента функции в точке
    vector<double> gradient(vector<double> &x, double eps = 1e-10)
    {
        vector<double> grad;

        for (int i = 0; i < arg_dim; i++)
        {
            double dy = partial_derivative(x, i, eps);
            grad.push_back(dy);
        }

        return grad;
    }

    vector<vector<double>> hessian(vector<double> &x, double eps = 1e-10)
    {
        vector<vector<double>> ans(arg_dim, vector<double>(arg_dim));
        for (int i = 0; i < arg_dim; i++){
            for (int j = 0; j < arg_dim; j++){
                ans[i][j] = partial_second_derivative(x, i, j);
            }
        }
        return ans;
    }
};