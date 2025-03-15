#include <expression.hpp>

#include <string>
#include <cstring>

#include <lexer.hpp>
#include <parser.hpp>

typedef long double Value_t;

int main(int argc, char* argv[])
{
    if (argc != 3 || std::strcmp(argv[1], "--eval") != 0)
    {
        printf("Usage: eval --eval <expression>\n");
        return EXIT_FAILURE;
    }

    // Создаём лексический анализатор для разбора выражения.
    Lexer lexer{std::string(argv[2])};

    // Создаём парсер для выражения.
    Parser<Value_t> parser{lexer};

    // Создаём выражение на основе строки.
    Expression expr = parser.parseExpression();

    // Задаём контексты для вычисления выражений.
    std::map<std::string, Value_t> context1 =
    {
        {"x", 1.0}, {"y", 2.0}
    };

    std::map<std::string, Value_t> context2 =
    {
        {"x", 2.0}, {"y", 3.0}
    };

    // Вычисляем выражение, созданное на основе строки, в разных контекстах.
    printf("EVAL[%s]{x = %.2Lf, y = %.2Lf} = %Lf\n", expr.to_string().c_str(), context1["x"], context1["y"], expr.eval(context1));
    printf("EVAL[%s]{x = %.2Lf, y = %.2Lf} = %Lf\n", expr.to_string().c_str(), context2["x"], context2["y"], expr.eval(context2));
    printf("DIFF[%s] = [%s]\n", expr.to_string().c_str(), expr.diff("x").to_string().c_str());

    return EXIT_SUCCESS;
}