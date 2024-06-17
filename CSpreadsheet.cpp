
#include <sstream>
#include "CSpreadsheet.h"
#include "Parser.h"
#include "StackExpressionvalue.h"


bool CSpreadsheet::setCell(CPos pos, std::string contents) {
    BuilderParser test;


    try { //try to create list of SinglePratOfExpressions using external parser functions
        parseExpression(contents, test);
    } catch (...) {//if exception was thrown return false
        return false;
    }
    //put into Cell contents and list of expressions
    excel[pos].content = contents;
    excel[pos].expression = std::move(test.getter_expression());

    //clear cache if something new was added
    dfs_check.clear();
    return true;
}


CValue CSpreadsheet::getValue(CPos pos) {

    //check cache and if CValue on specified CPosition was found -> return it
    if (dfs_check.contains(pos)) {
        return dfs_check[pos];
    }
    if (!excel.contains(pos)) { //if there is not such position at all -> return raw invalid empty CValue
        return CValue();
    }
    dfs_check[pos] = CValue(); // putting empty CValue on specified position before looking for it

    std::stack<StackExpressionValue> myStack;//creating the <stack> of StackExpressionValues
    for (auto it_start = excel[pos].expression.begin(); it_start !=
                                                        excel[pos].expression.end(); it_start++) { //iteratng through the whole list of expressions in that Cell
        (*it_start)->execute(myStack, std::make_pair(excel[pos].movement_X, excel[pos].movement_Y),
                             *this); // running the execute method for each SinglePartOfExpression to modify the stack
        if (myStack.top().type == Type::Invalid) { //checking if everything was not right and returning empty Cvalue
            return {};
        }
    }

    dfs_check[pos] = myStack.top().value; // writing into cache the CValue finally calculated in <stack> and left on a top
    return dfs_check[pos]; // returning the calculated value
}

void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    auto dst_coord = dst.getPos(); // getting the positions as two size_t of destination Position
    auto src_coord = src.getPos(); // getting the position as two size_t of source position
    std::list<Cell> copy; // creating the copy <list> for Cells to copy things inside from to write it to destination positions after
    for (int x = 0; x < w; x++) { //iterating by width
        for (int y = 0; y < h; y++) {//iterating by height
            CPos tmp_dst(std::make_pair(dst_coord.first + x,
                                        dst_coord.second + y));//creating the position of destination by iterating
            CPos tmp_src(std::make_pair(src_coord.first + x,
                                        src_coord.second + y));//creating the position of source by iterating

            if (excel.contains(tmp_src) && tmp_dst !=
                                           tmp_src) { //check if something exists in temporary source position and if it is not the same as destination one
                copy.push_back(excel[tmp_src]); // copying the filling
                copy.back().movement_X += tmp_dst.getPos().first -
                                          tmp_src.getPos().first; // remembering the position of row to move in copying
                copy.back().movement_Y += (tmp_dst.getPos().second -
                                           tmp_src.getPos().second);// remembering the position of column to move in copying
            }
        }
    }

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            CPos tmp_dst(std::make_pair(dst_coord.first + x, dst_coord.second + y));
            CPos tmp_src(std::make_pair(src_coord.first + x, src_coord.second + y));
            if (excel.contains(tmp_src)) { // check whether something exists in the source temporary position
                excel[tmp_dst] = copy.front(); //rewriting the filling to destination from front of the list of copy of source
                copy.pop_front();//erasing from list for next CPosition
            } else {
                excel.erase(
                        tmp_dst); //if there is nothing in source temporary position -> nothing should leave in destination temporary -> erase from there
            }
        }
    }

    //clearing cache because new values were put in different Cells and it may be not valid anymore
    dfs_check.clear();

}

bool CSpreadsheet::save(std::ostream &os) const {
    // Check if the output stream is in a failed state
    if (os.fail()) {
        return false;
    }

    // Get the size of the 'excel' container
    size_t excel_len = excel.size();

    // Create a stringstream to store the data temporarily
    std::stringstream ss;

    // Write the length of the 'excel' container to the stringstream
    ss.write(reinterpret_cast<const char *>(&excel_len), sizeof(excel_len));

    // Iterate through each element in the 'excel' container
    for (auto it = excel.begin(); it != excel.end(); it++) {
        // Get the position coordinates of the Сell
        auto adressX = it->first.getPos().first;
        auto adressY = it->first.getPos().second;

        // Get the movement values of the Сell
        auto moveX = it->second.movement_X;
        auto moveY = it->second.movement_Y;

        // Get the length and content of the cell
        auto content_len = it->second.content.size();
        auto contents = it->second.content;

        // Write the movement and position data to the stringstream
        ss.write(reinterpret_cast<const char *>(&moveX), sizeof(moveX));
        ss.write(reinterpret_cast<const char *>(&moveY), sizeof(moveY));
        ss.write(reinterpret_cast<const char *>(&adressX), sizeof(adressX));
        ss.write(reinterpret_cast<const char *>(&adressY), sizeof(adressY));

        // Write the length and content of the cell to the stringstream
        ss.write(reinterpret_cast<const char *>(&content_len), sizeof(content_len));
        ss.write(contents.c_str(), content_len);
    }

    // Calculate a hash of the stringstream content for integrity checking
    auto hassh = std::hash<std::string>{}(ss.str());

    // Write the hash to the output stream
    os.write(reinterpret_cast<const char *>(&hassh), sizeof(hassh));

    // Write the content of the stringstream to the output stream
    os << ss.str();

    return true;
}

bool CSpreadsheet::load(std::istream &is) {
    // Check if the input stream is in a failed state or at end-of-file
    if (is.fail() || is.eof()) {
        return false;
    }

    size_t excel_len;
    size_t hassh;

    // Read the hash from the input stream
    is.read(reinterpret_cast<char *>(&hassh), sizeof(hassh));

    // Create a stringstream to store the remaining data temporarily
    std::stringstream ss;
    ss << is.rdbuf();
    if (ss.fail()) {
        return false;
    }

    // Calculate the hash of the stringstream content for integrity checking
    auto calculated_hassh = std::hash<std::string>{}(ss.str());
    if (hassh != calculated_hassh) {
        return false;
    }

    // Read the length of the excel  from the stringstream
    ss.read(reinterpret_cast<char *>(&excel_len), sizeof(excel_len));

    // Iterate through each element to read its data
    for (size_t i = 0; i < excel_len; i++) {
        size_t moveX, moveY, adressX, adressY, content_len;

        // Read the movement and position data from the stringstream
        ss.read(reinterpret_cast<char *>(&moveX), sizeof(moveX));
        ss.read(reinterpret_cast<char *>(&moveY), sizeof(moveY));
        ss.read(reinterpret_cast<char *>(&adressX), sizeof(adressX));
        ss.read(reinterpret_cast<char *>(&adressY), sizeof(adressY));
        ss.read(reinterpret_cast<char *>(&content_len), sizeof(content_len));

        // Read the content of the cell from the stringstream
        std::string contents;
        contents.resize(content_len);
        ss.read(contents.data(), content_len);

        // Create a position object from the read coordinates
        CPos pos(std::make_pair(adressX, adressY));

        // Set the cell content at the read position
        if (!setCell(pos, contents)) {
            return false;
        }

        // Set the movement values for the cell
        excel[pos].movement_X = moveX;
        excel[pos].movement_Y = moveY;
    }


    return true;
}