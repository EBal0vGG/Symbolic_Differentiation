#ifndef HEADER_GUARD_UTILS_HPP_INCLUDED
#define HEADER_GUARD_UTILS_HPP_INCLUDED

#include <memory>

template <typename Value_t> class ExpressionImpl;
template <typename Value_t> class Value;
template <typename Value_t> class Variable;

template <typename Value_t>
bool is_val(const std::shared_ptr<ExpressionImpl<Value_t>> &expr) {
    return std::dynamic_pointer_cast<Value<Value_t>>(expr) != nullptr;
}

template <typename Value_t>
bool is_var(const std::shared_ptr<ExpressionImpl<Value_t>> &expr) {
    return std::dynamic_pointer_cast<Variable<Value_t>>(expr) != nullptr;
}

template <typename Value_t>
bool is_zero(const std::shared_ptr<ExpressionImpl<Value_t>> &expr) {
    if (auto value = std::dynamic_pointer_cast<Value<Value_t>>(expr)) {
        std::map<std::string, Value_t> emptyContext;

        return value->eval(emptyContext) == Value_t(0.0);
    }
    return false;
}

template <typename Value_t>
bool is_one(const std::shared_ptr<ExpressionImpl<Value_t>> &expr) {
    if (auto value = std::dynamic_pointer_cast<Value<Value_t>>(expr)) {
        std::map<std::string, Value_t> emptyContext;

        return value->eval(emptyContext) == Value_t(1.0);
    }
    return false;
}

#endif // HEADER_GUARD_UTILS_HPP_INCLUDED