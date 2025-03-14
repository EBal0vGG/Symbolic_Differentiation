#include <parser.hpp>

#include <stdexcept>

template <typename Value_t>
Parser<Value_t>::Parser(Lexer& lexer) :
    lexer_ (lexer),
    currentToken_ (),
    previousToken_ ()
{
    // Производим считывание первой лексемы.
    advance();
}

template <typename Value_t>
void Parser<Value_t>::advance() {
    // Считываем следующую лексему.
    previousToken_ = currentToken_;
    currentToken_ = lexer_.getNextToken();
#ifndef NDEBUG
    std::cout << currentToken_.lexeme << std::endl;
#endif
}

template <typename Value_t>
void Parser<Value_t>::expect(std::set<TokenType> types) {
    if (!types.contains(currentToken_.type)) {
        // Выбрасываем сообщение об ошибке.
        throw std::runtime_error(
            "Got unexpected token \"" + currentToken_.lexeme +
            "\" of type " + std::to_string(currentToken_.type));
    }

    // В случае успеха переходим к следующей лексеме.
    advance();
}

template <typename Value_t>
bool Parser<Value_t>::match(TokenType type) {
    if (currentToken_.type == type) {
        advance();
        return true;
    }

    return false;
}

template <typename Value_t>
Expression<Value_t> Parser<Value_t>::parseExpression() {
    Expression<Value_t> expr = parseExpr();

    expect({TOK_EOF});

    return expr;
}

template <typename Value_t>
Expression<Value_t> Parser<Value_t>::parseExpr() {
#ifndef NDEBUG
    std::cout << "Expr" << std::endl;
#endif
    Expression<Value_t> expr = parseTerm();

    while (currentToken_.type == TOK_PLUS || currentToken_.type == TOK_SUBTRACT) {
        if (currentToken_.type == TOK_PLUS) {
            // Считываем символ '+' в обязательном порядке.
            advance();

            // Считываем следующее слагаемое в выражении.
            Expression<Value_t> term = parseTerm();

            // Обновляем выражение для суммы.
            expr += term;
        }
        else {
            // Считываем символ '-' в обязательном порядке.
            advance();

            // Считываем следующее слагаемое в выражении.
            Expression<Value_t> term = parseTerm();

            // Обновляем выражение для суммы.
            expr -= term;
        }
    }

    return expr;
}

template <typename Value_t>
Expression<Value_t> Parser<Value_t>::parseTerm() {
#ifndef NDEBUG
    std::cout << "Term" << std::endl;
#endif
    Expression<Value_t> term = parseFactor();

    while (currentToken_.type == TOK_MULTIPLY || currentToken_.type == TOK_DIVIDE) {
        if (currentToken_.type == TOK_MULTIPLY) {
            // Считываем символ '*' в обязательном порядке.
            advance();

            // Считываем следующий множитель в выражении.
            Expression<Value_t> factor = parseFactor();

            // Обновляем выражение для произведения.
            term *= factor;
        }
        else {
            // Считываем символ '/' в обязательном порядке.
            advance();

            // Считываем следующий множитель в выражении.
            Expression<Value_t> factor = parseFactor();

            // Обновляем выражение для произведения.
            term /= factor;
        }
    }

    return term;
}
template <typename Value_t>
Expression<Value_t> Parser<Value_t>::parseFactor() {
#ifndef NDEBUG
    std::cout << "Factor" << std::endl;
#endif
    Expression<Value_t> factor = parsePower();

    while (currentToken_.type == TOK_POWER) {
        // Считываем символ '^' в обязательном порядке.
        advance();

        // Считываем следующую степень в выражении.
        Expression<Value_t> power = parsePower();

        // Обновляем выражение для степени.
        factor ^= power;
    }

    return factor;
}

template <typename Value_t>
Expression<Value_t> Parser<Value_t>::parsePower() {
#ifndef NDEBUG
    std::cout << "Power" << std::endl;
#endif
    if (match(TOK_FUNCTION)) {
        std::string funcName = previousToken_.lexeme;

        expect({TOK_BRACKET_LEFT});

        Expression<Value_t> expr = parseExpr();

        expect({TOK_BRACKET_RIGHT});

        if (funcName == "sin")
            return expr.sin();
        if (funcName == "cos")
            return expr.cos();
        if (funcName == "ln")
            return expr.ln();
        if (funcName == "exp")
            return expr.exp();
    }

    if (match(TOK_BRACKET_LEFT)) {
        Expression<Value_t> expr = parseExpr();

        expect({TOK_BRACKET_RIGHT});

        return expr;
    }

    if (match(TOK_VALUE)) {
        return Expression<Value_t>(std::stold(previousToken_.lexeme));
    }

    if (match(TOK_VARIABLE)) {
        return Expression<Value_t>(previousToken_.lexeme);
    }

    throw std::runtime_error(
        "Got unexpected token \"" + currentToken_.lexeme +
        "\" of type " + std::to_string(currentToken_.type));
}

template class Parser<long double>;
template class Parser<std::complex<long double>>;