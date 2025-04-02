#ifndef SOLVER_H
#define SOLVER_H
#include "function.h"
#include <utility>

template <typename ArgType, typename RetType>
class ExtremumFinder
{
protected:
    int max_iter = 10000;

public:
    ExtremumFinder(int max_iter)
    {
        this->max_iter = max_iter;
    }
    ExtremumFinder(const ExtremumFinder &) = delete;


    virtual std::pair<ArgType, RetType> find(MultivarFunction &funct, ArgType &x0, double eps, double alpha = 1) = 0;
};

class StepSplittingGradientMethod : public ExtremumFinder<std::vector<double>, double>
{
public:
    using ExtremumFinder<std::vector<double>, double>::ExtremumFinder;

    std::pair<std::vector<double>, double> find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha = 1) override;
};

class NewtonMethod : public ExtremumFinder<std::vector<double>, double>
{
public:
    using ExtremumFinder<std::vector<double>, double>::ExtremumFinder;

    std::pair<std::vector<double>, double> find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha = 1) override;
};

class HookJeevesMethod : public ExtremumFinder<std::vector<double>, double>
{
public:
    using ExtremumFinder<std::vector<double>, double>::ExtremumFinder;

    std::pair<std::vector<double>, double> find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha = 1) override;
};

#endif