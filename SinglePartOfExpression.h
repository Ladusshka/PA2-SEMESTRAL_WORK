#ifndef VELKAULOHA_SINGLEPARTOFEXPRESSION_H
#define VELKAULOHA_SINGLEPARTOFEXPRESSION_H

#include <stack>
#include <memory>
#include <variant>
#include "StackExpressionvalue.h"


using CValue = std::variant<std::monostate, double, std::string>;


/*SinglePartOfExpression is one of the most important class in whole project
 * it describes the part of expression that may appear in any Cell(field "expression")
 * it does not hold the CValue but contains the information about what is inside the Cell expression may be
 * All derived methods are used in <stack> of expressions in CSpreedSheet::getValue() method
 * to get the CValue from Cell if it contains any kind of Expression
 * They may be:
 *          1) any kind of math or logical binary operator
 *          2) double value which may be sooner converted to the CValue
 *          3) std::string value which also may be converted to the CValue
 *          4) Unary minus operator
 *          5) Cell pointer, to get expressions from
 *
 * */

class CSpreadsheet;

class SinglePartOfExpression {
public:
    virtual ~SinglePartOfExpression() = default;//default constructor


    /*method clone() which create the new object of SinglePartOfExpression with same filling
    * but in other place in memory to which the std::unique_ptr will point (used for Cell::expression copy constructor or operator "=" )*/
    virtual std::unique_ptr<SinglePartOfExpression> clone() = 0;

    /*Execute method will be used in CSpreadsheet to count the CValue in getValue() method. it will operate the existing <stack>
     * and insert StackExpressionValue depending on last (or two last) StackExpressionValues in <stack>
     * this method is crucial for work of getValue() because int the end of work in Stack will be left
     * StackExpression value with the result CValue of the whole expression of Cell
     * */
    virtual void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) = 0;
};

/*Derived expression class describing unary minus*/
class CMinus : public SinglePartOfExpression {
public:
    std::unique_ptr<SinglePartOfExpression> clone() override;

    void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) override;
};


/*Derived expression class describing any kind of Binary operator*/
class COperator : public SinglePartOfExpression {
public:
    explicit COperator(char c);

    ~COperator() override;

    std::unique_ptr<SinglePartOfExpression> clone() override;

    void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) override;

private:
    char oper; // there are different chars. each one stands for different binary operator
};


/*Derived expression class describing position of Cell to get expression from recursively*/
class CCell : public SinglePartOfExpression {
public:
    explicit CCell(std::string &a);

    ~CCell() override;

    std::unique_ptr<SinglePartOfExpression> clone() override;

    void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) override;

private:
    std::string Cell_position; // this field describes the Cell postion with or without $
};

/*Derived expression class describing double value which will be put into expression*/
class CDouble : public SinglePartOfExpression {
public:
    explicit CDouble(double number);

    ~CDouble() override;

    std::unique_ptr<SinglePartOfExpression> clone() override;

    void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) override;

private:
    double value;
};

/*Derived expression class describing std::string value which will be put into expression*/
class CString : public SinglePartOfExpression {
public:
    explicit CString(std::string &str);

    ~CString() override;

    std::unique_ptr<SinglePartOfExpression> clone() override;

    void
    execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) override;

private:
    std::string string_value;
};

#endif // VELKAULOHA_SINGLEPARTOFEXPRESSION_H
