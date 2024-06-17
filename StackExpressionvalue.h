
#ifndef VELKAULOHA_STACKEXPRESSIONVALUE_H
#define VELKAULOHA_STACKEXPRESSIONVALUE_H


#include <variant>
#include <string>

using CValue = std::variant<std::monostate, double, std::string>;
enum class Type { //Type describes type of StackExpressionValue which may be Double, String or Invalid if something goes wrong
    Double,
    String,
    Invalid
};
/*Structure  StackExpressionValue is used to get CValue in getValue()
 * method in CSpreadsheet to get the CValue from the Expression in Cell
 * */
struct StackExpressionValue {
    Type type;
    CValue value;
};

#endif //VELKAULOHA_STACKEXPRESSIONVALUE_H
