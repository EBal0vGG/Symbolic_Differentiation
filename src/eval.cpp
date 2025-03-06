#include <expression.hpp>

#include <iostream>
#include <cstring>

int main(int argc, char **argv) {
    if (argc != 5 || std::strcmp(argv[1], "--x") != 0 || std::strcmp(argv[3], "--y") != 0) {
        printf("Usage: eval --x <x value> --y <y value>\n");
        return EXIT_FAILURE;
    }

    long double x = stold(std::string(argv[2]));
    long double y = stold(std::string(argv[4]));

    std::map<std::string, Value_t> input_context = {
        {"x", x}, {"y", y}
    };

    // Создаём объект выражения на основе механизма синтаксического соответствия.
    Expression expr1 = 100.0_val - "x"_var * 2.0_val / 3.0_val + ("x"_var ^ 2.0_val);
    expr1 += "y"_var * "x"_var + "x"_var.sin();

    Expression expr2 = ((2.0_val).exp()).ln();

    // Задаём контексты для вычисления выражений.
    std::map<std::string, Value_t> context1 = {
        {"x", 1.0}, {"y", 2.0}
    };

    std::map<std::string, Value_t> context2 = {
        {"x", 2.0}, {"y", 3.0}
    };

    Expression expr3 = 100.0_val - "x"_var * 2.0_val / 3.0_val + ("y"_var ^ 2.0_val);

    std::map<std::string, Value_t> context3 = {
        {"x", 100.0}
    };

    // Вычисляем выражение в различных контекстах.
    printf("EVAL[%s]{x = %.1Lf, y = %.1Lf} = %Lf\n", expr1.to_string().c_str(), x, y, expr1.eval(input_context));

    printf("EVAL[%s]{x = %.1Lf, y = %.1Lf} = %Lf\n",
            expr1.to_string().c_str(), context1["x"], context1["y"], expr1.eval(context1));
    printf("EVAL[%s]{x = %.1Lf, y = %.1Lf} = %Lf\n",
            expr1.to_string().c_str(), context2["x"], context2["y"], expr1.eval(context2));
    printf("\n");

    printf("EVAL[%s] = %Lf\n", expr2.to_string().c_str(), expr2.eval(input_context));
    printf("\n");

    printf("SUBSTITUTE[%s]{x = %.1Lf} = %s\n", expr3.to_string().c_str(), context3["x"], expr3.substitute(context3).to_string().c_str());

    return EXIT_SUCCESS;
}