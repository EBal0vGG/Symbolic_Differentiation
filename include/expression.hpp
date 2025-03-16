#ifndef HEADER_GUARD_EXPRESSION_HPP_INCLUDED
#define HEADER_GUARD_EXPRESSION_HPP_INCLUDED

#include <string>
#include <map>
#include <memory>
#include <complex>

// Абстрактный класс, задающий интерфейс между выражением и его реализацией.
template <typename Value_t> class ExpressionImpl {
public:
    // Запрет на создание экземпляров класса ExpressionImpl.
    ExpressionImpl() = default;
    virtual ~ExpressionImpl() = default;

    // Функция вычисления результата выражения.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const = 0;

    // Взятие производной по переменной.
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const = 0;

    // Функция подстановки значений в вырежение.
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const = 0;

    // Функция преобразование выражение в упрощенное.
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const = 0;

    // Функция преобразования выражения в строку.
    virtual std::string to_string() const = 0;
};

// Класс, задающий выражение и методы работы с ним.
template <typename Value_t> class Expression {
public:
    // Создание выражений.
    Expression(const std::string &variable);
    Expression(Value_t val);

    template <typename T>
    friend Expression<T> m_val(T val);

    template <typename T>
    friend Expression<T> m_var(const char *var);

    // Конструирование выражений на основе других выражений.
    Expression  operator+ (const Expression &other);
    Expression  operator- (const Expression &other);
    Expression  operator* (const Expression &other);
    Expression  operator/ (const Expression &other);
    Expression  operator^ (const Expression &other);
    Expression &operator*=(const Expression &other);
    Expression &operator-=(const Expression &other);
    Expression &operator+=(const Expression &other);
    Expression &operator/=(const Expression &other);
    Expression &operator^=(const Expression &other);

    Expression sin();
    Expression cos();
    Expression ln();
    Expression exp();

    // Операции с выражениями.
    Value_t eval(std::map<std::string, Value_t> &context) const;
    Expression diff(const std::string &by) const;
    Expression substitute(std::map<std::string, Value_t> &context) const;
    Expression prettify() const;
    std::string to_string() const;

private:
    Expression(std::shared_ptr<ExpressionImpl<Value_t>> impl);

    std::shared_ptr<ExpressionImpl<Value_t>> impl_;
};

// Определения дружественных функций.
template <typename T>
Expression<T> m_val(T val);

template <typename T>
Expression<T> m_var(const char* var);

// Класс, представляющий число в рамках выражения.
template <typename Value_t> class Value : public ExpressionImpl<Value_t> {
public:
    // Создание числа на основе ... числа.
    Value(Value_t value);

    virtual ~Value() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    Value_t value_;
};

// Класс, представляющий переменную в рамках выражения.
template <typename Value_t> class Variable : public ExpressionImpl<Value_t> {
public:
    // Создание переменной на основе её имени.
    Variable(const std::string &name);

    virtual ~Variable() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::string name_;
};

// Класс, представляющий выражение сложения двух выражений.
template <typename Value_t> class OperationAdd : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для суммы на основе подвыражений.
    OperationAdd(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                 const std::shared_ptr<ExpressionImpl<Value_t>> &right);

    virtual ~OperationAdd() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> left_;
    std::shared_ptr<ExpressionImpl<Value_t>> right_;
};

// Класс, представляющий выражение вычитания двух выражений.
template <typename Value_t> class OperationSub : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для вычитания на основе подвыражений.
    OperationSub(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                 const std::shared_ptr<ExpressionImpl<Value_t>> &right);

    virtual ~OperationSub() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> left_;
    std::shared_ptr<ExpressionImpl<Value_t>> right_;
};

// Класс, представляющий выражение умножения двух выражений.
template <typename Value_t> class OperationMul : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для уможения на основе подвыражений.
    OperationMul(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                 const std::shared_ptr<ExpressionImpl<Value_t>> &right);

    virtual ~OperationMul() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> left_;
    std::shared_ptr<ExpressionImpl<Value_t>> right_;
};

// Класс, представляющий выражение деления двух выражений.
template <typename Value_t> class OperationDiv : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для деления на основе подвыражений.
    OperationDiv(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                 const std::shared_ptr<ExpressionImpl<Value_t>> &right);

    virtual ~OperationDiv() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> left_;
    std::shared_ptr<ExpressionImpl<Value_t>> right_;
};

// Класс, представляющий выражение возведения в степень двух выражений.
template <typename Value_t> class OperationPow : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для возведения в степень на основе подвыражений.
    OperationPow(const std::shared_ptr<ExpressionImpl<Value_t>> &left,
                 const std::shared_ptr<ExpressionImpl<Value_t>> &right);

    virtual ~OperationPow() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> left_;
    std::shared_ptr<ExpressionImpl<Value_t>> right_;
};

// Класс, представляющий выражение взятия синуса.
template <typename Value_t> class OperationSin : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для взятия синуса на основе подвыражения.
    OperationSin(const std::shared_ptr<ExpressionImpl<Value_t>> &agrument);

    virtual ~OperationSin() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> argument_;
};

// Класс, представляющий выражение взятия косинуса.
template <typename Value_t> class OperationCos : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для взятия косинуса на основе подвыражения.
    OperationCos(const std::shared_ptr<ExpressionImpl<Value_t>> &agrument);

    virtual ~OperationCos() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> argument_;
};

// Класс, представляющий выражение взятия натурального логарифма.
template <typename Value_t> class OperationLn : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для взятия логарифма на основе подвыражения.
    OperationLn(const std::shared_ptr<ExpressionImpl<Value_t>> &agrument);

    virtual ~OperationLn() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> argument_;
};

// Класс, представляющий степенную функцию от экспоненты.
template <typename Value_t> class OperationExp : public ExpressionImpl<Value_t> {
public:
    // Создание выражения для взятия степенной функции от экспоненты.
    OperationExp(const std::shared_ptr<ExpressionImpl<Value_t>> &agrument);

    virtual ~OperationExp() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> diff(const std::string &by) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> substitute(std::map<std::string, Value_t> &context) const override;
    virtual std::shared_ptr<ExpressionImpl<Value_t>> prettify() const override;
    virtual std::string to_string() const override;

private:
    std::shared_ptr<ExpressionImpl<Value_t>> argument_;
};

#endif // HEADER_GUARD_EXPRESSION_HPP_INCLUDED