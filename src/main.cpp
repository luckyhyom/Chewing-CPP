#include "TextTable.h"
#include "HTMLTable.h"
#include "Table.h"
#include <iostream>
#include <limits>
#include <regex>


bool isValidCellReference(const std::string& input);

int main()
{
    std::cout << "Program started" << std::endl; // 디버깅 메시지
    TextTable table(8, 8);
    table.reg_cell(new Cell("Hello World"), "B2");

    while (1)
    {
        std::cout << table << std::endl;

        std::string location;
        std::string value;

        std::cout << "Please enter the cell location (e.g., A1, C5) where you would like to input the data. : \n";
        if (!(std::cin >> location) || !isValidCellReference(location))
        {
            std::cerr << "Invalid cell location. Please enter a valid cell reference (e.g., A1, C5)." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "input value: ";
        if (!(std::cin >> value))
        {
            std::cerr << "Invalid input for value. Please enter a valid string." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        table.reg_cell(new Cell(value), location);
    }

    return 0;
}

bool isValidCellReference(const std::string& input) {
    // ^[A-Za-z]+[0-9]+$:
    // ^          : 문자열의 시작
    // [A-Za-z]+  : 하나 이상의 알파벳 문자
    // [0-9]+     : 하나 이상의 숫자
    // $          : 문자열의 끝
    std::regex cellRegex("^[A-Za-z]+[0-9]+$");

    return std::regex_match(input, cellRegex);
}