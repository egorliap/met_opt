#pragma once

#include "problem.h"
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class TransportProblemParser
{
public:
    TransportProblem *parse(const string &filename);

private:
    static std::vector<std::string> split(const std::string &s);
};