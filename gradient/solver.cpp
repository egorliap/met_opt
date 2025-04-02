#include <utility>
#include "solver.h"
#include "vector_utils.h"

std::pair<std::vector<double>, double> StepSplittingGradientMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    int iter = 0;
    vector<double> d = funct.gradient(x0);
    vector<double> x_cur(x0);
    vector<double> x_next;
    double step = alpha;
    bool success = false;
    double cur_f;

    while (iter < max_iter)
    {
        success = false;
        cur_f = funct.get_value(x_cur);
        d = funct.gradient(x_cur);
        if (norm(d) < eps)
        {
            return {x_cur, cur_f};
        }

        while (!success)
        {
            x_next = subtract(x_cur, multiply(step, d));

            if (cur_f > funct.get_value(x_next))
            {
                x_cur = x_next;
                success = true;
            }
            else
            {
                step = step / 2;
            }
        }
        iter++;
    }
    return {x_cur, cur_f};
}

std::pair<std::vector<double>, double> NewtonMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    int iter = 0;

    vector<double> g = funct.gradient(x0);
    vector<vector<double>> h = funct.hessian(x0);

    vector<double> x_cur(x0);
    vector<double> x_new;
    double cur_f = funct.get_value(x_cur);
    
    while (iter < max_iter)
    {
        g = funct.gradient(x_cur);
        h = funct.hessian(x_cur);
        
        if (norm(g) < eps)
        { 
            return {x_cur, cur_f};
        }
        double det = determinant(h);
        if (det < 1e-10)
        {
            x_new = subtract(x_cur, multiply(1e-2, g));
        }
        else
        {
            vector<vector<double>> inv_h = get_inverse_matrix(h);
            vector<double> mult = matrix_vector_multiply(inv_h, g);
            x_new = subtract(x_cur, mult);
        }
        x_cur = x_new;
        double new_f = funct.get_value(x_cur);

        if (new_f > cur_f) {
            x_cur = subtract(x_cur, multiply(0.5, g)); 
        }
        iter++;
    }
    return {x_cur, cur_f};
}

std::pair<std::vector<double>, double> HookJeevesMethod::find(MultivarFunction &funct, std::vector<double> &x0, double eps, double alpha)
{
    vector<double> x_base = x0;
    vector<double> x = x0;

    vector<double> delta_step(x.size(), alpha); // Шаги по каждой координате
    double func_value = funct.get_value(x);
    int iter = 0;
    bool improved;

    while (iter < max_iter)
    {
        improved = false;

        // 1. Исследующий поиск (Exploratory move)
        for (size_t i = 0; i < x.size(); ++i)
        {
            // Пробуем шаг в положительном направлении
            vector<double> x_plus = x_base;
            x_plus[i] += delta_step[i];
            double f_plus = funct.get_value(x_plus);

            // Пробуем шаг в отрицательном направлении
            vector<double> x_minus = x_base;
            x_minus[i] -= delta_step[i];
            double f_minus = funct.get_value(x_minus);

            // Выбираем наилучшее направление
            if (f_plus < func_value)
            {
                x_base = x_plus;
                func_value = f_plus;
                improved = true;
            }
            else if (f_minus < func_value)
            {
                x_base = x_minus;
                func_value = f_minus;
                improved = true;
            }
        }

        // 2. Проверка на улучшение
        if (improved)
        {
            // 3. Поиск по образцу (Pattern move)
            vector<double> x_pattern = x_base;
            for (size_t i = 0; i < x.size(); ++i)
            {
                x_pattern[i] += (x_base[i] - x[i]);
            }

            // Проверяем новую точку
            double f_pattern = funct.get_value(x_pattern);
            if (f_pattern < func_value)
            {
                x = x_base;
                x_base = x_pattern;
                func_value = f_pattern;
            }
            else
            {
                x = x_base;
            }
        }
        else
        {
            // 4. Уменьшаем шаг, если улучшения нет
            for (size_t i = 0; i < delta_step.size(); ++i)
            {
                delta_step[i] *= 0.5;
            }
        }

        // Проверка условия остановки
        bool stop = true;
        for (const auto& step : delta_step)
        {
            if (step > eps)
            {
                stop = false;
                break;
            }
        }
        if (stop)
        {
            break;
        }

        iter++;
    }

    return { x_base, func_value };
}
