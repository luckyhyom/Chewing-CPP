#ifndef CELL_H
#define CELL_H

#include <string>

class Table;

class Cell
{
    friend class Table;
    Table *table;
    std::string data;
    int x, y;

public:
    Cell(const std::string &data);
    int to_numeric();
    std::string stringify();
};

#endif // CELL_H