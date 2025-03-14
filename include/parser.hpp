#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include <set>

#include <expression.hpp>
#include <lexer.hpp>

// Задание формальной грамматики языка выражений.
// Терминалы - лексемы языка выражений: Value, Variable, +, -, *, /, (, )
//
// Правила продукций в форме Бэкуса-Наура.
// Expr   ::= Term + Expr
//          | Term - Expr
//          | Term
//
// Term   ::= Factor * Term
//          | Factor / Term
//          | Factor
//
// Factor ::= Power ^ Factor
//          | Power
//
// Power  ::= ( Expr )
//          | Func
//          | Value
//          | Variable

// Синтаксический анализатор для синтаксического разбора языка выражений.
template <typename Value_t> class Parser {
public:
    // Создание парсера на основе лексера.
    Parser(Lexer& lexer);

    // Синтаксический разбор выражения.
    Expression<Value_t> parseExpression();

private:
    // Ссылка на лексический анализатор.
    Lexer& lexer_;
    // Текущая лексема.
    Token currentToken_;
    // Предыдущая считанная лексема.
    Token previousToken_;

    // Безусловный cдвиг "каретки" синтаксического анализатора.
    void advance();
    // Сдвиг "каретки" синтаксического анализатора с выдачей исключения при несовпадении типа лексемы.
    void expect(std::set<TokenType> types);
    // Сдвиг "каретки" синтаксического анализатора при совпадении типа лексемы.
    bool match(TokenType type);

    // Методы для синтаксического разбора согласно формальной грамматике.
    Expression<Value_t> parseExpr();
    Expression<Value_t> parseTerm();
    Expression<Value_t> parseFactor();
    Expression<Value_t> parsePower();
};

#endif // HEADER_GUARD_PARSER_HPP_INCLUDED