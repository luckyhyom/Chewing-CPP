#include "HTMLTable.h"
#include <iostream>

HTMLTable::HTMLTable(int row, int col) : Table(row, col) {}

HTMLTable::~HTMLTable()
{
    std::cout << "HTML Table Done" << std::endl;
}

std::string HTMLTable::print_table()
{
    std::string data;
    data.append("<div>");
    for (size_t i = 0; i < row; i++)
    {
        data.append("   <div>");
        for (size_t j = 0; j < col; j++)
        {
            Cell *cell = data_base[i][j];
            data.append("       <div>");
            if (cell == nullptr)
            {
                data.append("</div>\n");
                continue;
            }
            data.append(cell->stringify());
            data.append("</div>\n");
        }
        data.append("   </div>\n");
    }
    data.append("</div>\n");
    return data;
}
