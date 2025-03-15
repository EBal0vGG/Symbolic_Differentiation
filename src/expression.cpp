#include <expression.hpp>
#include <utils.hpp>

#include <stdexcept>
#include <cmath>
#include <complex>

//==================//
// Класс Expression //
//==================//

template <typename Value_t>
Expression<Value_t>::Expression(std::shared_ptr<ExpressionImpl<Value_t>> impl) :
    impl_ (impl)
{}

template <typename Value_t>
Expression<Value_t>::Expression(const std::string &variable) :
    impl_ (std::make_shared<Variable<Value_t>>(variable))
{}

template <typename Value_t>
Expression<Value_t>::Expression(Value_t val) :
    impl_ (std::make_shared<Value<Value_t>>(val))
{}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::operator+(const Expression<Value_t> &other) {
    return Expression<Value_t>(std::make_shared<OperationAdd<Value_t>>(impl_, other.impl_));
}

template <typename Value_t>
Expression<Value_t> &Expression<Value_t>::operator+=(const Expression<Value_t> &other) {
    *this = *this + other;

    return *this;
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::operator-(const Expression<Value_t> &other) {
    return Expression<Value_t>(std::make_shared<OperationSub<Value_t>>(impl_, other.impl_));
}

template <typename Value_t>
Expression<Value_t> &Expression<Value_t>::operator-=(const Expression<Value_t> &other) {
    *this = *this - other;

    return *this;
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::operator*(const Expression<Value_t> &other) {
    return Expression<Value_t>(std::make_shared<OperationMul<Value_t>>(impl_, other.impl_));
}

template <typename Value_t>
Expression<Value_t> &Expression<Value_t>::operator*=(const Expression<Value_t> &other) {
    *this = *this * other;

    return *this;
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::operator/(const Expression<Value_t> &other) {
    return Expression<Value_t>(std::make_shared<OperationDiv<Value_t>>(impl_, other.impl_));
}

template <typename Value_t>
Expression<Value_t> &Expression<Value_t>::operator/=(const Expression<Value_t> &other) {
    *this = *this / other;

    return *this;
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::operator^(const Expression<Value_t> &other) {
    return Expression<Value_t>(std::make_shared<OperationPow<Value_t>>(impl_, other.impl_));
}

template <typename Value_t>
Expression<Value_t>& Expression<Value_t>::operator^=(const Expression<Value_t> &other) {
    *this = *this ^ other;

    return *this;
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::sin() {
    return Expression<Value_t>(std::make_shared<OperationSin<Value_t>>(impl_));
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::cos() {
    return Expression<Value_t>(std::make_shared<OperationCos<Value_t>>(impl_));
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::ln() {
    return Expression<Value_t>(std::make_shared<OperationLn<Value_t>>(impl_));
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::exp() {
    return Expression<Value_t>(std::make_shared<OperationExp<Value_t>>(impl_));
}

template <typename Value_t>
Value_t Expression<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    return impl_->eval(context);
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::diff(const std::string &by) const {
    return Expression<Value_t>(impl_->diff(by));
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return Expression<Value_t>(impl_->substitute(context));
}

template <typename Value_t>
Expression<Value_t> Expression<Value_t>::prettify() const {
    return Expression<Value_t>(impl_->prettify());
}

template <typename Value_t>
std::string Expression<Value_t>::to_string() const {
    return impl_->to_string();
}

// Определения дружественных функций.
template <typename T>
Expression<T> m_val(T val) {
    return Expression<T>(std::make_shared<Value<T>>(val));
}

template <typename T>
Expression<T> m_var(const char *var) {
    return Expression<T>(std::make_shared<Variable<T>>(std::string(var)));
}

// Explicit instantiation for required types
template Expression<long double> m_val(long double);
template Expression<std::complex<long double>> m_val(std::complex<long double>);

template Expression<long double> m_var(const char*);
template Expression<std::complex<long double>> m_var(const char*);

template class Expression<long double>;
template class Expression<std::complex<long double>>;

//=============//
// Класс Value //
//=============//

template <typename Value_t>
Value<Value_t>::Value(Value_t value) :
    value_ (value)
{}

// Реализация интерфейса ExpressionImpl.
template <typename Value_t>
Value_t Value<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    // Отмечаем контекст вычисления как неиспользуемый.
    (void) context;

    return value_;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Value<Value_t>::diff(const std::string &by) const {
    (void) by;

    return std::make_shared<Value<Value_t>>(Value(0.0));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Value<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    (void) context;

    return std::make_shared<Value<Value_t>>(Value(value_));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Value<Value_t>::prettify() const {
    return std::make_shared<Value<Value_t>>(value_);
}

template <typename Value_t>
std::string Value<Value_t>::to_string() const {
    return std::to_string(value_);
}

template <>
std::string Value<std::complex<long double>>::to_string() const {
    return "(" + std::to_string(value_.real()) + " + " + std::to_string(value_.imag()) + "i)";
}

template class Value<long double>;
template class Value<std::complex<long double>>;

//================//
// Класс Variable //
//================//

template <typename Value_t>
Variable<Value_t>::Variable(const std::string &name) :
    name_ (name)
{}

// Реализация интерфейса ExpressionImpl.
template <typename Value_t>
Value_t Variable<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    auto iter = context.find(name_);

    if (iter == context.end()) {
        throw std::runtime_error("Variable \"" + name_ + "\" not present in evaluation context");
    }

    return iter->second;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Variable<Value_t>::diff(const std::string &by) const {
    if (by == name_) {
        return std::make_shared<Value<Value_t>>(Value<Value_t>(1.0));
    }
    return std::make_shared<Value<Value_t>>(Value<Value_t>(0.0));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Variable<Value_t>::substitute(std::map<std::string, Value_t> &context) const {

    auto iter = context.find(name_);

    if (iter != context.end()) {
        return std::make_shared<Value<Value_t>>(Value<Value_t>(iter->second));
    }

    return std::make_shared<Variable<Value_t>>(Variable<Value_t>(name_));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> Variable<Value_t>::prettify() const {
    return std::make_shared<Variable<Value_t>>(name_);
}

template <typename Value_t>
std::string Variable<Value_t>::to_string() const {
    return name_;
}

template class Variable<long double>;
template class Variable<std::complex<long double>>;

//====================//
// Класс OperationAdd //
//====================//

template <typename Value_t>
OperationAdd<Value_t>::OperationAdd(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                                    const std::shared_ptr<ExpressionImpl<Value_t>> &right) :
    left_  (left),
    right_ (right)
{}

template <typename Value_t>
Value_t OperationAdd<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value_left  = left_->eval(context);
    Value_t value_right = right_->eval(context);

    return value_left + value_right;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationAdd<Value_t>::diff(const std::string &by) const {
    return std::make_shared<OperationAdd<Value_t>>(left_->diff(by), right_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationAdd<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationAdd<Value_t>> (
        left_->substitute(context), right_->substitute(context)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationAdd<Value_t>::prettify() const {
    auto new_left  = left_->prettify();
    auto new_right = right_->prettify();

    if (is_zero(new_left)) return new_right;
    if (is_zero(new_right)) return new_left;
    if (is_val(new_left) && is_val(new_right)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(
            new_left->eval(emptyContext) + new_right->eval(emptyContext)
        );
    }
    return std::make_shared<OperationAdd<Value_t>>(new_left, new_right);
}

template <typename Value_t>
std::string OperationAdd<Value_t>::to_string() const {
    return std::string("(")   + left_->to_string()  +
           std::string(" + ") + right_->to_string() +
           std::string(")");
}

template class OperationAdd<long double>;
template class OperationAdd<std::complex<long double>>;

//====================//
// Класс OperationSub //
//====================//

template <typename Value_t>
OperationSub<Value_t>::OperationSub(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                                    const std::shared_ptr<ExpressionImpl<Value_t>> &right) :
    left_  (left),
    right_ (right)
{}

template <typename Value_t>
Value_t OperationSub<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value_left  = left_->eval(context);
    Value_t value_right = right_->eval(context);

    return value_left - value_right;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSub<Value_t>::diff(const std::string &by) const {
    return std::make_shared<OperationSub<Value_t>>(left_->diff(by), right_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSub<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationSub<Value_t>> (
        left_->substitute(context), right_->substitute(context)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSub<Value_t>::prettify() const {
    auto new_left  = left_->prettify();
    auto new_right = right_->prettify();

    if (is_zero(new_right)) return new_left;
    if (is_val(new_left) && is_val(new_right)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(
            new_left->eval(emptyContext) - new_right->eval(emptyContext)
        );
    }

    return std::make_shared<OperationSub<Value_t>>(new_left, new_right);
}

template <typename Value_t>
std::string OperationSub<Value_t>::to_string() const {
    return std::string("(")   + left_->to_string()  +
           std::string(" - ") + right_->to_string() +
           std::string(")");
}

template class OperationSub<long double>;
template class OperationSub<std::complex<long double>>;

//====================//
// Класс OperationMul //
//====================//

template <typename Value_t>
OperationMul<Value_t>::OperationMul(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                                    const std::shared_ptr<ExpressionImpl<Value_t>> &right) :
    left_  (left),
    right_ (right)
{}

template <typename Value_t>
Value_t OperationMul<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value_left  = left_->eval(context);
    Value_t value_right = right_->eval(context);

    return value_left * value_right;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationMul<Value_t>::diff(const std::string &by) const {
    return std::make_shared<OperationAdd<Value_t>> (
        std::make_shared<OperationMul<Value_t>>(left_->diff(by), right_),
        std::make_shared<OperationMul<Value_t>>(left_, right_->diff(by))
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationMul<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationMul<Value_t>> (
        left_->substitute(context), right_->substitute(context)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationMul<Value_t>::prettify() const {
    auto new_left  = left_->prettify();
    auto new_right = right_->prettify();

    if (is_one(new_left)) return new_right;
    if (is_one(new_right)) return new_left;
    if (is_zero(new_left) || is_zero(new_right))
        return std::make_shared<Value<Value_t>>(0.0);
    if (is_val(new_left) && is_val(new_right)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(
            new_left->eval(emptyContext) * new_right->eval(emptyContext)
        );
    }

    return std::make_shared<OperationMul<Value_t>>(new_left, new_right);
}

template <typename Value_t>
std::string OperationMul<Value_t>::to_string() const {
    return std::string("(")   + left_->to_string()  +
           std::string(" * ") + right_->to_string() +
           std::string(")");
}

template class OperationMul<long double>;
template class OperationMul<std::complex<long double>>;

//====================//
// Класс OperationDiv //
//====================//

template <typename Value_t>
OperationDiv<Value_t>::OperationDiv(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                                    const std::shared_ptr<ExpressionImpl<Value_t>> &right) :
    left_  (left),
    right_ (right)
{}

template <typename Value_t>
Value_t OperationDiv<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value_left  = left_->eval(context);
    Value_t value_right = right_->eval(context);

    return value_left / value_right;
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationDiv<Value_t>::diff(const std::string &by) const {
    auto numerator = std::make_shared<OperationSub<Value_t>> (
        std::make_shared<OperationMul<Value_t>>(left_->diff(by), right_),
        std::make_shared<OperationMul<Value_t>>(left_, right_->diff(by))
    );

    auto denominator = std::make_shared<OperationPow<Value_t>>(
        right_,
        std::make_shared<Value<Value_t>>(2.0)
    );

    return std::make_shared<OperationDiv<Value_t>>(numerator, denominator);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationDiv<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationDiv<Value_t>> (
        left_->substitute(context), right_->substitute(context)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationDiv<Value_t>::prettify() const {
    auto new_left  = left_->prettify();
    auto new_right = right_->prettify();

    if (is_one(new_right)) return new_left;
    if (is_zero(new_left)) return std::make_shared<Value<Value_t>>(0.0);
    if (is_val(new_left) && is_val(new_right)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(
            new_left->eval(emptyContext) / new_right->eval(emptyContext)
        );
    }

    return std::make_shared<OperationDiv<Value_t>>(new_left, new_right);
}

template <typename Value_t>
std::string OperationDiv<Value_t>::to_string() const {
    return std::string("(")   + left_->to_string()  +
           std::string(" / ") + right_->to_string() +
           std::string(")");
}

template class OperationDiv<long double>;
template class OperationDiv<std::complex<long double>>;

//====================//
// Класс OperationPow //
//====================//

template <typename Value_t>
OperationPow<Value_t>::OperationPow(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                                    const std::shared_ptr<ExpressionImpl<Value_t>> &right) :
    left_  (left),
    right_ (right)
{}

template <typename Value_t>
Value_t OperationPow<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value_left  = left_->eval(context);
    Value_t value_right = right_->eval(context);

    return pow(value_left, value_right);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationPow<Value_t>::diff(const std::string &by) const {
    // left_^right_ * (right_' * ln(left_) + (right_ * left_') / left_)

    // right_' * ln(left_)
    auto term1 = std::make_shared<OperationMul<Value_t>> (
        right_->diff(by),
        std::make_shared<OperationLn<Value_t>>(left_)
    );

    // (right_ * left_') / left_
    auto term2 = std::make_shared<OperationDiv<Value_t>> (
        std::make_shared<OperationMul<Value_t>>(right_, left_->diff(by)),
        left_
    );

    // left_^right_ * (term1 + term2)
    return std::make_shared<OperationMul<Value_t>> (
        std::make_shared<OperationPow<Value_t>>(left_, right_),
        std::make_shared<OperationAdd<Value_t>>(term1, term2)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationPow<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationPow<Value_t>> (
        left_->substitute(context), right_->substitute(context)
    );
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationPow<Value_t>::prettify() const {
    auto new_left = left_->prettify();
    auto new_right = right_->prettify();

    if (is_zero(new_left)) return std::make_shared<Value<Value_t>>(0.0L);
    if (is_zero(new_right) || is_one(new_left))
        return std::make_shared<Value<Value_t>>(1.0);
    if (is_one(new_right)) return new_left;
    if (is_val(new_left) && is_val(new_right)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(
            pow(new_left->eval(emptyContext), new_right->eval(emptyContext))
        );
    }

    return std::make_shared<OperationPow<Value_t>>(new_left, new_right);
}

template <typename Value_t>
std::string OperationPow<Value_t>::to_string() const {
    return std::string("(")   + left_->to_string()  +
           std::string(" ^ ") + right_->to_string() +
           std::string(")");
}

template class OperationPow<long double>;
template class OperationPow<std::complex<long double>>;

//====================//
// Класс OperationSin //
//====================//

template <typename Value_t>
OperationSin<Value_t>::OperationSin(const std::shared_ptr<ExpressionImpl<Value_t>> &argument) :
    argument_(argument)
{}

template <typename Value_t>
Value_t OperationSin<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value  = argument_->eval(context);

    return sin(value);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSin<Value_t>::diff(const std::string &by) const {
    auto sin_diff = std::make_shared<OperationCos<Value_t>>(argument_);
    return std::make_shared<OperationMul<Value_t>>(sin_diff, argument_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSin<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationSin<Value_t>>(argument_->substitute(context));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationSin<Value_t>::prettify() const {
    auto new_arg = argument_->prettify();

    if (is_val(new_arg)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(sin(new_arg->eval(emptyContext)));
    }
    return std::make_shared<OperationSin<Value_t>>(new_arg);
}

template <typename Value_t>
std::string OperationSin<Value_t>::to_string() const {
    return "sin(" + argument_->to_string() + ")";
}

template class OperationSin<long double>;
template class OperationSin<std::complex<long double>>;

//====================//
// Класс OperationCos //
//====================//

template <typename Value_t>
OperationCos<Value_t>::OperationCos(const std::shared_ptr<ExpressionImpl<Value_t>> &argument) :
    argument_(argument)
{}

template <typename Value_t>
Value_t OperationCos<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value  = argument_->eval(context);

    return cos(value);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationCos<Value_t>::diff(const std::string &by) const {
    auto cos_diff = std::make_shared<OperationMul<Value_t>> (
        std::make_shared<Value<Value_t>>(Value<Value_t>(-1.0)),
        std::make_shared<OperationSin<Value_t>>(argument_)
    );

    return std::make_shared<OperationMul<Value_t>>(cos_diff, argument_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationCos<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationCos<Value_t>>(argument_->substitute(context));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationCos<Value_t>::prettify() const {
    auto new_arg = argument_->prettify();

    if (is_val(new_arg)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(cos(new_arg->eval(emptyContext)));
    }
    return std::make_shared<OperationCos<Value_t>>(new_arg);
}

template <typename Value_t>
std::string OperationCos<Value_t>::to_string() const {
    return "cos(" + argument_->to_string() + ")";
}

template class OperationCos<long double>;
template class OperationCos<std::complex<long double>>;

//===================//
// Класс OperationLn //
//===================//

template <typename Value_t>
OperationLn<Value_t>::OperationLn(const std::shared_ptr<ExpressionImpl<Value_t>> &argument) :
    argument_(argument)
{}

template <typename Value_t>
Value_t OperationLn<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value  = argument_->eval(context);

    return log(value);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationLn<Value_t>::diff(const std::string &by) const {
    auto ln_diff = std::make_shared<OperationDiv<Value_t>> (
        std::make_shared<Value<Value_t>>(Value<Value_t>(1.0)),
        argument_
    );

    return std::make_shared<OperationMul<Value_t>>(ln_diff, argument_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationLn<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationLn<Value_t>>(argument_->substitute(context));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationLn<Value_t>::prettify() const {
    auto new_arg = argument_->prettify();

    if (is_val(new_arg)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(log(new_arg->eval(emptyContext)));
    }
    return std::make_shared<OperationLn<Value_t>>(new_arg);
}

template <typename Value_t>
std::string OperationLn<Value_t>::to_string() const {
    return "ln(" + argument_->to_string() + ")";
}

template class OperationLn<long double>;
template class OperationLn<std::complex<long double>>;

//====================//
// Класс OperationExp //
//====================//

template <typename Value_t>
OperationExp<Value_t>::OperationExp(const std::shared_ptr<ExpressionImpl<Value_t>> &argument) :
    argument_(argument)
{}

template <typename Value_t>
Value_t OperationExp<Value_t>::eval(std::map<std::string, Value_t> &context) const {
    Value_t value  = argument_->eval(context);

    return exp(value);
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationExp<Value_t>::diff(const std::string &by) const {
    auto exp_diff = std::make_shared<OperationExp<Value_t>>(argument_);
    return std::make_shared<OperationMul<Value_t>>(exp_diff, argument_->diff(by));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationExp<Value_t>::substitute(std::map<std::string, Value_t> &context) const {
    return std::make_shared<OperationExp<Value_t>>(argument_->substitute(context));
}

template <typename Value_t>
std::shared_ptr<ExpressionImpl<Value_t>> OperationExp<Value_t>::prettify() const {
    auto new_arg = argument_->prettify();

    if (is_val(new_arg)) {
        std::map<std::string, Value_t> emptyContext;

        return std::make_shared<Value<Value_t>>(exp(new_arg->eval(emptyContext)));
    }
    return std::make_shared<OperationExp<Value_t>>(new_arg);
}

template <typename Value_t>
std::string OperationExp<Value_t>::to_string() const {
    return "exp(" + argument_->to_string() + ")";
}

template class OperationExp<long double>;
template class OperationExp<std::complex<long double>>;