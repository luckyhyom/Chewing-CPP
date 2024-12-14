#ifndef HTMLTABLE_H
#define HTMLTABLE_H

#include "table.h"

class HTMLTable : public Table {
    public:
        HTMLTable(int row, int col);
        virtual std::string print_table() override;
        virtual ~HTMLTable();
};

#endif // HTMLTABLE_H