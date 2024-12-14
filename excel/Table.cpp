#include "table.h"
#include <iostream>

Table::Table(int row, int col) : row(row), col(col) {
    data_base = new Cell**[row];
    for (size_t i = 0; i < row; i++) {
        data_base[i] = new Cell*[col];
        for (size_t j = 0; j < col; j++) {
            data_base[i][j] = nullptr;
        }
    }
}

Table::~Table() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            delete data_base[i][j];
        }
        delete[] data_base[i];
    }
    delete[] data_base;
}


void Table::reg_cell(Cell* c, int row, int col) {
    c->table = this;
    c->x = row;
    c->y = col;
    data_base[row][col] = c;
}

std::string Table::get_cell(int row, int col) {
    return data_base[row][col] ? data_base[row][col]->data : "";
}

std::ostream& operator<<(std::ostream& o, Table& t) {
    o << t.print_table(); // 각 테이블의 print_table() 함수 호출
    return o;
}