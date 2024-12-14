#include "CSVTable.h"
#include <iostream>

CSVTable::CSVTable(int row, int col) : Table(row, col) {}

CSVTable::~CSVTable() {
    std::cout << "CSV Table Done"  << std::endl;
}

std::string CSVTable::print_table() {
    std::string data;
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            Cell* cell = data_base[i][j];
            if(cell == nullptr) {
                continue;
            }
            data.append(cell->stringify());
            if(j < col - 1) {
                data.append(",");
            }
        }
        data.append("\n");
    }
    return data;
}

