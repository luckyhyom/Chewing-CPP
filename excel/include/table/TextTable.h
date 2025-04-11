#ifndef TEXTTABLE_H
#define TEXTTABLE_H

#include "Table.h"

class TextTable : public Table
{
private:
    std::string makeRowLine() const;
    std::string formatCellContent(const std::string& content) const;
public:
    TextTable(int row, int col);
    virtual std::string print_table() override;
    virtual ~TextTable();
};

#endif // TEXTTABLE_H