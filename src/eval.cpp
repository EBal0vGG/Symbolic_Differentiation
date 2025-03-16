#include <expression.hpp>

#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <sstream>

#include <lexer.hpp>
#include <parser.hpp>

typedef long double Value_t;

std::map<std::string, Value_t> parseVariables(int argc, char* argv[], int startIndex) {
    std::map<std::string, Value_t> context;
    for (int i = startIndex; i < argc; i++) {
        std::string arg = argv[i];
        size_t equalsPos = arg.find('=');
        if (equalsPos != std::string::npos) {
            std::string var = arg.substr(0, equalsPos);
            Value_t value = std::stold(arg.substr(equalsPos + 1));
            context[var] = value;
        }
    }
    return context;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: differentiator --eval <expression> [var=value ...]\n";
        std::cerr << "       differentiator --diff <expression> --by <variable>\n";
        return EXIT_FAILURE;
    }

    if (std::strcmp(argv[1], "--eval") == 0) {
        Lexer lexer{std::string(argv[2])};
        Parser<Value_t> parser{lexer};
        Expression expr = parser.parseExpression();

        std::map<std::string, Value_t> context = parseVariables(argc, argv, 3);

        Value_t result = expr.eval(context);
        std::cout << result << std::endl;
    }
    else if (std::strcmp(argv[1], "--diff") == 0 && argc >= 5 && std::strcmp(argv[3], "--by") == 0) {
        Lexer lexer{std::string(argv[2])};
        Parser<Value_t> parser{lexer};
        Expression expr = parser.parseExpression();

        std::string variable = argv[4];
        Expression diffExpr = expr.diff(variable);

        std::cout << diffExpr.prettify().to_string() << std::endl;
    }
    else {
        std::cerr << "Invalid arguments.\n";
        std::cerr << "Usage: differentiator --eval <expression> [var=value ...]\n";
        std::cerr << "       differentiator --diff <expression> --by <variable>\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}