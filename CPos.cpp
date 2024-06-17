#include "CPos.h"
#include <stdexcept>
#include <cctype>

/*
 * function stringToNumber converts any string
 * to size_t number . It is used to create from A -- 0, B -- 1, aa -- 26 and so on
 * */
size_t stringToNumber(const std::string &str) {
    size_t number = 0;
    for (char c: str) {
        c = std::tolower(c);
        if (c < 'a' || c > 'z') continue;
        number = number * 26 + (c - 'a' + 1);
    }
    return number - 1;
}


/*
 * Constructor of CPos that reads the string, analyzes it and makes a pair of size_t numbers which will be written to
 * private field "position"
 *
 *
 * In loop the argument string is being read until it meets the digit and then stops
 * After that the string what was collected is transformed to size_t number by stringToNumber() function
 *
 * The other part creates from the char its analog in size_t and writes it to the result part of the pair
 * */
CPos::CPos(std::string_view str) {
    std::string symbols;
    symbols.reserve(20);
    size_t index = 0;

    while (!isdigit(str[index])) {
        if (!isdigit(str[index]) && !isalpha(str[index])) {
            throw std::invalid_argument("Invalid character in Column");
        }
        symbols.push_back(str[index]);
        index++;
    }

    size_t number = 0;
    for (size_t i = index; i < str.size(); i++) {
        if (!isdigit(str[i])) {
            throw std::invalid_argument("Invalid character in Row");
        }
        number = number * 10 + (str[i] - '0');
    }

    position.first = stringToNumber(symbols);
    position.second = number;
}

CPos::CPos(std::pair<size_t, size_t> coord) : position(coord) {}

CPos::CPos(const CPos &other) : position(other.position) {}

bool CPos::operator==(const CPos &other) const {
    return position == other.position;
}

std::pair<size_t, size_t> CPos::getPos() const {
    return position;
}


void CPos::addMovement(std::pair<size_t, size_t> mov) {
    position.first += mov.first;
    position.second += mov.second;
}

std::ostream &operator<<(std::ostream &os, const CPos &a) {
    os << a.position.first << " " << a.position.second;
    return os;
}

bool operator<(const CPos &a, const CPos &b) {
    if (a.position.first != b.position.first) {
        return a.position.first < b.position.first;
    }
    return a.position.second < b.position.second;
}
