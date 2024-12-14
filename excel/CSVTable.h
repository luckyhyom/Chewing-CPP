#ifndef CSVTABLE_H
#define CSVTABLE_H

#include "table.h"

class CSVTable : public Table {
    public:
        CSVTable(int row, int col);
        virtual std::string print_table() override;
        virtual ~CSVTable();
};

#endif // CSVTABLE_H