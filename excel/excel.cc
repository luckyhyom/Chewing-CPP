#include <iostream>
#include "cell.h"
#include "table.h"

using namespace std;

ostream& operator<<(ostream& o, Table& t) {
    o << t.print_table();
    return o;
}

int main() {
    //TextTable table(5,5);
    //HTMLTable table(5,5);
    CSVTable table(5,5);
    table.reg_cell(new Cell("Hello World"), 3, 1);

    while (1)
    {
        std::cout << table << std::endl;
        int x, y;
        std::string value;
        
        std::cout << "select x: ";
        if (!(std::cin >> x)) {
            std::cerr << "Invalid input for x. Please enter an integer." << std::endl;
            std::cin.clear(); // 실패 상태 플래그 해제
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 잘못된 입력 무시
            continue; // 루프 처음으로 이동
        }

        std::cout << "select y: ";
        if (!(std::cin >> y)) {
            std::cerr << "Invalid input for y. Please enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "input value: ";
        if (!(std::cin >> value)) {
            std::cerr << "Invalid input for value. Please enter a valid string." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        Cell* cell = new Cell(value);
        table.reg_cell(cell, x, y);
    }
    
    return 0;
}