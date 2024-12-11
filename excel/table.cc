#include <iostream>
#include "table.h"
#pragma once


Table::Table(int row, int col): row(row), col(col) {
    data_base = new Cell**[row];
    for (size_t i = 0; i < row; i++)
    {
        data_base[i] = new Cell*[col];
        for (size_t j = 0; j < col; j++) data_base[i][j] = nullptr;
    }
}

Table::~Table() {
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) delete data_base[i][j];
        delete[] data_base[i];
    }
    delete[] data_base;
}

void Table::reg_cell(Cell *c, int row, int col) {
    c->table = this;
    c->x = row;
    c->y = col;
    data_base[row][col] = c;
};

Cell* Table::get_cell(int row,int col) {
    return data_base[row][col];
};


class TextTable : public Table {
    public:
        TextTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~TextTable() { std::cout << "Text Table Done" << std::endl; };
};

std::string TextTable::print_table() {
    int max_length = 5;
    std::string data;
    std::string row_line;
    for (size_t i = 0; i < col; i++) row_line.append("----------");
    row_line.append("-\n");
    data.append(row_line);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) {
            if(data_base[i][j] != nullptr) {
                data.append("| ");
                if(get_cell(i, j)->stringify().length() > max_length) {
                    data.append(get_cell(i, j)->stringify().substr(0, max_length) + ".. ");
                } else if(get_cell(i, j)->stringify().length() < max_length) {
                    data.append(
                        get_cell(i, j)->stringify() + std::string(max_length + 3 - get_cell(i, j)->stringify().length(), ' ')
                    );
                } else {
                    data.append(get_cell(i, j)->stringify() + "  ");
                }
            } else {
                data.append("|         ");
            }
        }
        data.append("|\n");
        data.append(row_line);
    }
    return data;
}

std::string CSVTable::print_table() {
    int max_length = 5;
    std::string data;
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) {
            if(data_base[i][j] != nullptr) {
                data.append(get_cell(i, j)->stringify());
            }
            if(j < col-1) {
                data.append(",");
            }
        }
        data.append("\n");
    }
    return data;
}




std::string HTMLTable::print_table() {
    int max_length = 5;
    std::string data;
    data.append("<div>\n");
    for (size_t i = 0; i < row; i++)
    {
        data.append("   <div>\n");
        for (size_t j = 0; j < col; j++) {
            data.append("       <div>");
            if(data_base[i][j] != nullptr) {
                data.append(get_cell(i, j)->stringify());
            } else {
                data.append("Empty");
            }
            data.append("</div>\n");
        }
        data.append("   </div>\n");
    }
    data.append("</div>\n");
    return data;
}

