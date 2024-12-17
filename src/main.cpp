#include "TextTable.h"
#include "HTMLTable.h"
#include "table.h"
#include <iostream>
#include <limits>

int main()
{
    std::cout << "Program started" << std::endl; // 디버깅 메시지
    HTMLTable table(5, 5);
    table.reg_cell(new Cell("Hello World"), 3, 1);

    while (1)
    {
        std::cout << table << std::endl;

        int x, y;
        std::string value;

        std::cout << "select DEBUG YES JAM: ";
        if (!(std::cin >> x))
        {
            std::cerr << "Invalid input for x. Please enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "select y: ";
        if (!(std::cin >> y))
        {
            std::cerr << "Invalid input for y. Please enter an integer." << std::endl;
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

        table.reg_cell(new Cell(value), x, y);
    }

    return 0;
}
