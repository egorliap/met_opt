#include "vector_utils.h"
#include <iostream>
using std::vector;

vector<double> subtract(const vector<double> &left, const vector<double> &right)
{
    if (left.size() != right.size())
    {

        throw std::invalid_argument("vectors must be the same length");
    }

    vector<double> res;

    for (int i = 0; i < right.size(); i++)
    {
        res.push_back(left[i] - right[i]);
    }
    return res;
}

vector<double> add(const vector<double> &left, const vector<double> &right)
{
    if (left.size() != right.size())
    {
        throw std::invalid_argument("vectors must be the same length");
    }
    vector<double> res;
    for (int i = 0; i < right.size(); i++)
    {
        res.push_back(left[i] + right[i]);
    }
    return res;
}

vector<double> multiply(double alpha, const vector<double> &vec)
{
    vector<double> res;
    for (int i = 0; i < vec.size(); i++)
    {
        res.push_back(alpha * vec[i]);
    }
    return res;
}

double dot(const vector<double> &left, const vector<double> &right)
{
    if (left.size() != right.size())
    {
        throw std::invalid_argument("vectors must be the same length");
    }
    double res = 0;
    for (int i = 0; i < right.size(); i++)
    {
        res += left[i] * right[i];
    }
    return res;
}

double norm(const vector<double> &vec)
{
    double res = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        res += vec[i] * vec[i];
    }

    return sqrt(res);
}

void print_vector(const vector<double> &vec, const std::string &name)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (i == static_cast<int>(vec.size() / 2) - 1)
        {
            std::cout << name << " = ";
        }
        else
        {
            for (int j = 0; j < name.size() + 3; j++)
            {
                std::cout << " ";
            }
        }
        if (vec[i] > 0)
        {
            std::cout << " ";
        }
        std::cout << vec[i] << std::endl;
    }
}

vector<vector<double>> get_inverse_matrix(vector<vector<double>>& matrix)
{
    vector<vector<double>> augmented(matrix.size(), vector<double>(2 * matrix.size(), 0));

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix.size(); ++j)
        {
            augmented[i][j] = matrix[i][j];
        }
        augmented[i][i + matrix.size()] = 1;
    }

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        size_t max_row = i;
        for (size_t k = i + 1; k < matrix.size(); ++k)
        {
            if (abs(augmented[k][i]) > abs(augmented[max_row][i]))
            {
                max_row = k;
            }
        }

        if (max_row != i)
        {
            std::swap(augmented[i], augmented[max_row]);
        }

        if (augmented[i][i] == 0)
        {
            throw std::invalid_argument("Matrix is singular and cannot be inverted");
        }

        double pivot = augmented[i][i];
        for (size_t j = 0; j < 2 * matrix.size(); ++j)
        {
            augmented[i][j] = (augmented[i][j] / pivot);
        }

        for (size_t k = 0; k < matrix.size(); ++k)
        {
            if (k != i)
            {
                double factor = augmented[k][i];
                for (size_t j = 0; j < 2 * matrix.size(); ++j)
                {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    vector<vector<double>> inverse(matrix.size(), vector<double>(matrix.size(), 0));
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix.size(); ++j)
        {
            inverse[i][j] = augmented[i][j + matrix.size()];
        }
    }
    return inverse;
}

// Детерминант (рекурсивная реализация)
double determinant(vector<vector<double>>& matrix)
{
    if (matrix.size() == 0 || matrix[0].size() != matrix.size())
    {
        throw std::invalid_argument("Matrix must be square");
    }

    if (matrix.size() == 1)
    {
        return matrix[0][0];
    }

    if (matrix.size() == 2)
    {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    if (matrix.size() == 3)
    {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] +
               matrix[0][1] * matrix[1][2] * matrix[2][0] +
               matrix[0][2] * matrix[1][0] * matrix[2][1] -
               matrix[0][2] * matrix[1][1] * matrix[2][0] -
               matrix[0][0] * matrix[1][2] * matrix[2][1] -
               matrix[0][1] * matrix[1][0] * matrix[2][2];
    }

    double det = 0;
    for (size_t col = 0; col < matrix.size(); ++col)
    {
        vector<vector<double>> submatrix(matrix.size() - 1, vector<double>(matrix.size() - 1));
        for (size_t i = 1; i < matrix.size(); ++i)
        {
            size_t subcol = 0;
            for (size_t j = 0; j < matrix.size(); ++j)
            {
                if (j == col)
                    continue;
                submatrix[i - 1][subcol] = matrix[i][j];
                subcol++;
            }
        }

        double minor = determinant(submatrix);
        det += (col % 2 == 0 ? 1 : -1) * matrix[0][col] * minor;
    }

    return det;
}

vector<double> matrix_vector_multiply(vector<vector<double>>& matrix, vector<double>& vec)
{
    vector<double> result(matrix.size(), 0);

    for (int i = 0; i < matrix.size(); i++)
    {
        vector<double> line = matrix[i];
        for (int k = 0; k < vec.size(); k++)
        {
            result[i] += (line[k] * vec[k]);
        }
    }
    return result;
}