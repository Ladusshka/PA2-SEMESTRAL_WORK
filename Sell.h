
#ifndef VELKAULOHA_Cell_H
#define VELKAULOHA_Cell_H

#include <list>
#include <string>
#include <memory>
#include "SinglePartOfExpression.h"

/*Cell is class that is used in unordered_map in Excel sheet
 * to collect the expression which may be used in Cell of the sheet
 * it may be only number, string or "=A1+Rammstein420*83"
 *
 * All the data is collected inside <list> of unique_ptr of
 * Derived classes of SinglePartOfExpression
 * unique_ptr is used to be not scared to work with memory and that each expression in Cell
 * had its, only its, parts of expression
 *
 *  Field "content" is used to collect the data which will be given first in CSpreadsheet::SetCell()
 *  and after will be used in CSpreadsheet::load() and CSpreadsheet::save() to write and read data of each Cell
 *
 *
 *  movement_X and movement_Y are used for work with absolute positions in the CSpreadsheet::copyRect()
 *  it will add two numbers to each of position when it is needed
 *
 *  Other methods are default constructor, copy constructor and operator "="
 * */


struct Cell {

    std::list<std::unique_ptr<SinglePartOfExpression>> expression;
    std::string content;
    size_t movement_X = 0;
    size_t movement_Y = 0;

    Cell();

    Cell(const Cell &other);

    Cell &operator=(const Cell &other);

};


#endif //VELKAULOHA_Cell_H
