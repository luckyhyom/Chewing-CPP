#include "TextTable.h"
#include <iostream>

// 가독성과 재활용을 위해 상수로 분리
static const int MAX_LENGTH = 5;
static const std::string CELL_SEPARATOR = "| ";
static const std::string EMPTY_CELL = "|         "; // "| " + 9칸(' ' * 9)

TextTable::TextTable(int row, int col)
    : Table(row, col)
{
}

TextTable::~TextTable()
{
    std::cout << "Text Table Done" << std::endl;
}

// 행 구분선 생성 함수
static std::string makeRowLine(size_t colCount)
{
    // "----------"는 정확히 10글자
    // 각 열마다 10글자씩, 마지막에 "-"를 하나 더 붙여서 총 (colCount*10 + 1)개의 '-'
    // 여기서는 기존 코드처럼 반복해서 붙이는 방식을 유지
    std::string line;
    for (size_t i = 0; i < colCount; ++i) {
        line.append("----------");
    }
    line.append("-");
    line.append("\n");
    return line;
}

// 셀 내용 포매팅 함수
static std::string formatCellContent(const std::string& content)
{
    // 셀 내용의 실제 길이를 먼저 확인
    const size_t len = content.length();

    // MAX_LENGTH보다 길면 자르고 ".. " 접미어를 붙임
    if (len > MAX_LENGTH) {
        return content.substr(0, MAX_LENGTH) + ".. ";
    }
    // MAX_LENGTH보다 짧으면 공백으로 패딩
    else if (len < MAX_LENGTH) {
        // "  " 여유공간을 고려해 (MAX_LENGTH + 3 - len)개의 공백
        return content + std::string(MAX_LENGTH + 3 - len, ' ');
    }
    // 딱 같으면 기본적으로 "  " (세 칸) 붙여줌
    else {
        return content + "   ";
    }
}

std::string TextTable::print_table()
{
    std::string result;
    // 먼저 행 구분선부터 생성
    std::string rowLine = makeRowLine(col);

    // 테이블 상단 선
    result.append(rowLine);

    // 실제 데이터 출력
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            if (data_base[i][j] != nullptr) {
                // 셀 내용 한 번만 가져와서 사용
                std::string cellContent = get_cell(i, j);
                result.append(CELL_SEPARATOR);
                result.append(formatCellContent(cellContent));
            }
            else {
                // 비어 있는 셀 처리
                result.append(EMPTY_CELL);
            }
        }
        result.append("|\n");
        // 행 끝나면 구분선 다시 출력
        result.append(rowLine);
    }

    return result;
}
