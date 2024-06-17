#ifndef VELKAULOHA_CPOS_H
#define VELKAULOHA_CPOS_H

#include <iostream>
#include <string>
#include <string_view>
#include <utility>


/*The CPos class is used in the CSpreadsheet class to represent the position of a cell within the spreadsheet.
 * The class provides several constructors: by the string argument, pair os size_t numbers and the copy constructor
 * Private field is only the pair of two numbers which represent the column and the row in the Excel sheet
 * A0 stands for the pair 0 and 0, B2 stands for 1 and 2, AA34 is 26 34 and so on...
 *
 *
 * operator == compares to CPositions
 * method getPos() is used for returning th pair which describes the position in Excel sheet
 *
 * addMovement() changes the current position by adding to it the movement specified as pair of two numbers
 * for example : A0.addMovement(std::make_pair(1,1)) will transform A0 to B1
 *
 * operator << is used for the output
 *
 * operator < is used to Compare two CPositions
 *
 * Inside structure hash creates the size_t value of "hashed" CPosition to insert it into the unordered_map in Excel sheet
 *
 * */
class CPos {
public:
    CPos(std::string_view str);

    CPos(std::pair<size_t, size_t> coord);

    CPos(const CPos &other);

    bool operator==(const CPos &other) const;

    std::pair<size_t, size_t> getPos() const;

    void addMovement(std::pair<size_t, size_t> mov);

    friend std::ostream &operator<<(std::ostream &os, const CPos &a);

    friend bool operator<(const CPos &a, const CPos &b);

private:
    std::pair<size_t, size_t> position;
};

namespace std {
    template<>
    struct hash<CPos> {
        size_t operator()(const CPos &pos) const {
            std::size_t h1 = std::hash<size_t>{}(pos.getPos().first);
            std::size_t h2 = std::hash<size_t>{}(pos.getPos().second);
            return h1 ^ (h2 << 1);
        }
    };
}

#endif // VELKAULOHA_CPOS_H
