#include <iostream>
#pragma once
#include "cell.h"

using namespace std;

class Table {
    protected:
        Cell*** data_base;  // 왜 3중 포인터 인지 잘 생각해보세요!
        int row;
        int col;

    public:
        Table(int row, int col);
        virtual std::string print_table() = 0;
        void reg_cell(Cell* c, int row, int col);  // Cell 을 등록한다
        Cell* get_cell(int row,int col);  // 해당 위치의 Cell 데이터를 얻는다.
        int get_row();
        int get_col();
        ~Table();
};

class CSVTable : public Table {
    public:
        CSVTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~CSVTable() { std::cout << "CSVTable Done" << std::endl; };
};

class HTMLTable : public Table {
    public:
        HTMLTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~HTMLTable() { std::cout << "HTMLTable Done" << std::endl; };
};
