#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "lp_problem_solver.h"
using std::vector;



template<typename T>
vector<T> subtract_vectors(vector<T> a, vector<T> b) {
    vector<T> res;
    for (int i = 0; i < a.size(); i++){
        if (std::find(b.begin(), b.end(), a[i]) == b.end()) {
            res.push_back(a[i]);
        }
    }
    return res;
}


void generate_combinations(const vector<int>& elements, int n, size_t start, vector<int>& current, vector<vector<int>>& result) {
    if (current.size() == n) {
        result.push_back(current);
        return;
    }

    for (size_t i = start; i < elements.size(); ++i) {
        current.push_back(elements[i]);
        generate_combinations(elements, n, i + 1, current, result);
        current.pop_back();
    }
}
vector<vector<int>> combinations(const vector<int>& elements, int n) {
    vector<vector<int>> result;
    vector<int> current;
    generate_combinations(elements, n, 0, current, result);
    return result;
}

template<typename T>
vector<T> concatenate_vectors(const vector<T>& vec1, const vector<T>& vec2) {
    vector<T> result;
    for (auto& el : vec1) {
        result.push_back(el);
    }
    for (auto& el : vec2) {
        result.push_back(el);
    }
    std::sort(result.begin(), result.end());
    return result;
}


class Matrix {
public:
    vector<vector<double>> matrix;
    int n;
    int m;
    vector<int> line_indexes;
    vector<int> column_indexes;

    Matrix(const vector<vector<double>>& matrix) {
        this->matrix = matrix;
        n = matrix.size();
        m = matrix[0].size();
        for (int i = 0; i < n; i++) {
            line_indexes.push_back(i);
        }
        for (int j = 0; j < m; j++) {
            column_indexes.push_back(j);
        }
    }
    Matrix(const vector<double>& vect) {
        for (auto& el : vect) {
            matrix.push_back(vector<double>(1, el));
        }

        n = matrix.size();
        m = matrix[0].size();
        for (int i = 0; i < n; i++) {
            line_indexes.push_back(i);
        }
        for (int j = 0; j < m; j++) {
            column_indexes.push_back(j);
        }
    }

    Matrix& transpose() {
        vector<vector<double>> t_matrix(m, vector<double>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                t_matrix[j][i] = matrix[i][j];
            }
        }
        Matrix* ret = new Matrix(t_matrix);

        ret->set_columns(line_indexes);
        ret->set_lines(column_indexes);
        return *ret;
    }

    void print() {
        for (auto& line_ind : line_indexes) {
            for (auto& col_ind : column_indexes) {
                std::cout << matrix[line_ind][col_ind] << " ";
            }
            std::cout << std::endl;
        }
    }
    void set_columns(vector<int> columns) {
        column_indexes = columns;
    }
    void set_lines(vector<int> lines) {
        line_indexes = lines;
    }

    Matrix& allocate_matrix(vector<int> lines, vector<int> columns) {
        vector<vector<double>> new_matrix(lines.size(), vector<double>(columns.size()));
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < columns.size(); j++) {
                new_matrix[i][j] = matrix[lines[i]][columns[j]];
            }
        }
        Matrix* ret = new Matrix(new_matrix);
        return *ret;
    }

    Matrix& multiply(const Matrix& a) { // on the right side
        vector<vector<double>> result(line_indexes.size(), vector<double>(a.column_indexes.size(), 0));

        for (int i = 0; i < line_indexes.size(); i++) {
            vector<double> line = matrix[line_indexes[i]];
            for (int k = 0; k < a.column_indexes.size(); k++) {

                for (int j = 0; j < column_indexes.size(); j++) {
                    result[i][k] += line[column_indexes[j]] * a.matrix[a.line_indexes[j]][a.column_indexes[k]];
                }
            }
        }
        Matrix* ans = new Matrix(result);
        return *ans;
    }
    Matrix& get_inverse_matrix() {
        if (n == 0 || line_indexes.size() != column_indexes.size()) {
            throw std::invalid_argument("Matrix must be square");
        }
        vector<vector<double>> augmented(line_indexes.size(), vector<double>(2 * line_indexes.size(), 0));

        for (size_t i = 0; i < line_indexes.size(); ++i) {
            for (size_t j = 0; j < column_indexes.size(); ++j) {
                augmented[i][j] = matrix[line_indexes[i]][column_indexes[j]];
            }
            augmented[i][i + line_indexes.size()] = 1;
        }

        for (size_t i = 0; i < line_indexes.size(); ++i) {
            size_t max_row = i;
            for (size_t k = i + 1; k < column_indexes.size(); ++k) {
                if (abs(augmented[k][i]) > abs(augmented[max_row][i])) {
                    max_row = k;
                }
            }

            if (max_row != i) {
                std::swap(augmented[i], augmented[max_row]);
            }

            if (augmented[i][i] == 0) {
                throw std::invalid_argument("Matrix is singular and cannot be inverted");
            }

            double pivot = augmented[i][i];
            for (size_t j = 0; j < 2 * line_indexes.size(); ++j) {
                augmented[i][j] /= pivot;
            }

            for (size_t k = 0; k < line_indexes.size(); ++k) {
                if (k != i) {
                    double factor = augmented[k][i];
                    for (size_t j = 0; j < 2 * line_indexes.size(); ++j) {
                        augmented[k][j] -= factor * augmented[i][j];
                    }
                }
            }
        }

        vector<vector<double>> inverse(line_indexes.size(), vector<double>(line_indexes.size(), 0));
        for (size_t i = 0; i < line_indexes.size(); ++i) {
            for (size_t j = 0; j < column_indexes.size(); ++j) {
                inverse[i][j] = augmented[i][j + line_indexes.size()];
            }
        }

        Matrix* inversed_matrix = new Matrix(inverse);

        return *inversed_matrix;
    }

    double determinant(vector<vector<double>> matrix) {
        if (matrix.size() == 0 || matrix[0].size() != matrix.size()) {
            throw std::invalid_argument("Matrix must be square");
        }

        if (matrix.size() == 1) {
            return matrix[0][0];
        }

        if (matrix.size() == 2) {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }

        if (matrix.size() == 3) {
            return matrix[0][0] * matrix[1][1] * matrix[2][2] +
                matrix[0][1] * matrix[1][2] * matrix[2][0] +
                matrix[0][2] * matrix[1][0] * matrix[2][1] -
                matrix[0][2] * matrix[1][1] * matrix[2][0] -
                matrix[0][0] * matrix[1][2] * matrix[2][1] -
                matrix[0][1] * matrix[1][0] * matrix[2][2];
        }

        double det = 0;
        for (size_t col = 0; col < matrix.size(); ++col) {
            vector<vector<double>> submatrix(matrix.size() - 1, vector<double>(matrix.size() - 1));
            for (size_t i = 1; i < matrix.size(); ++i) {
                size_t subcol = 0;
                for (size_t j = 0; j < matrix.size(); ++j) {
                    if (j == col) continue;
                    submatrix[i - 1][subcol] = matrix[i][j];
                    subcol++;
                }
            }

            double minor = determinant(submatrix);
            det += (col % 2 == 0 ? 1 : -1) * matrix[0][col] * minor;
        }

        return det;
    }

    vector<int> get_addition_to_square_matrix(vector<int>& available_indexes) {

        int need_to_add = line_indexes.size() - column_indexes.size();
        if (need_to_add <= 0) {
            return column_indexes;
        }
        vector<vector<int>> combs = combinations(available_indexes, need_to_add);
        for (auto& comb : combs) {

            if (determinant(
                allocate_matrix(
                    line_indexes,
                    concatenate_vectors<int>(column_indexes, comb)).matrix)
                != 0) {

                set_columns(concatenate_vectors<int>(column_indexes, comb));

                return column_indexes;
            }
        }

    }

    Matrix& subtract(Matrix& a) {
        vector<vector<double>> ans(line_indexes.size(), vector<double>(column_indexes.size()));
        for (int i = 0; i < line_indexes.size(); i++) {
            for (int j = 0; j < column_indexes.size(); j++) {
                ans[i][j] = matrix[line_indexes[i]][column_indexes[j]] - a.matrix[a.line_indexes[i]][a.column_indexes[j]];
            }
        }
        Matrix* ret = new Matrix(ans);
        return *ret;
    }

    bool operator==(const Matrix& rhs) {
        if (rhs.line_indexes.size() != line_indexes.size() || column_indexes.size() != rhs.column_indexes.size()) {
            return false;
        }
        for (int i = 0; i < line_indexes.size(); i++) {
            for (int j = 0; j < column_indexes.size(); j++) {
                if (matrix[line_indexes[i]][column_indexes[j]] != rhs.matrix[rhs.line_indexes[i]][rhs.column_indexes[j]]) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(const Matrix& rhs) { return !operator==(rhs); }
    bool operator< (const Matrix& rhs) {
        if (rhs.line_indexes.size() != line_indexes.size() || column_indexes.size() != rhs.column_indexes.size()) {
            return false;
        }
        for (int i = 0; i < line_indexes.size(); i++) {
            for (int j = 0; j < column_indexes.size(); j++) {
                if (matrix[line_indexes[i]][column_indexes[j]] >= rhs.matrix[rhs.line_indexes[i]][rhs.column_indexes[j]]) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator> (const Matrix& rhs) { return  operator< (rhs); }
    bool operator<=(const Matrix& rhs) { return !operator> (rhs); }
    bool operator>=(const Matrix& rhs) { return !operator< (rhs); }
    bool operator>(int num) {
        for (int i = 0; i < line_indexes.size(); i++) {
            for (int j = 0; j < column_indexes.size(); j++) {
                if (matrix[line_indexes[i]][column_indexes[j]] <= num) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator<(int num) {
        for (int i = 0; i < line_indexes.size(); i++) {
            for (int j = 0; j < column_indexes.size(); j++) {
                if (matrix[line_indexes[i]][column_indexes[j]] >= num) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator<=(int num) { return !operator> (num); }
    bool operator>=(int num) { return !operator< (num); }
};

vector<double> SimplexSolver::get_support_vector()
{
    return vector<double>();
}

LPProblemSolution& SimplexSolver::solve(LPProblem& problem)
{
    vector<double> support = { 1,1,124,1,1,1};
    
    vector<vector<double>> A_;
    for (auto& constr : problem.get_constraints()) {
        A_.push_back(constr.coefficients);
    }

    vector<int> n_plus, n_zero, n_all;
    for (int i = 0; i < support.size(); i++) {
        if (support[i] != 0) {
            n_plus.push_back(i);
        }
        else {
            n_zero.push_back(i);
        }
        n_all.push_back(i);
    }

    Matrix A(A_);
    Matrix X(support);


    A.set_columns(n_plus);


    vector<int> nk = A.get_addition_to_square_matrix(n_zero);

    vector<int> lk = subtract_vectors<int>(n_all, nk);

    Matrix c(problem.get_objective());
    
    A.set_columns(nk);

    Matrix B = A.get_inverse_matrix();
    
    Matrix cnk = c.allocate_matrix(nk, {0});
  
    A.set_columns(n_all);

    Matrix dkt = c.transpose().subtract(cnk.transpose().multiply(B.multiply(A)));
   

    dkt.print();
    if (dkt.allocate_matrix({0}, lk) >= 0) {
        LPProblemSolution* solution = new LPProblemSolution(Status::OPTIMAL, support, c.transpose().multiply(X).matrix[0][0]);
        return *solution;
    }

    LPProblemSolution* solution = new LPProblemSolution(Status::UNBOUNDED);
    return *solution;
}

void LPProblemSolution::print()
{
    std::cout << "\n\n\nSolution to the problem:\nSTATUS: "<< (status==Status::OPTIMAL ? "Optimal" : status == Status::INFEASABLE ? "Infeasable" : "Unbounded") << std::endl;
    std::cout << "X* = ( ";
    for (auto& el : solution) {
        std::cout << el << " ";
    }
    std::cout << ")" << std::endl;
    std::cout << "Objective value is: " << objective_value << std::endl;
}
