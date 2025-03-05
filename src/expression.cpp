#include <expression.hpp>

#include <stdexcept>
#include <cmath>

//==================//
// Класс Expression //
//==================//

Expression::Expression(std::shared_ptr<ExpressionImpl> impl) :
    impl_ (impl)
{}

Expression::Expression(std::string variable) :
    impl_ (std::make_shared<Variable>(variable))
{}

Expression Expression::operator+(const Expression &other) {
    return Expression(std::make_shared<OperationAdd>(*this, other));
}

Expression& Expression::operator+=(const Expression &other) {
    *this = *this + other;

    return *this;
}

Expression Expression::operator-(const Expression &other) {
    return Expression(std::make_shared<OperationSub>(*this, other));
}

Expression& Expression::operator-=(const Expression &other) {
    *this = *this - other;

    return *this;
}

Expression Expression::operator*(const Expression &other) {
    return Expression(std::make_shared<OperationMul>(*this, other));
}

Expression& Expression::operator*=(const Expression &other) {
    *this = *this * other;

    return *this;
}

Expression Expression::operator/(const Expression &other) {
    return Expression(std::make_shared<OperationDiv>(*this, other));
}

Expression& Expression::operator/=(const Expression &other) {
    *this = *this * other;

    return *this;
}

Expression Expression::operator^(const Expression &other) {
    return Expression(std::make_shared<OperationPow>(*this, other));
}

Expression& Expression::operator^=(const Expression &other) {
    *this = *this ^ other;

    return *this;
}

Expression Expression::sin() {
    return Expression(std::make_shared<OperationSin>(*this));
}

Expression Expression::cos() {
    return Expression(std::make_shared<OperationCos>(*this));
}

Expression Expression::ln() {
    return Expression(std::make_shared<OperationLn>(*this));
}

Expression Expression::exp() {
    return Expression(std::make_shared<OperationExp>(*this));
}

Expression operator""_val(Value_t val) {
    return Expression(std::make_shared<Value>(val));
}

Expression operator""_var(const char *name) {
    return Expression(std::make_shared<Variable>(std::string(name)));
}

Expression operator""_var(const char *name, size_t size) {
    (void) size;

    return Expression(std::make_shared<Variable>(std::string(name)));
}

Value_t Expression::eval(std::map<std::string, Value_t> context) const {
    return impl_->eval(context);
}

std::string Expression::to_string() const {
    return impl_->to_string();
}

//=============//
// Класс Value //
//=============//

Value::Value(Value_t value) :
    value_ (value)
{}

// Реализация интерфейса ExpressionImpl.
Value_t Value::eval(std::map<std::string, Value_t> context) const {
    // Отмечаем контекст вычисления как неиспользуемый.
    (void) context;

    return value_;
}

std::string Value::to_string() const {
    return std::to_string(value_);
}

//================//
// Класс Variable //
//================//

Variable::Variable(std::string name) :
    name_ (name)
{}

// Реализация интерфейса ExpressionImpl.
Value_t Variable::eval(std::map<std::string, Value_t> context) const {
    auto iter = context.find(name_);

    if (iter == context.end())
    {
        throw std::runtime_error("Variable \"" + name_ + "\" not present in evaluation context");
    }

    return iter->second;
}

std::string Variable::to_string() const {
    return name_;
}

//====================//
// Класс OperationAdd //
//====================//

OperationAdd::OperationAdd(Expression left, Expression right) :
    left_  (left),
    right_ (right)
{}

Value_t OperationAdd::eval(std::map<std::string, Value_t> context) const {
    Value_t value_left  = left_.eval(context);
    Value_t value_right = right_.eval(context);

    return value_left + value_right;
}

std::string OperationAdd::to_string() const {
    return std::string("(")   + left_.to_string()  +
           std::string(" + ") + right_.to_string() +
           std::string(")");
}

//====================//
// Класс OperationSub //
//====================//

OperationSub::OperationSub(Expression left, Expression right) :
    left_  (left),
    right_ (right)
{}

Value_t OperationSub::eval(std::map<std::string, Value_t> context) const {
    Value_t value_left  = left_.eval(context);
    Value_t value_right = right_.eval(context);

    return value_left - value_right;
}

std::string OperationSub::to_string() const {
    return std::string("(")   + left_.to_string()  +
           std::string(" - ") + right_.to_string() +
           std::string(")");
}

//====================//
// Класс OperationMul //
//====================//

OperationMul::OperationMul(Expression left, Expression right) :
    left_  (left),
    right_ (right)
{}

Value_t OperationMul::eval(std::map<std::string, Value_t> context) const {
    Value_t value_left  = left_.eval(context);
    Value_t value_right = right_.eval(context);

    return value_left * value_right;
}

std::string OperationMul::to_string() const {
    return std::string("(")   + left_.to_string()  + std::string(")") +
           std::string(" * ") +
           std::string("(")   + right_.to_string() + std::string(")");
}

//====================//
// Класс OperationDiv //
//====================//

OperationDiv::OperationDiv(Expression left, Expression right) :
    left_  (left),
    right_ (right)
{}

Value_t OperationDiv::eval(std::map<std::string, Value_t> context) const {
    Value_t value_left  = left_.eval(context);
    Value_t value_right = right_.eval(context);

    return value_left / value_right;
}

std::string OperationDiv::to_string() const {
return std::string("(")   + left_.to_string()  + std::string(")") +
       std::string(" / ") +
       std::string("(")   + right_.to_string() + std::string(")");
}

//====================//
// Класс OperationPow //
//====================//

OperationPow::OperationPow(Expression left, Expression right) :
    left_  (left),
    right_ (right)
{}

Value_t OperationPow::eval(std::map<std::string, Value_t> context) const {
    Value_t value_left  = left_.eval(context);
    Value_t value_right = right_.eval(context);

    try {
        return powl(value_left, value_right);
    } catch (const std::exception &e) {
        throw std::runtime_error("Error in powl: " + std::string(e.what())       +
                                 " (Left: "        + std::to_string(value_left)  +
                                 ", Right: "       + std::to_string(value_right) + ")");
    }
}

std::string OperationPow::to_string() const {
    return std::string("(")   + left_.to_string()  + std::string(")") +
           std::string(" ^ ") +
           std::string("(")   + right_.to_string() + std::string(")");
}

//====================//
// Класс OperationSin //
//====================//

OperationSin::OperationSin(Expression argument) :
    argument_(argument)
{}

Value_t OperationSin::eval(std::map<std::string, Value_t> context) const {
    Value_t value  = argument_.eval(context);

    return sinl(value);
}

std::string OperationSin::to_string() const {
    return "sin(" + argument_.to_string() + ")";
}

//====================//
// Класс OperationCos //
//====================//

OperationCos::OperationCos(Expression argument) :
    argument_(argument)
{}

Value_t OperationCos::eval(std::map<std::string, Value_t> context) const {
    Value_t value  = argument_.eval(context);

    return cosl(value);
}

std::string OperationCos::to_string() const {
    return "cos(" + argument_.to_string() + ")";
}

//====================//
// Класс OperationLn  //
//====================//

OperationLn::OperationLn(Expression argument) :
    argument_(argument)
{}

Value_t OperationLn::eval(std::map<std::string, Value_t> context) const {
    Value_t value  = argument_.eval(context);

    return logl(value);
}

std::string OperationLn::to_string() const {
    return "ln(" + argument_.to_string() + ")";
}

//====================//
// Класс OperationExp //
//====================//

OperationExp::OperationExp(Expression argument) :
    argument_(argument)
{}

Value_t OperationExp::eval(std::map<std::string, Value_t> context) const {
    Value_t value  = argument_.eval(context);

    return expl(value);
}

std::string OperationExp::to_string() const {
    return "exp(" + argument_.to_string() + ")";
}
