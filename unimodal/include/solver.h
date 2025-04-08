#ifndef SOLVER_H
#define SOLVER_H
#include "function.h"
#include <utility>

template <typename ArgType, typename RetType>
class ExtremumFinder
{
protected:
    int max_iter=10000;

public:
    ExtremumFinder(int max_iter){
        this->max_iter = max_iter;
    }
    virtual int get_theor_calls_count(double a, double b, double eps) = 0;
    virtual std::pair<ArgType, ArgType> find(Function<ArgType, RetType> *funct, double a, double b, double eps, bool is_min = true) = 0;
};

class GoldenRatioExtremumFinder : public ExtremumFinder<double, double>
{
public:
    using ExtremumFinder<double, double>::ExtremumFinder;
    int get_theor_calls_count(double a, double b, double eps) override;
    std::pair<double, double> find(Function<double, double> *funct, double a, double b, double eps, bool is_min = true) override;
};

class UniformSearchExtremumFinder : public ExtremumFinder<double, double>
{
public:
    using ExtremumFinder<double, double>::ExtremumFinder;
    int get_theor_calls_count(double a, double b, double eps) override;
    std::pair<double, double> find(Function<double, double> *funct, double a, double b, double eps, bool is_min = true) override;
};

#endif