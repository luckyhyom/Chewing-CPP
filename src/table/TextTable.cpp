#include "TextTable.h"
#include <iostream>

TextTable::TextTable(int row, int col) : Table(row, col) {}

TextTable::~TextTable()
{
    std::cout << "Text Table Done" << std::endl;
}

std::string TextTable::print_table()
{
    int max_length = 5;
    std::string data;
    std::string row_line;
    for (size_t i = 0; i < col; i++)
        row_line.append("----------");
    row_line.append("-\n");
    data.append(row_line);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            if (data_base[i][j] != nullptr)
            {
                data.append("| ");
                if (get_cell(i, j).length() > max_length)
                {
                    data.append(get_cell(i, j).substr(0, max_length) + ".. ");
                }
                else if (get_cell(i, j).length() < max_length)
                {
                    data.append(
                        get_cell(i, j) + std::string(max_length + 3 - get_cell(i, j).length(), ' '));
                }
                else
                {
                    data.append(get_cell(i, j) + "  ");
                }
            }
            else
            {
                data.append("|         ");
            }
        }
        data.append("|\n");
        data.append(row_line);
    }
    return data;
}
