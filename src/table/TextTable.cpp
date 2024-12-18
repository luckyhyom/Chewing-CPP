#include "TextTable.h"
#include <iostream>
#include <sstream>

// 매직 넘버를 상수로 분리
static const int MAX_LENGTH = 5;
static const std::string CELL_SEPARATOR = "| ";
static const std::string EMPTY_CELL     = "|         "; // "| " + 9칸 = 총 11글자

TextTable::TextTable(int row, int col)
    : Table(row, col)
{
}

TextTable::~TextTable()
{
    std::cout << "Text Table Done" << std::endl;
}

// 행 구분선 생성 함수
std::string TextTable::makeRowLine() const
{
    // 각 열마다 10개의 '-' ("----------")을 붙이고
    // 마지막에 '-' 한 번 더 붙여서 총 col * 10 + 1개의 '-'
    std::ostringstream oss;
    for (size_t c = 0; c < col; ++c) {
        oss << "----------";
    }
    oss << "-\n";  // 마지막 '-' + 개행
    return oss.str();
}

// 셀 내용 포매팅 함수
std::string TextTable::formatCellContent(const std::string& content) const
{
    if (content.size() > MAX_LENGTH) {
        // MAX_LENGTH를 초과하면 잘라서 ".. "을 붙임
        return content.substr(0, MAX_LENGTH) + ".. ";
    }
    else if (content.size() < MAX_LENGTH) {
        // MAX_LENGTH보다 짧으면 공백으로 패딩
        // "  " (두 칸) 여유를 고려해 (MAX_LENGTH + 3 - content.size())칸의 공백
        return content + std::string(MAX_LENGTH + 3 - content.size(), ' ');
    }
    else {
        // 길이가 정확히 MAX_LENGTH인 경우에는 "   " 붙여줌
        return content + "   ";
    }
}

std::string TextTable::print_table()
{
    // row나 col이 0이면 테이블이 의미가 없으므로 빈 문자열 반환
    if (row == 0 || col == 0) {
        return {};
    }

    // 문자열 조립을 위한 스트림
    std::ostringstream oss;

    // 행 구분선 미리 생성
    std::string rowLine = makeRowLine();

    // 테이블 최상단 선
    oss << rowLine;

    // Labeling the columns.
    for (size_t i = 0; i < col; i++) {
        std::string label(1, static_cast<char>(Table::ASCII_VALUE_FOR_A + i));
        oss << CELL_SEPARATOR << formatCellContent(label);
    }
    
    oss << "|\n";
    oss << rowLine;    

    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            if (data_base[i][j] != nullptr) {
                // 셀 내용을 한 번만 가져와서 사용
                std::string cellContent = get_cell(i, j);
                oss << CELL_SEPARATOR << formatCellContent(cellContent);
            } else {
                // 비어있는 셀 처리
                oss << EMPTY_CELL;
            }
        }
        oss << "|\n";       // 각 행 끝의 세로줄 + 개행
        oss << rowLine;    // 행 구분선
    }

    return oss.str();
}
