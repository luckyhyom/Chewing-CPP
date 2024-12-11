#ifndef TEXTTABLE_H
#define TEXTTABLE_H

#include "table.h"

class TextTable : public Table {
    public:
        TextTable(int row, int col);
        virtual std::string print_table() override;
        virtual ~TextTable();
};

#endif // TEXTTABLE_H