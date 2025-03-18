#pragma once
#include "function.h"

template <typename ArgType, typename RetType>
class ExtremumFinder
{
protected:
    int max_iter=10000;

public:
    ExtremumFinder(int max_iter){
        this->max_iter = max_iter;
    }
    virtual ArgType find(Function<ArgType, RetType> *funct, double a, double b, double eps, bool is_min = true) = 0;
};

class GoldenRatioExtremumFinder : public ExtremumFinder<double, double>
{
public:
    using ExtremumFinder<double, double>::ExtremumFinder;
    double find(Function<double, double> *funct, double a, double b, double eps, bool is_min = true) override;
};

class UniformSearchExtremumFinder : public ExtremumFinder<double, double>
{
public:
    using ExtremumFinder<double, double>::ExtremumFinder;
    double find(Function<double, double> *funct, double a, double b, double eps, bool is_min = true) override;
};
