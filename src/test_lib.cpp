#include "../include/expression.hpp"
#include <gtest/gtest.h>
#include <map>
#include <string>

using namespace std;

// Test fixture for Expression tests
class ExpressionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize any common resources for tests
    }

    void TearDown() override {
        // Clean up any common resources
    }
};

// Test Value class
TEST_F(ExpressionTest, ValueEvaluation) {
    Expression<long double> expr = m_val<long double>(5.0L);
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(expr.eval(context), 5.0L);
}

TEST_F(ExpressionTest, ValueDifferentiation) {
    Expression<long double> expr = m_val<long double>(5.0L);
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 0.0L); // Derivative of a constant is 0
}

// Test Variable class
TEST_F(ExpressionTest, VariableEvaluation) {
    Expression<long double> expr = m_var<long double>("x");
    map<string, long double> context = {{"x", 10.0L}};
    EXPECT_DOUBLE_EQ(expr.eval(context), 10.0L);
}

TEST_F(ExpressionTest, VariableDifferentiation) {
    Expression<long double> expr = m_var<long double>("x");
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 1.0L); // Derivative of x w.r.t. x is 1
}

// Test Addition
TEST_F(ExpressionTest, AdditionEvaluation) {
    Expression<long double> expr = m_val<long double>(5.0L) + m_val<long double>(10.0L);
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(expr.eval(context), 15.0L);
}

TEST_F(ExpressionTest, AdditionDifferentiation) {
    Expression<long double> expr = m_var<long double>("x") + m_val<long double>(10.0L);
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 1.0L); // Derivative of x + 10 w.r.t. x is 1
}

// Test Multiplication
TEST_F(ExpressionTest, MultiplicationEvaluation) {
    Expression<long double> expr = m_val<long double>(5.0L) * m_val<long double>(10.0L);
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(expr.eval(context), 50.0L);
}

TEST_F(ExpressionTest, MultiplicationDifferentiation) {
    Expression<long double> expr = m_var<long double>("x") * m_val<long double>(10.0L);
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 10.0L); // Derivative of x * 10 w.r.t. x is 10
}

// Test Sine Function
TEST_F(ExpressionTest, SineEvaluation) {
    Expression<long double> expr = m_val<long double>(0.0L).sin();
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(expr.eval(context), 0.0L); // sin(0) = 0
}

TEST_F(ExpressionTest, SineDifferentiation) {
    Expression<long double> expr = m_var<long double>("x").sin();
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context = {{"x", 0.0L}};
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 1.0L); // Derivative of sin(x) at x=0 is cos(0) = 1
}

// Test Exponential Function
TEST_F(ExpressionTest, ExponentialEvaluation) {
    Expression<long double> expr = m_val<long double>(0.0L).exp();
    map<string, long double> context;
    EXPECT_DOUBLE_EQ(expr.eval(context), 1.0L); // exp(0) = 1
}

TEST_F(ExpressionTest, ExponentialDifferentiation) {
    Expression<long double> expr = m_var<long double>("x").exp();
    Expression<long double> diffExpr = expr.diff("x");
    map<string, long double> context = {{"x", 0.0L}};
    EXPECT_DOUBLE_EQ(diffExpr.eval(context), 1.0L); // Derivative of exp(x) at x=0 is exp(0) = 1
}

// Test Prettify
TEST_F(ExpressionTest, Prettify) {
    Expression<long double> expr = (m_var<long double>("x") + m_val<long double>(5.0L)) * m_val<long double>(2.0L);
    Expression<long double> prettyExpr = expr.prettify();
    map<string, long double> context = {{"x", 3.0L}};
    EXPECT_DOUBLE_EQ(prettyExpr.eval(context), 16.0L); // (x + 5) * 2 at x=3 is 16
}

// Test ToString
TEST_F(ExpressionTest, ToString) {
    Expression<long double> expr = m_var<long double>("x") + m_val<long double>(5.0L);
    EXPECT_EQ(expr.to_string(), "(x + 5)");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}