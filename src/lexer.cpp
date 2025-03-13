#include <lexer.hpp>
#include <stdexcept>

Lexer::Lexer(const std::string& input) :
    input_  (input),
    pos_    (),
    end_    (),
    column_ (0)
{
    pos_ = input_.c_str();
    end_ = pos_ + input_.size();
}

Token Lexer::getNextToken() {
    // Пропускаем пробельные символы до начала лексемы.
    skipSpaceSequence();

    // Обрабатываем конец разбираемой строки.
    if (pos_ >= end_) {
        return Token{TOK_EOF, "", column_};
    }

    // Следующий символ для "угадывания" типа лексемы.
    char currentChar = peek();

    if (isalpha(currentChar) || currentChar == '_') {
        // Лексический разбор имени переменной.
        return getVariable();
    }
    else if (isdigit(currentChar)) {
        // Лексический разбор числового значения.
        return getValue();
    }
    else if (currentChar == '+') {
        // Извлекаем символ '+' из области предпросмотра.
        get();

        // Лексема для операции сложения.
        return (Token){TOK_PLUS, std::string("+"), column_};
    }
    else if (currentChar == '-') {
        // Извлекаем символ '-' из области предпросмотра.
        get();

        // Лексема для операции вычитания.
        return (Token){TOK_SUBTRACT, std::string("-"), column_};
    }
    else if (currentChar == '*') {
        // Извлекаем символ '*' из области предпросмотра.
        get();

        // Лексема для операции умножения.
        return (Token){TOK_MULTIPLY, std::string("*"), column_};
    }
    else if (currentChar == '/') {
        // Извлекаем символ '/' из области предпросмотра.
        get();

        // Лексема для операции деления.
        return (Token){TOK_DIVIDE, std::string("/"), column_};
    }
    else if (currentChar == '^') {
        // Извлекаем символ '^' из области предпросмотра.
        get();

        // Лексема для операции возведения в степень.
        return (Token){TOK_POWER, std::string("^"), column_};
    }
    else if (currentChar == '(') {
        // Извлекаем символ '(' из области предпросмотра.
        get();

        // Лексема для левой скобки.
        return (Token){TOK_BRACKET_LEFT, std::string("("), column_};
    }
    else if (currentChar == ')') {
        // Извлекаем символ ')' из области предпросмотра.
        get();

        // Лексема для правой скобки.
        return (Token){TOK_BRACKET_RIGHT, std::string(")"), column_};
    }
    else {
        // Сообщаем об ошибке.
        throw std::runtime_error(
            std::string("Unexpected subexpression on pos ") + std::to_string(column_));
    }
}

Token Lexer::getTokenByPattern(const std::regex& regexp, TokenType expectedTokenType) {
    // Результат поиска паттерна для регулярного выражения в строке.
    std::cmatch matchResult{};

    // Производим поиск паттерна в строке.
    bool matchStatus = std::regex_search(
        pos_, end_, matchResult, regexp, std::regex_constants::match_continuous);
    if (!matchStatus) {
        // Сообщаем об ошибке.
        throw std::runtime_error(
            std::string("Unexpected subexpression on pos ") + std::to_string(column_));
    }

    // Сдвигаем текущую позицию.
    pos_ = matchResult[0].second;

    // Строка для считанной лексемы.
    std::string lexemeStr = std::string(matchResult[0].first, matchResult[0].second);

    // Возвращаем считанную лексему.
    return Token{expectedTokenType, std::move(lexemeStr), column_};
}

void Lexer::skipTokenByPattern(const std::regex& regexp) {
    // Результат поиска паттерна для регулярного выражения в строке.
    std::cmatch matchResult{};

    // Производим поиск паттерна в строке.
    bool matchStatus = std::regex_search(
        pos_, end_, matchResult, regexp, std::regex_constants::match_continuous);
    if (matchStatus) {
        // Сдвигаем текущую позицию.
        pos_ = matchResult[0].second;
    }
}

char Lexer::peek() const {
    return *pos_;
}

char Lexer::get() {
    char c = peek();
    pos_++;
    return c;
}

void Lexer::skipSpaceSequence() {
    static const std::regex spacePattern{REGEXP_SPACE_SEQUENCE};

    skipTokenByPattern(spacePattern);
}

Token Lexer::getVariable() {
    static const std::regex variablePattern{REGEXP_VARIABLE};

    return getTokenByPattern(variablePattern, TOK_VARIABLE);
}

Token Lexer::getValue() {
    static const std::regex valuePattern{REGEXP_VALUE};

    return getTokenByPattern(valuePattern, TOK_VALUE);
}