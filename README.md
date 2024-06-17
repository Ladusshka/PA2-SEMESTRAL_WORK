# PA2-SEMESTRAL_WORK
Excel in terminal
The task is to implement a class (a set of classes) that will function as a spreadsheet. The spreadsheet will allow cell operations (setting, value calculation, copying), will be able to calculate the value of a cell according to a formula, will be able to detect cyclic dependencies between cells, and will be able to save and retrieve table contents. In addition to functionality, the evaluation will focus on the appropriate design of classes, the use of polymorphism and the appropriate use of the versioning system.

The functionality is divided into several parts, the rating will depend on how many requirements your implementation can cover. Furthermore, your implementation will not have to implement its own syntactic analysis of the expressions that will appear in the cells. The parser will be available in the test environment and is included in the delivered archive in the form of a statically linked library.

The interface provides the following classes:

CSspreadsheet - a custom spreadsheet that you implement. The required public interface is described below, its implementation is up to you.

CPos - cell identifier in the table. We identify the cells in the table in a standard way: a non-empty sequence of letters A to Z indicates the column followed by a non-negative row number (we consider 0 to be a valid row number). Letters in the column name can be lowercase or uppercase (case insensitive). Columns are labeled sequentially A, B, C, ..., Z, AA, AB, AC, ..., AZ, BA, BB, ..., ZZZ, AAAA, ... Examples of cell identifiers: A7, PROG7250 , ... If you're not sure about the notation, look at the actual spreadsheet. The cell identifier entered in this way can be awkward to implement, so it is encapsulated in the CPos class, which can convert it to a more suitable representation. The implementation of the conversion and the CPos class is up to you.

CExpressionBuilder - abstract class used by the supplied expression parser. If you choose to use the supplied parser, you will need to implement the subclass and pass its instance to the parser.
parseExpression() - a function in the test environment that performs a syntactic analysis of the specified expression and passes the identified parts of this expression to your CEExpressionBuilder subclass for processing.

CValue - represents the cell value. The cell value is either undefined, a decimal number, or a string. The CValue class is a named specialization of the generic std::variant class.


Expressions in cells:
The implementation requires that table cells can be empty (undefined), can contain a number, a string, or a formula to calculate a value. Formulas have a syntax similar to a standard spreadsheet, but are limited by the number of functions implemented. The term may include:
numeric literal - integer or decimal number, optionally with an exponent, i.e. 15, 2.54, 1e+8, 1.23e-10, ...
string literal - a string is a sequence of characters enclosed in quotation marks. If the string is to contain a quote, the quote is doubled. A string literal can contain any characters, including the newline character.
cell link - the link is implemented in standard notation (column - sequence of letters, row - number). A column reference is case insensitive. Example of references: A7, ZXCV789456, ... In a formula, you can refer to a cell absolutely or relatively. Additionally, absolute/relative reference can be specified independently for row and column. An absolute reference means that the referenced cell will still be the same even if we copy the expression in the cell to another cell. A relative reference means that when you copy a formula to another cell, the referenced cell changes. An absolute reference is introduced by the $ character. Examples: A5, $A5, $A$5, A$5.
range of cells (range) - the expression defines a rectangular field of cells, the positions are given by the upper left and lower right corners, the separator is a colon character. Example: A5:X17. Cell references can again be absolute or relative, the absolute/relativity of the reference is applied when copying. Examples: A$7:$X29, A7:$X$29, ...
function - an expression in a cell can call functions and pass parameters to them. The implementation requires the following features:
    sum (range) - sums the values ​​of all cells in the given range, which is evaluated as a number. If there is no cell with a numeric value in the range, the sum function will return an undefined value.
    count (range) - count all cells that have a defined value in a given range.
    min (range) - in the given range, the values ​​of all cells will be passed, which will be evaluated as a number. The result is the value of the smallest number found. If there is no cell with a numeric value in the range, min will return an undefined value.
    max (range) - in the given range, the values ​​of all cells will be passed, which will be evaluated as a number. The result is the value of the largest number found. If there is no cell with a numeric value in the range, max will result in an undefined value.
    countval (value, range) - goes through the values ​​of all cells in the given range and counts those of them that have the same value as the evaluated value expression. Comparison of string type values ​​is clearly given, when comparing numbers consider an exact match       (no epsilon tolerance),
    if (cond, ifTrue, ifFalse) - the condition evaluates the cond condition expression. If the value of the condition is undefined or a string, the result of the if function is an undefined value. If the result of the condition is a non-zero number, the if result is the     value of the ifTrue expression, if the result of the condition is the number 0, the result of the if expression is the value of the ifFalse expression.
round brackets - allow you to change the priority of subexpression evaluation.
binary operators ^, *, / and - are used for exponentiation, multiplication, division and subtraction. All require two numeric operands. If at least one operand is of a different type, the result is an undefined value. Furthermore, division by zero is evaluated as an undefined value, compare the value 0 of the divisor directly with language operators (don't consider epsilon tolerance),
the + operator works for numbers similarly to other binary operators. In addition, it is able to concatenate operands in combination number+string, string+number and string+string. To convert a number to a string, use the std::to_string function.
unary operator - used to change the sign. It is only applicable to a numeric operand, otherwise it evaluates to an undefined value.
relational operators < <=, >, >=, <>, = are used for comparing operands. Operands must be two numeric values ​​or two string values. The result is the number 1 (the condition is true) or 0 (the condition is not true). If at least one of the operands is an undefined value, or if number-string or string-number values ​​are compared, the result of the comparison is an undefined value. Compare decimal numbers directly with language operators (no epsilon tolerance).


Operator priority and operator associativity, in order from highest priority:
    function call
    exponentiation ^, left associativity,
    unary -, right associativity,
    multiplication by * and division by /, left associativity,
    addition/concatenation + and subtraction -, left associativity,
    relational operators < <=, > and >=, left associativity,
    equality/inequality operators <> and =, left associativity.
The above rules are important especially if you decide to implement your own parser. The supplied function parseExpression() processes the input according to the procedure described above and uses the described priorities and associativity.


Class CPos:
class represents the cell identifier. The test environment only uses the constructor of this class, to which it passes a string with the name of the cell (eg "A7"). The implementation processes this string, checks it, and stores it in member variables. If the constructor receives an invalid cell identifier, it throws a std::invalid_argument exception. The internal implementation of the class and its other interface is up to you.


The parseExpression() function and the CExpressionBuilder class
The parseExpression( expr, builder ) function represents an interface to the supplied expression parser that can be used in cells. The call parameter is a string with the expression expr and the builder instance whose interface will be called during expression parsing. The function does not return any value, the processed expression will store the supplied builder instance. The function either processes the expression without error or throws a std::invalid_argument exception with an error description. To use the function correctly, you need to supply your own builder. It will be created as a descendant of the abstract class CExpressionBuilder, the implementation of this subclass is your task.

The syntactic analysis of the expression creates its postfix form from the infix notation (e.g. 4 + A2 + B$7), i.e. for the sample example it will be: 4 A2 + B$7 +. This postfix form is not created explicitly. The parser instead calls the corresponding builder methods, for example it would call:

valNumber (4)
valReference("A2")
opAdd()
valReference("B$7")
opAdd()
Postfix form is much easier to process. Store the supplied operand values ​​(valNumber, valString, valReference, ...) on the stack in your CExpressionBuilder subclass instance. The incoming operator (opAdd, opSub, ..., funcCall) means that you fetch the corresponding number of operands from your stack and concatenate those operands with the corresponding operator (eg + operator). You have to pay attention to the order of the operands, left operands are deeper on the stack. You save the result on the stack. After successful processing of the entire expression, you will have only one item on the stack - the evaluation of the entire expression.

The interface of the supplied parser is flexible. For example, it will make it possible to evaluate an expression directly during parsing - you will store the values ​​of the evaluated subexpressions on the stack. Thus, by repeatedly processing the expression in the cell, the new value of the expression can be calculated (e.g. after changing the content of a cell on which the value of the expression depends). But such a procedure is very slow. Therefore, use the supplied parser more efficiently - use it for AST construction. Repeatedly evaluating the AST is much less overhead than parsing the entire expression repeatedly.





CExpressionBuilder abstract class
opAdd()
    application of the addition/concatenation operator (+),
opSub()
    application of the subtraction operator (-),
opMul()
    application of the multiplication operator (*),
opDiv()
    application of the division operator (/),
opPow()
    application of the exponentiation operator (^),
opNeg()
    application of the unary minus (-) operator,
opEq()
    applying the comparison operator to equality (=),
opNo()
    applying the comparison operator to inequality (<>),
opLt()
    application of the less than (<) comparison operator,
opLe()
    applying the comparison operator less than or equal to (<=),
opGt()
    application of the greater than (>) comparison operator,
opGe()
    application of the comparison operator greater than or equal to (>=),
valNumber ( num )
    adding the value of a numeric literal, the parameter is the value of a number,
valString ( str )
    adding a string literal value, the parameter is a string value,
valReference ( str )
    adding a link to another cell, the parameter is the cell identifier (eg A7 or $x$12),
valRange ( str )
    adding a reference to the range of cells (range), the parameter is the identification of a rectangular section of cells (e.g. A7:$C9 or $x$12:z29),
funcCall ( fnName, paramCnt )
    adding a function call, the parameter is the function name fnName and the number of function parameters paramCnt. The supplied parser checks whether the specified function name is valid (sum, min, max, count, if or countval) and further checks that the number and       types of parameters match (e.g. if has 3 parameters and none is range, sum must have one parameter of type range, ...). So you don't have to do this check on the builder's side. Of course, checking is needed if you decide to build your own parser.

run CSspreadsheet
implicit constructor
    Creates an empty table.
copy constructor/move constructor/= operator, destructor
    Ensures correct copy operations with the instance.
save ( os )
    The method saves the contents of the instance to the specified output stream. It must be possible to reconstruct the original content of the instance from the data stored in this way. The method returns true for success, false for write error.
load ( is )
    The method retrieves the contents of a CSspreadsheet instance from the specified input stream. Returns true for success, false for error (corrupted content).
setCell ( pos, contents )
    The method replaces the content of the specified cell with content. The content is entered as a string, it can contain:
    a valid decimal number, i.e. setCell ( CPos ("A1"), "123456.789e-9" ),
    string, i.e. any sequence of characters that does not start with the = character. For example, setCell ( CPos ("A1"), "abc\ndef\\\"" ),
    the expression that will be used to calculate the value of the cell. The expression starts with =, for example: setCell ( CPos ("A1"), "=A5-($B7*c$4+sum($X9:AC$124))^2" ),
    The return value of the setCell method is true (success) or false (failure for an invalid entered expression, the cell remains unchanged). It is not entirely reasonable to recalculate the values ​​of all cells that depend on the changed cell value immediately after       calling setCell. It is more time efficient to calculate the value of the cell only with the subsequent call to getValue.
getValue ( pos )
    The method calculates the contents of the cell and returns it as the return value. If the value cannot be calculated (the cell content is not defined, the calculation contains a loop), the method returns CValue().
copyRect ( dstCell, srcCell, w, h )
    The method copies a square of cells of size w columns and h rows from position srcCell (upper left corner of the source square) to position dstCell (upper left corner of the target square). Note that the source and destination squares may overlap.
abilities ()
      class method returns composite bit flags of optional properties that your implementation handles and that are to be tested. The return value will be a combination of SPREADSHEET_xxx constants, or 0 if you don't want to perform any optional test. If your                 implementation handles the specified property, but you do not specify it in the return value of capabilities(), then the property will not be tested and you will not receive points for it. If your implementation does not handle a feature, but you leave it turned        on in capabilities, then the feature will be tested and the test will fail. In addition, if your program fails or exceeds the time limit during the test, no further tests will be performed, so you may be graded worse. It is therefore reasonable that the return          value corresponds to the actual implemented properties. The constants have the following meaning:
      SPREADSHEET_CYCLIC_DEPS - the implementation can handle cyclic dependencies between cells,
      SPREADSHEET_FUNCTIONS - the implementation handles functions in expressions,
      SPREADSHEET_FILE_IO - the implementation can save and load to/from files and deal with damaged files,
      SPREADSHEET_SPEED - cell value calculations are optimized, it makes sense to perform an optional speed test,
      SPREADSHEET_PARSER - the implementation uses its own expression parser, it makes sense to perform a bonus test.


