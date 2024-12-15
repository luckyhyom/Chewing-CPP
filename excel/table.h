#ifndef TABLE_H
#define TABLE_H

#include "Cell.h"
#include <string>

class Table
{
protected:
    Cell ***data_base;
    int row, col;

public:
    Table(int row, int col);
    virtual std::string print_table() = 0;
    friend std::ostream &operator<<(std::ostream &o, Table &t); // 선언
    void reg_cell(Cell *c, int row, int col);
    std::string get_cell(int row, int col);
    int get_row();
    int get_col();
    virtual ~Table();
};

#endif // TABLE_H