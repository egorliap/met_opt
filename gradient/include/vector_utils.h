#include <vector>
#include <stdexcept>
#include <cmath>

using std::vector;

vector<double> subtract(const vector<double> &left, const vector<double> &right);

vector<double> add(const vector<double> &left, const vector<double> &right);

vector<double> multiply(double alpha, const vector<double> &vec);

double dot(const vector<double> &left, const vector<double> &right);

double norm(const vector<double> &vec);

void print_vector(const vector<double> &vec, const std::string &name);

vector<vector<double>> get_inverse_matrix(vector<vector<double>>& matrix);

vector<double> matrix_vector_multiply(vector<vector<double>>& matrix, vector<double>& vec);

double determinant(vector<vector<double>>& matrix);