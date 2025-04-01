from pulp import *  # noqa: F403

# Инициализация задачи
prob = LpProblem("Linear_Programming_Problem", LpMinimize)

# Определение переменных
x = [LpVariable(f'x_{i}', lowBound=0) for i in range(1, 21)]

# Коэффициенты целевой функции
cost_coeff = [3, 5, 6, 8, 11, 4, 7, 3, 5, 14, 9, 3, 4, 4, 10, 1, 5, 16, 3, 8]

# Целевая функция
prob += lpSum([cost_coeff[i] * x[i] for i in range(20)]), "Total Cost"

# Ограничения
prob += x[0] + x[1] + x[2] + x[3] + x[4] == 10, "Constraint_1"
prob += x[5] + x[6] + x[7] + x[8] + x[9] == 3, "Constraint_2"
prob += x[10] + x[11] + x[12] + x[13] + x[14] == 8, "Constraint_3"
prob += x[15] + x[16] + x[17] + x[18] + x[19] == 10, "Constraint_4"
prob += x[0] + x[5] + x[10] + x[15] == 2, "Constraint_5"
prob += x[1] + x[6] + x[11] + x[16] == 8, "Constraint_6"
prob += x[2] + x[7] + x[12] + x[17] == 2, "Constraint_7"
prob += x[3] + x[8] + x[13] + x[18] == 8, "Constraint_8"
prob += x[4] + x[9] + x[14] + x[19] == 11, "Constraint_9"

# Решение задачи
prob.solve(PULP_CBC_CMD(msg=True))

# Вывод первого оптимального решения
print("Первое оптимальное решение:")
for v in prob.variables():
    print(f"{v.name} = {v.varValue}")
print(f"Оптимальное значение целевой функции: {value(prob.objective)}\n")

# Фиксируем целевую функцию на оптимальном значении
optimal_value = value(prob.objective)
prob += lpSum([cost_coeff[i] * x[i] for i in range(20)]) == optimal_value, "Fix_Optimal_Value"


for i in range(20):
    if i != 18:
        continue
    prob_temp = prob.copy()
    prob_temp += x[i], f"Minimize_x{i+1}"
    prob_temp.solve(PULP_CBC_CMD(msg=False))
    print(f"\nРешение с минимальным x_{i+1}: x_{i+1} = {round(x[i].varValue)}")

    min_val = round(x[i].varValue)
    
    prob_temp = prob.copy()
    prob_temp += -x[i], f"Maximize_x{i+1}"
    prob_temp.solve(PULP_CBC_CMD(msg=False))
    print(f"Решение с максимальным x_{i+1}: x_{i+1} = {round(x[i].varValue)}")
    
    max_val = round(x[i].varValue)
    
    for j in range(min_val, max_val+1):
        prob_temp = prob.copy()
        prob_temp += x[i] == j, f"{j}"
        prob_temp.solve(PULP_CBC_CMD(msg=False))
        print(f"Решение с x_{i+1}: x_{i+1} = {j}")

        for k in range(20):
            print(f"x_{k+1} = {round(x[k].varValue)}")
        input()