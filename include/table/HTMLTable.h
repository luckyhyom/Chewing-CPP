#ifndef HTMLTABLE_H
#define HTMLTABLE_H

#include "Table.h"

class HTMLTable : public Table
{
private:
    std::string renderCell(Cell* cell) const;
    // 오버로딩된 함수: std::string 인자를 받음
    std::string renderCell(const std::string content) const;

    std::string renderLabel() const;
    std::string renderRow(size_t rowIndex) const;
public:
    HTMLTable(int row, int col);
    virtual std::string print_table() override;
    virtual ~HTMLTable();
};

#endif // HTMLTABLE_H