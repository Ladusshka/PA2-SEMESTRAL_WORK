//
// Created by petr on 5/11/24.
//

#include "Sell.h"

/*default constructor which will set each field of Cell to default values*/
Cell::Cell() = default;


/*In copy constructor "content" fields will copy
 * All elements from "expression" list will copy by using method "clone()" in SinglePartOfExpression.
 * Fields movement_X and movement_Y will copy to
 * */
Cell::Cell(const Cell &other) {
    this->content = other.content;

    this->expression.clear();
    for (const auto &a: other.expression) {
        this->expression.push_back(a->clone());
    }
    this->movement_X = other.movement_X;
    this->movement_Y = other.movement_Y;
}


/* operator "=" does the same thing as copy constructor but returns
 * the reference to the existing Cell object
 * this method is widely used in copyRect()
 * */

Cell &Cell::operator=(const Cell &other) {

    if (this == &other) {
        return *this;
    }
    this->content = other.content;
    this->expression.clear();
    for (const auto &a: other.expression) {
        this->expression.push_back(a->clone());
    }
    this->movement_X = other.movement_X;
    this->movement_Y = other.movement_Y;

    return *this;
}

