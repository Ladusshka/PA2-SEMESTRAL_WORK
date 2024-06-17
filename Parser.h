
#ifndef VELKAULOHA_PARSER_H
#define VELKAULOHA_PARSER_H

#include "expression.h"
#include "SinglePartOfExpression.h"
#include <memory>
#include <list>

/*Parser is derived class from parser given as CExprBulder
 * which contains inside list of unique_ptr of SinglePartOfExpressions
 * Each method pushes back int the right order the unique_ptr of derived class from SinglePartOfExpression
 * depending on which option is added : operator, Position of Cell or String
 * */
class BuilderParser : public CExprBuilder {
public:

    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(std::string val) override;

    void valReference(std::string val) override;

    void valRange(std::string val) override {}

    void funcCall(std::string fnName,
                  int paramCount) override {}

    std::list<std::unique_ptr<SinglePartOfExpression>> &getter_expression();

private:
    std::list<std::unique_ptr<SinglePartOfExpression>> expression;
};


#endif //VELKAULOHA_PARSER_H
