
#include "Parser.h"
#include "SinglePartOfExpression.h"
#include <memory>

void BuilderParser::opAdd() {
    auto oper = std::make_unique<COperator>('+');
    expression.push_back(std::move(oper));
}

void BuilderParser::opMul() {
    auto oper = std::make_unique<COperator>('*');
    expression.push_back(std::move(oper));
}

void BuilderParser::opSub() {
    auto oper = std::make_unique<COperator>('-');
    expression.push_back(std::move(oper));
}

void BuilderParser::opDiv() {
    auto oper = std::make_unique<COperator>('/');
    expression.push_back(std::move(oper));
}

void BuilderParser::opPow() {
    auto oper = std::make_unique<COperator>('^');
    expression.push_back(std::move(oper));
}

void BuilderParser::opNeg() {
    auto min = std::make_unique<CMinus>();
    expression.push_back(std::move(min));
}

void BuilderParser::opEq() {
    auto oper = std::make_unique<COperator>('=');
    expression.push_back(std::move(oper));
}

void BuilderParser::opNe() {
    auto oper = std::make_unique<COperator>('!');
    expression.push_back(std::move(oper));
}

void BuilderParser::opLt() {
    auto oper = std::make_unique<COperator>('<');
    expression.push_back(std::move(oper));
}

void BuilderParser::opLe() {
    auto oper = std::make_unique<COperator>('L');
    expression.push_back(std::move(oper));
}

void BuilderParser::opGt() {
    auto oper = std::make_unique<COperator>('>');
    expression.push_back(std::move(oper));
}

void BuilderParser::opGe() {
    auto oper = std::make_unique<COperator>('G');
    expression.push_back(std::move(oper));
}

void BuilderParser::valNumber(double val) {
    auto oper = std::make_unique<CDouble>(val);
    expression.push_back(std::move(oper));
}

void BuilderParser::valString(std::string val) {
    auto oper = std::make_unique<CString>(val);
    expression.push_back(std::move(oper));
}

void BuilderParser::valReference(std::string val) {
    auto oper = std::make_unique<CCell>(val);
    expression.push_back(std::move(oper));
}


std::list<std::unique_ptr<SinglePartOfExpression>> &BuilderParser::getter_expression() {
    return expression;
}
