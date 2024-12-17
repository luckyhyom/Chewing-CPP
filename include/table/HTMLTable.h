#ifndef HTMLTABLE_H
#define HTMLTABLE_H

#include "Table.h"

class HTMLTable : public Table
{
private:
    std::string renderCell(Cell* cell) const;
    std::string renderRow(size_t rowIndex) const;
public:
    HTMLTable(int row, int col);
    virtual std::string print_table() override;
    virtual ~HTMLTable();
};

#endif // HTMLTABLE_H