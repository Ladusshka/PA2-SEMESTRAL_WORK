
#include "StackExpressionvalue.h"
#include "SinglePartOfExpression.h"
#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <stack>
#include "CPos.h"
#include "CSpreadsheet.h"
#include <iostream>
#include <sstream>
#include <cmath>


using CValue = std::variant<std::monostate, double, std::string>;

// COperator implementation
COperator::COperator(char c) : oper(c) {}

COperator::~COperator() = default;

std::unique_ptr<SinglePartOfExpression>
COperator::clone() {//returns std::unique_ptr to the COperator with same operator but other place in memory
    return std::make_unique<COperator>(*this);
}


// CMinus implementation
std::unique_ptr<SinglePartOfExpression>
CMinus::clone() {//returns std::unique_ptr to the CMinus with same filling but other place in memory
    return std::make_unique<CMinus>(*this);
}


// CCell implementation
CCell::CCell(std::string &a) : Cell_position(a) {}

CCell::~CCell() = default;

std::unique_ptr<SinglePartOfExpression>
CCell::clone() {//returns std::unique_ptr to the CCell with same pointing position to Cell in Excel sheet but other place in memory
    return std::make_unique<CCell>(*this);
}


// CDouble implementation
CDouble::CDouble(double number) : value(number) {}

CDouble::~CDouble() = default;

std::unique_ptr<SinglePartOfExpression>
CDouble::clone() {//returns std::unique_ptr to the CDouble with same double_value but other place in memory
    return std::make_unique<CDouble>(*this);
}

// CString implementation
CString::CString(std::string &str) : string_value(str) {}

CString::~CString() = default;

std::unique_ptr<SinglePartOfExpression>
CString::clone() {//returns std::unique_ptr to the CString with same string_value but other place in memory
    return std::make_unique<CString>(*this);
}


void CMinus::execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) {
    auto value1 = myStack.top(); // taking last part of expression in <stack> to operate with it
    myStack.pop();// deleting that element

    if (value1.type == Type::Invalid ||
        value1.type == Type::String) { // checking whether the type of expression is std::string or pointer to Sell
        // it is abandoned to put minus before std::string or std::monotype in StackExpressionvalue, because by the logic of the getting value
        // <stack> has postfix interpretation which will be done by class Parser
        // so if it will get string or pointer to Cell that is bad logical and will return std::monotype
        // with Type::Invalid to stop looking for the value
        StackExpressionValue bad;
        bad.type = Type::Invalid;
        bad.value = CValue();
        myStack.push(bad);
        return;
    }
    //if last option before unary minus was double -> will be created double value in new StackExpressionValue with other sign and put into the <stack> back
    double double_val = -std::get<double>(value1.value);
    StackExpressionValue res;
    res.value = double_val;
    res.type = Type::Double;
    myStack.push(res);
}

void
COperator::execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) {

    //Stack will always have right things to count, because parser did check all for us correctly before putting something to the Cell expression
    auto right = myStack.top();//taking first last element from <stack>
    myStack.pop();//erasing first last element from <stack>
    auto left = myStack.top();//taking second last element from <stack>
    myStack.pop();// erasing second last element from <stack>


    /* By the fact postfix view is used for counting -> all the operations will be "left @ right" */

    StackExpressionValue res;
    if (oper == '+') {
        /*Checking if operator is "+" because it may work with different variations:
         * double + double
         * std::string + std::string
         * std::string + double = std::string
         * double + std::string = std::string
         * */

        /*in this place is not possible to get Type::Invalid because the method in the CSpreadsheet runs whole list from the start and if CCell will be there,
         * it will get recursively its value and it will be put in the <stack>*/
        if (right.type != left.type) { //check if there are not the same type
            std::string str_val; //variables
            std::string double_val;

            if (right.type == Type::Double) { // if one of them is double -> the other one is string
                str_val = std::get<std::string>(left.value);//getting the std::string from StackExpressionValue
                double_val = std::to_string(std::get<double>(right.value)); // making the std::string from double
                res.value = (str_val + double_val);//adding one to other

            } else {// the second last(right one) one is std::string -> convert to std::string first last(left one) to the std::string
                str_val = std::get<std::string>(right.value);
                double_val = std::to_string(std::get<double>(left.value));
                res.value = (double_val + str_val);//adding two new std::strings
            }
        } else { // if they are the same type
            if (right.type == Type::Double) { //check if they are both double
                res.value = std::get<double>(right.value) + std::get<double>(left.value);//just adding them
                res.type = Type::Double;
                myStack.push(res); // pushing the new StackExpressionvalue and returning
                return;
            } else {//both std::string -> adding them and returning
                res.value = std::get<std::string>(left.value) + std::get<std::string>(right.value);
            }
        }
        res.type = Type::String; //adding new Type::String StackExpressionvalue with the result to <stack> and returning
        myStack.push(res);
        return;
    }

    if (right.type != left.type) { // if type of one differs it is not possible to make any kind of operation
        //  like dividing,multiplying ... strings between other std::strings or doubles
        //  so will be return StackExpressionValue with Type::Invalid
        StackExpressionValue bad;
        bad.type = Type::Invalid;
        bad.value = CValue();
        myStack.push(bad);
        return;
    }
    if (oper == '-' || oper == '*' || oper == '^' ||
        oper == '/') { //checking whether operator is other math binary operator
        if (right.type == Type::String) { // if it is string -> return Type::Invalid StackExpressionValue in <stack>
            StackExpressionValue bad;
            bad.type = Type::Invalid;
            bad.value = CValue();
            myStack.push(bad);
            return;
        }//in this part StackExpressionvalues will only be Type::Double
        switch (oper) { // doing operation depending on operator by itself
            case '-':
                res.value =
                        std::get<double>(left.value) -
                        std::get<double>(right.value); // if it is "-" -> left - right to result value
                break;
            case '*':
                res.value = std::get<double>(left.value) *
                            std::get<double>(right.value);// if it is "*" -> left * right result value
                break;
            case '/':// if it is "/" -> left / right
                if (std::get<double>(right.value) == 0) {  // check division by zero
                    StackExpressionValue bad; //if dividing by zero -> push to <stack> Type::Invalid
                    bad.type = Type::Invalid;
                    bad.value = CValue();
                    myStack.push(bad);
                    return;
                }
                res.value = std::get<double>(left.value) /
                            std::get<double>(right.value); // if not left / right result value
                break;
            case '^':
                res.value = pow(std::get<double>(left.value),
                                std::get<double>(right.value)); // if it is "^" -> left^right to result value
                break;
        }
    } else { // case of logical binary operators
        if (right.type == Type::String) { // std::strings are allowed to compare
            std::string rightStr = std::get<std::string>(right.value); // reading right std::string
            std::string leftStr = std::get<std::string>(left.value); // reading right std::string
            switch (oper) {  // just comparing operators as it should be
                case '=':
                    res.value = (double) (leftStr ==
                                          rightStr); // result will be in any case double value taking 0 or 1 as bool
                    break;
                case '!':
                    res.value = (double) (leftStr != rightStr);
                    break;
                case '<':
                    res.value = (double) (leftStr < rightStr);
                    break;
                case 'L':
                    res.value = (double) (leftStr <= rightStr);
                    break;
                case '>':
                    res.value = (double) (leftStr > rightStr);
                    break;
                case 'G':
                    res.value = (double) (leftStr >= rightStr);
                    break;
            }
        } else {
            double rigthNum = std::get<double>(right.value); // reading right double
            double leftNum = std::get<double>(left.value); // reading left double
            switch (oper) { //comparing doubles as they should be
                case '=':
                    res.value = (double) (leftNum == rigthNum);
                    break;
                case '!':
                    res.value = (double) (leftNum != rigthNum);
                    break;
                case '<':
                    res.value = (double) (leftNum < rigthNum);
                    break;
                case 'L':
                    res.value = (double) (leftNum <= rigthNum);
                    break;
                case '>':
                    res.value = (double) (leftNum > rigthNum);
                    break;
                case 'G':
                    res.value = (double) (leftNum >= rigthNum);
                    break;
            }
        }

    }

    res.type = Type::Double; // StackExpressionvalue will bw double because no mater it was logical( return 0 or 1) or mathematical (double value counted in math operator)
    myStack.push(res); // pushing to <stack> of StackExpression Values


}

void CCell::execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) {
    std::stringstream ss(Cell_position);  // reading CCell string_position to the std::stringstream
    std::string tmp;
    int counter = 0; // counter of "$"
    std::string result;

    result.reserve(Cell_position.size());

    while (std::getline(ss, tmp,
                        '$')) { //dividing the hole string_position to smaller and adding them to result to get the CPos after
        counter++;
        result += tmp;
    }
    counter--;

    CPos position(result);//creating CPos  on result position
    if (counter == 1 && Cell_position[0] == '$') { // deciding whether it is only absolute column
        movement.first = 0; //giving no movement to movement by columns
    } else if (counter == 1 && Cell_position[0] != '$') {// deciding whether there is only absolute row
        movement.second = 0; // giving no movement to movement by rows
    } else if (counter == 2) {// both up cases
        movement.first = 0;
        movement.second = 0;
    }
    position.addMovement(movement); // adding movement to the position of the Cell

    CValue value = cs.getValue(position); // getting value of that Cell by its position
    //creating new StackExpression value and returning it

    if (std::holds_alternative<double>(value)) {
        StackExpressionValue tmp3;
        tmp3.value = value;
        tmp3.type = Type::Double;
        myStack.push(tmp3);
        return;
    }
    if (std::holds_alternative<std::string>(value)) {
        StackExpressionValue tmp3;
        tmp3.value = value;
        tmp3.type = Type::String;
        myStack.push(tmp3);
        return;
    }
    StackExpressionValue tmp3;
    tmp3.value = value;
    tmp3.type = Type::Invalid;
    myStack.push(tmp3);
}


void CDouble::execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) {
    StackExpressionValue tmp(Type::Double,
                             value); //creating StackExpressionValue containing only double value which equals to double in CDouble
    myStack.push(std::move(tmp)); //pushing it to <stack>
}


void CString::execute(std::stack<StackExpressionValue> &myStack, std::pair<size_t, size_t> movement, CSpreadsheet &cs) {
    StackExpressionValue tmp(Type::String,
                             string_value);//creating StackExpressionValue containing only std::string value which equals to string_value in CString
    myStack.push(std::move(tmp));
}