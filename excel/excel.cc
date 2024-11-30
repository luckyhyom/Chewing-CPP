#include <iostream>
using namespace std;

class Table;
class Cell {
    Table* table;  // 어느 테이블에 속해있는지
    std::string data;
    int x, y;  // 테이블 에서의 위치
    public:
        Cell(const std::string& data) : data(data){};
};

class Table {
    Cell*** data_base;  // 왜 3중 포인터 인지 잘 생각해보세요!
    int row;
    int col;

    public:
        Table(int row, int col);
        virtual std::string print_table() = 0;
        void reg_cell(Cell* c, int row, int col);  // Cell 을 등록한다
        std::string get_cell(int row,int col);  // 해당 위치의 Cell 데이터를 얻는다.
        int get_row();
        int get_col();
        ~Table();
};

Table::Table(int row, int col): row(row), col(col) {
    data_base = new Cell**[row];
    for (size_t i = 0; i < row; i++)
    {
        data_base[i] = new Cell*[col];
        for (size_t j = 0; j < col; j++) data_base[i][j] = nullptr;
    }
}

Table::~Table() {
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) delete data_base[i][j];
        delete[] data_base[i];
    }
    delete[] data_base;
}

//Table::void reg_cell(Cell* c, int row, int col);  // Cell 을 등록한다
//Table::get_cell_std::string(int row,int col);  // 해당 위치의 Cell 데이터를 얻는다.

ostream& operator<<(ostream& o, Table& t) {
    o << t.print_table();
    return o;
}

class TextTable : public Table {
    public:
        TextTable();
        std::string print_table() = 0;
        ~TextTable();
};
class CSVTable : public Table {};
class HTMLTable : public Table {};

int main() {
    return 0;
}