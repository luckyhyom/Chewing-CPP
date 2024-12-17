#include "HTMLTable.h"
#include <iostream>
#include <sstream>

// HTMLTable: Table을 상속받아 HTML 형식으로 출력하기 위한 클래스
HTMLTable::HTMLTable(int row, int col)
    : Table(row, col)
{
}

HTMLTable::~HTMLTable()
{
    std::cout << "HTML Table Done" << std::endl;
}

// 출력 시 들여쓰기에 사용할 문자열 상수
static const std::string INDENT_TABLE = "    "; // <table> 내부에 대한 들여쓰기
static const std::string INDENT_ROW   = "        "; // <tr> 내부에 대한 들여쓰기
static const std::string INDENT_CELL  = "            "; // <td> 내부에 대한 들여쓰기

// 개별 셀을 HTML td 태그로 감싸는 헬퍼 함수
std::string HTMLTable::renderCell(Cell* cell) const
{
    std::ostringstream oss;
    oss << INDENT_CELL << "<td>";
    if (cell) {
        oss << cell->stringify();
    }
    oss << "</td>\n";
    return oss.str();
}

// 한 행(row)을 <tr></tr> 태그로 감싸는 헬퍼 함수
std::string HTMLTable::renderRow(size_t rowIndex) const
{
    std::ostringstream oss;
    oss << INDENT_ROW << "<tr>\n";
    for (size_t j = 0; j < col; ++j) {
        Cell* cell = data_base[rowIndex][j];
        oss << renderCell(cell);
    }
    oss << INDENT_ROW << "</tr>\n";
    return oss.str();
}

std::string HTMLTable::print_table()
{
    // row나 col이 0이면 의미가 없으므로 빈 문자열 반환 (혹은 "<p>Empty Table</p>" 등)
    if (row == 0 || col == 0) {
        return {};
    }

    std::ostringstream oss;

    // 테이블 시작
    // 필요하다면 class나 style 속성 등 추가 가능. 예: <table class="my-table" style="border:1px solid black;">
    oss << "<table>\n";

    for (size_t i = 0; i < row; ++i) {
        oss << renderRow(i);
    }

    // 테이블 끝
    oss << "</table>\n";

    return oss.str();
}
