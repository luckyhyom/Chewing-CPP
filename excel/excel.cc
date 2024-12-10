#include <iostream>
using namespace std;

class Table;
class TextTable;
class Cell {
    friend Table;
    friend TextTable;
    Table* table;  // 어느 테이블에 속해있는지
    std::string data;
    int x, y;  // 테이블 에서의 위치
    public:
        Cell(const std::string& data) : data(data){};
};

class Table {
    protected:
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

void Table::reg_cell(Cell *c, int row, int col) {
    c->table = this;
    c->x = row;
    c->y = col;
    data_base[row][col] = c;
};

std::string Table::get_cell(int row,int col) {
    return data_base[row][col]->data;
};

ostream& operator<<(ostream& o, Table& t) {
    o << t.print_table();
    return o;
}

class TextTable : public Table {
    public:
        TextTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~TextTable() { std::cout << "Text Table Done" << std::endl; };
};

std::string TextTable::print_table() {
    int max_length = 5;
    std::string data;
    std::string row_line;
    for (size_t i = 0; i < col; i++) row_line.append("----------");
    row_line.append("-\n");
    data.append(row_line);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) {
            if(data_base[i][j] != nullptr) {
                data.append("| ");
                if(get_cell(i, j).length() > max_length) {
                    data.append(get_cell(i, j).substr(0, max_length) + ".. ");
                } else if(get_cell(i, j).length() < max_length) {
                    data.append(
                        get_cell(i, j) + std::string(max_length + 3 - get_cell(i, j).length(), ' ')
                    );
                } else {
                    data.append(get_cell(i, j) + "  ");
                }
            } else {
                data.append("|         ");
            }
        }
        data.append("|\n");
        data.append(row_line);
    }
    return data;
}

class CSVTable : public Table {
    public:
        CSVTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~CSVTable() { std::cout << "CSVTable Done" << std::endl; };
};


std::string CSVTable::print_table() {
    int max_length = 5;
    std::string data;
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++) {
            if(data_base[i][j] != nullptr) {
                data.append(get_cell(i, j));
            }
            if(j < col-1) {
                data.append(",");
            }
        }
        data.append("\n");
    }
    return data;
}


class HTMLTable : public Table {
    public:
        HTMLTable(int row, int col): Table(row,col) {};
        std::string print_table();
        ~HTMLTable() { std::cout << "HTMLTable Done" << std::endl; };
};


std::string HTMLTable::print_table() {
    int max_length = 5;
    std::string data;
    data.append("<div>\n");
    for (size_t i = 0; i < row; i++)
    {
        data.append("   <div>\n");
        for (size_t j = 0; j < col; j++) {
            data.append("       <div>");
            if(data_base[i][j] != nullptr) {
                data.append(get_cell(i, j));
            } else {
                data.append("Empty");
            }
            data.append("</div>\n");
        }
        data.append("   </div>\n");
    }
    data.append("</div>\n");
    return data;
}


int main() {
    //TextTable table(5,5);
    //HTMLTable table(5,5);
    CSVTable table(5,5);
    table.reg_cell(new Cell("Hello World"), 3, 1);

    while (1)
    {
        std::cout << table << std::endl;
        int x, y;
        std::string value;
        
        std::cout << "select x: ";
        if (!(std::cin >> x)) {
            std::cerr << "Invalid input for x. Please enter an integer." << std::endl;
            std::cin.clear(); // 실패 상태 플래그 해제
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 잘못된 입력 무시
            continue; // 루프 처음으로 이동
        }

        std::cout << "select y: ";
        if (!(std::cin >> y)) {
            std::cerr << "Invalid input for y. Please enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "input value: ";
        if (!(std::cin >> value)) {
            std::cerr << "Invalid input for value. Please enter a valid string." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        Cell* cell = new Cell(value);
        table.reg_cell(cell, x, y);
    }
    
    return 0;
}