#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "lp_problem_solver.h"
#include <cmath>
#include <iomanip>
#include <string>

using std::pair;
using std::vector;

// first iteration to reference vector
vector<double> SimplexSolver::artificial_basis_method(vector<Constraint> &constraints)
{
    int m = constraints.size();
    int n = constraints[0].coefficients.size();

    vector<vector<double>> A(m, vector<double>(n + m, 0));
    vector<double> b(m, 0);
    vector<double> c(n + m, 0);

    vector<Constraint> tmp(constraints);

    vector<double> solution(n + m, 0);
    for (int i = 0; i < m; i++)
    {

        for (int j = 0; j < n; j++)
        {
            A[i][j] = constraints[i].coefficients[j];
        }
        b[i] = constraints[i].b;
        if (b[i] < 0)
        {
            b[i] *= -1;
            for (int j = 0; j < n; j++)
            {
                A[i][j] *= -1;
            }
            A[i][n + i] = -1;
        }
        else
        {
            A[i][n + i] = 1;
        }
        c[n + i] = 1;
        solution[n + i] = b[i];
    }

    LPProblem *problem = new LPProblemSlack(m + n);
    problem->set_objective(c, ObjectiveType::MINIMIZE);
    for (int i = 0; i < m; i++)
    {

        Constraint constraint;
        constraint.coefficients = A[i];
        constraint.b = b[i];
        constraint.type = InequalityType::EQUAL;
        problem->add_constraint(constraint);
    }

    for (int i = 0; i < n + m; i++)
    {
        problem->add_var_bound({i + 1, BoundType::NOT_NEGATIVE});
    }
    problem->print_problem();
    LPProblemSolution artificial_solution = solve(*problem, solution);

    bool infeasible = false;
    for (int i = n; i < n + m; ++i)
    {
        if (artificial_solution.solution[i] < 0)
        { // Учет вычислительной погрешности
            infeasible = true;
            break;
        }
    }
    if (infeasible)
    {
        throw std::runtime_error("Problem is infeasible");
    }

    vector<double> ans;
    for (int i = 0; i < n; i++)
    {
        ans.push_back(artificial_solution.solution[i]);
    }

    return ans;
}

LPProblemSolution &SimplexSolver::solve(LPProblem &problem, vector<double> support)
{
    int m;

    if (support.empty())
    {
        auto constraints = problem.get_constraints();
        support = artificial_basis_method(constraints);
    }
    std::cin >> m;
    Matrix X(support);

    vector<vector<double>> A_;
    for (auto &constr : problem.get_constraints())
    {
        A_.push_back(constr.coefficients);
    }
    int iter = 0;

    Matrix A(A_);
    std::cout << "A[M, N]: ";
    A.print();
    vector<int> nk;
    vector<int> jk_used;

    vector<pair<int, int>> used_in_basis_change;
    bool basis_changed = false;
    while (true)
    {
        std::cin.get();

        std::cout << "ITERATION NUMBER " << iter << "\n\n";
        if (!basis_changed)
        {
            used_in_basis_change.clear();
        }

        std::cout << "USED IN BASIS:\n";
        for (auto &p : used_in_basis_change)
        {
            std::cout << "(" << p.first << ", " << p.second << ")\n";
        }

        std::cout << "X: ";
        X.print();

        vector<int> n_plus, n_zero, n_all;
        for (int i = 0; i < X.matrix.size(); i++)
        {
            if (X.matrix[i][0] != 0)
            {
                n_plus.push_back(i);
            }
            else
            {
                n_zero.push_back(i);
            }
            n_all.push_back(i);
        }
        std::cout << "n_plus: ";
        print_vector(n_plus);
        std::cout << "n_zero: ";
        print_vector(n_zero);
        std::cout << "n_all: ";
        print_vector(n_all);

        A.set_columns(n_plus);
        std::cout << "A[M, N+]: ";
        A.print();
        if (nk.empty() || !basis_changed)
        {
            nk = A.get_addition_to_square_matrix(n_zero);
        }
        else
        {
            A.set_columns(nk);
        }
        std::cout << "nk: ";
        print_vector(nk);
        std::cout << "A[M, Nk]: ";
        A.print();

        vector<int> lk = subtract_vectors<int>(n_all, nk);
        std::cout << "lk: ";
        print_vector(lk);

        Matrix c(problem.get_objective());

        Matrix B = A.get_inverse_matrix();
        std::cout << "B[Nk, M]: ";
        B.print();

        Matrix cnk = c.allocate_matrix(nk, {0});

        A.set_columns(n_all);
        std::cout << "A[M, N]: ";
        A.print();

        Matrix dkt = c.transpose().subtract(cnk.transpose().multiply(B.multiply(A)));

        std::cout << "dkt: ";
        dkt.print();

        // // Вывод симплекс-таблицы
        // std::cout << "\nSimplex Tableau:\n";

        // // Восстановить все столбцы в A
        // A.set_columns(n_all);

        // // Вычислить коэффициенты таблицы: B * A
        // Matrix tableau_coeff = B.multiply(A);

        // // Получить RHS из X для базисных переменных
        // vector<double> rhs;
        // for (int i : nk)
        // {
        //     rhs.push_back(X.matrix[i][0]);
        // }

        // // Определить ширину каждого столбца
        // vector<size_t> column_widths(tableau_coeff.column_size() + 2, 0); // +2 для Basis и RHS

        // // Ширина столбца Basis
        // column_widths[0] = 5; // "Basis" занимает 5 символов

        // // Ширина столбцов переменных
        // for (int j = 0; j < tableau_coeff.column_size(); ++j)
        // {
        //     column_widths[j + 1] = 6; // "x1", "x2", и т.д. + числа
        //     for (size_t i = 0; i < tableau_coeff.matrix.size(); ++i)
        //     {
        //         size_t value_length = std::to_string(tableau_coeff.matrix[i][j]).length();
        //         if (value_length > column_widths[j + 1])
        //         {
        //             column_widths[j + 1] = value_length;
        //         }
        //     }
        // }

        // // Ширина столбца RHS
        // column_widths.back() = 6; // "RHS" занимает 6 символов
        // for (double value : rhs)
        // {
        //     size_t value_length = std::to_string(value).length();
        //     if (value_length > column_widths.back())
        //     {
        //         column_widths.back() = value_length;
        //     }
        // }

        // // Вывести заголовок таблицы
        // std::cout << std::setw(column_widths[0]) << "Basis" << " | ";
        // for (int j = 0; j < tableau_coeff.column_size(); ++j)
        // {
        //     std::cout << std::setw(column_widths[j + 1]) << "x" << (j + 1) << " | ";
        // }
        // std::cout << std::setw(column_widths.back()) << "RHS" << "\n";

        // // Вывод разделителя
        // for (size_t width : column_widths)
        // {
        //     std::cout << std::string(width + 2, '-') << "+";
        // }
        // std::cout << "\n";

        // // Вывод строк базисных переменных
        // for (size_t i = 0; i < nk.size(); ++i)
        // {
        //     std::cout << std::setw(column_widths[0]) << "x" << (nk[i] + 1) << " | ";
        //     for (int j = 0; j < tableau_coeff.column_size(); ++j)
        //     {
        //         std::cout << std::setw(column_widths[j + 1]) << std::fixed << std::setprecision(2) << tableau_coeff.matrix[i][j] << " | ";
        //     }
        //     std::cout << std::setw(column_widths.back()) << rhs[i] << "\n";
        // }

        // // Вывод целевой строки
        // double obj_value = c.transpose().multiply(X).matrix[0][0];
        // std::cout << std::setw(column_widths[0]) << "z" << " | ";
        // for (int j = 0; j < dkt.matrix[0].size(); ++j)
        // {
        //     std::cout << std::setw(column_widths[j + 1]) << std::fixed << std::setprecision(2) << dkt.matrix[0][j] << " | ";
        // }

        // std::cout << "\n";

        // std::cout << std::setw(column_widths.back()) << obj_value << "\n\n";
        // // Вывод разделителя
        // for (size_t width : column_widths)
        // {
        //     std::cout << std::string(width + 2, '-') << "+";
        // }
        // std::cin.get();

        Matrix dklkt = dkt.allocate_matrix({0}, lk);

        std::cout << "dklkt: ";
        dklkt.print();

        if (dklkt >= 0)
        {
            LPProblemSolution *solution = new LPProblemSolution(Status::OPTIMAL, X.transpose().matrix[0], c.transpose().multiply(X).matrix[0][0]);
            return *solution;
        }

        vector<int> jk;
        for (int i = 0; i < dkt.matrix[0].size(); i++)
        {
            if (dkt.matrix[0][i] < 0)
            {
                jk.push_back(i);
                break;
            }
        }
        
        std::cout << "jk: ";
        print_vector(jk);

        A.set_columns(jk);
        std::cout << "A[M, jk]: ";
        A.print();

        Matrix BA = B.multiply(A);
        std::cout << "BA: ";
        BA.print();

        vector<double> uk_(n_all.size(), 0);
        for (int i = 0; i < BA.matrix.size(); i++)
        {
            uk_[nk[i]] = BA.matrix[i][0];
        }
        for (auto &j : jk)
        {
            uk_[j] = -1;
        }
        Matrix uk(uk_);

        std::cout << "uk: ";
        uk.print();

        if (uk.allocate_matrix(nk, {0}) <= 0)
        {
            LPProblemSolution *solution = new LPProblemSolution(Status::UNBOUNDED);
            return *solution;
        }

        vector<int> i_;
        for (auto &i : nk)
        {
            if (uk.matrix[i][0] > 0)
            {
                i_.push_back(i);
            }
        }
        std::cout << "i: ";
        print_vector(i_);

        if (nk == n_plus || subtract_vectors<int>(nk, n_plus).empty() || uk.allocate_matrix(subtract_vectors<int>(nk, n_plus), {0}) <= 0)
        {
            double theta_k = 100000000000;
            for (auto &i : i_)
            {
                if (theta_k > X.matrix[i][0] / uk.matrix[i][0])
                {
                    theta_k = X.matrix[i][0] / uk.matrix[i][0];
                }
            }
            std::cout << "theta: " << theta_k << std::endl;

            X = X.subtract(uk * theta_k);
            basis_changed = false;
            jk_used.clear();
        }
        else
        {
            bool ext = false;
            vector<int> choice = subtract_vectors<int>(nk, n_plus);
            std::cout << "choice: ";
            print_vector(choice);
            std::cout << "lk: ";
            print_vector(lk);

            for (int i = 0; i < lk.size(); i++)
            {
                int l = lk[i];
                for (int j = 0; j < choice.size(); j++)
                {
                    int n = choice[j];

                    for (auto &p : used_in_basis_change)
                    {

                        if (p.first == n && p.second == l)
                        {
                            ext = true;
                            break;
                        }
                        ext = false;
                    }
                    if (ext)
                    {
                        ext = false;
                        continue;
                    }
                    else
                    {
                        vector<int> columns = concatenate_vectors<int>({l}, subtract_vectors<int>(nk, {n}));
                        A.set_columns(columns);

                        if (A.determinant(A.allocate_matrix(A.line_indexes, A.column_indexes).matrix) != 0)
                        {
                            nk = columns;

                            A.set_columns(nk);
                            used_in_basis_change.push_back(pair<int, int>(l, n));
                            used_in_basis_change.push_back(pair<int, int>(n, l));

                            std::cout << "CHANGED" << std::endl;

                            ext = true;
                            basis_changed = true;
                            break;
                        }
                    }
                }
                if (ext)
                {
                    break;
                }
            }
        }

        iter += 1;
    }

    LPProblemSolution *solution = new LPProblemSolution(Status::INFEASABLE);
    return *solution;
}

void LPProblemSolution::print_sol()
{
    std::cout << "\n\n\nSolution to the problem:\nSTATUS: " << (status == Status::OPTIMAL ? "Optimal" : status == Status::INFEASABLE ? "Infeasable"
                                                                                                                                     : "Unbounded")
              << std::endl;
    std::cout << "X* = ( ";
    for (auto &el : solution)
    {
        std::cout << el << " ";
    }
    std::cout << ")" << std::endl;
    std::cout << "Objective value is: " << objective_value << std::endl;
}
