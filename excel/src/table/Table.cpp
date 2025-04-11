#include "Table.h"
#include <iostream>
#include <cctype>


constexpr char COLUMN_START = 'A'; // 열 시작 문자
constexpr int BASE_INDEX = 1;     // Excel의 1 기반 인덱스를 0 기반으로 변환

Table::Table(int row, int col) : row(row), col(col)
{
    data_base = new Cell**[row];
    for (size_t i = 0; i < row; i++)
    {
        data_base[i] = new Cell*[col];
        for (size_t j = 0; j < col; j++)
        {
            data_base[i][j] = nullptr;
        }
    }
}

Table::~Table()
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            delete data_base[i][j];
        }
        delete[] data_base[i];
    }
    delete[] data_base;
}

void Table::reg_cell(Cell *c, std::string location)
{
    int col = std::toupper(location[0]) - COLUMN_START;
    std::string input_row = location.substr(1); // 1번 인덱스부터 끝까지
    int row = std::stoi(input_row) - BASE_INDEX;

    c->table = this;
    c->x = row;
    c->y = col;
    data_base[row][col] = c;
}

/**
 * Cell이 가진 데이터의 자료형은 추상화시킨다.
 * 따라서 Table에서 Cell의 data를 반환하지 않는다. (이렇게 되면 Table도 get_cell을 추상화해서 각각 구현해야함)
 */
Cell* Table::get_cell(int row, int col)
{
    return data_base[row][col];
}

Cell* Table::get_cell(std::string location)
{
  int col = std::toupper(location[0]) - COLUMN_START;
  std::string input_row = location.substr(1); // 1번 인덱스부터 끝까지
  int row = std::stoi(input_row) - BASE_INDEX;

  return data_base[row][col];
}

std::ostream &operator<<(std::ostream &o, Table &t)
{
    o << t.print_table(); // 각 테이블의 print_table() 함수 호출
    return o;
}