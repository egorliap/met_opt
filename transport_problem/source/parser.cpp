#include "include/parser.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

TransportProblem *TransportProblemParser::parse(const string &filename)
{
    ifstream file(filename);
    if (!file)
        throw runtime_error("Cannot open file: " + filename);

    string line;

    // Read n and m
    getline(file, line);
    auto tokens = split(line);
    if (tokens.size() != 2)
        throw runtime_error("Invalid n m format");
    int n = stoi(tokens[0]), m = stoi(tokens[1]);

    vector<vector<double>> matrix;
    for (int i = 0; i < n; ++i)
    {
        getline(file, line);
        tokens = split(line);
        if (tokens.size() != m)
            throw runtime_error("Invalid constraint format");

        vector<double> coeffs(m);
        transform(tokens.begin(), tokens.begin() + m, coeffs.begin(), [](const string &s)
                  { return stod(s); });

        matrix.push_back(coeffs);
    }

    vector<double> providers(n);

    getline(file, line);
    tokens = split(line);
    if (tokens.size() != n)
        throw runtime_error("Invalid constraint format");
    transform(tokens.begin(), tokens.begin() + n, providers.begin(), [](const string &s)
              { return stod(s); });

    vector<double> consumers(m);

    getline(file, line);
    tokens = split(line);
    if (tokens.size() != m)
        throw runtime_error("Invalid constraint format");
    transform(tokens.begin(), tokens.begin() + m, consumers.begin(), [](const string &s)
              { return stod(s); });

    TransportProblem* problem = new TransportProblem(matrix, providers, consumers);
    return problem;
}

std::vector<std::string> TransportProblemParser::split(const std::string &s)
{
    vector<string> tokens;
    istringstream iss(s);
    string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}
