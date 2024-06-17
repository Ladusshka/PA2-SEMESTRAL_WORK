
#ifndef VELKAULOHA_CSPREADSHEET_H
#define VELKAULOHA_CSPREADSHEET_H

#include "CPos.h"
#include "Sell.h"
#include <iostream>
#include <variant>
#include <unordered_map>

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
using CValue = std::variant<std::monostate, double, std::string>;


/*Two unordered maps to remember the CValues in each position which is called dfs_check but in fact is cache
* And one to remember the expression which is inside of the map on the specific position
*/
class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    bool load(std::istream &is);

    bool save(std::ostream &os) const;

    bool setCell(CPos pos, std::string contents);

    CValue getValue(CPos pos);

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1);

private:
    std::unordered_map<CPos, Cell, std::hash<CPos>> excel;
    std::unordered_map<CPos, CValue, std::hash<CPos>> dfs_check;
};

#endif // VELKAULOHA_CSPREADSHEET_H

