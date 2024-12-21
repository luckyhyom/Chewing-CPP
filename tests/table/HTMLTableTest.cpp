#include <gtest/gtest.h>
#include "HTMLTable.h"
#include "StringCell.h"
#include "NumberCell.h"
#include "ExprCell.h"
#include "DateCell.h"

/**
 * TDD 장점
 * 1. Test Case 검증 자동화
 *    - 코드 수정 시 Output이 달라졌을 때 인지할 수 있음
 *    - 검증 시간 단축
 *          
 * (참고1) 내부 함수는 검증하지 않는다.
 * 일반적으로 리팩토링은 테스트 대상의 Output을 유지하는데
 * 내부 함수를 검증하게 되면 리팩토링할때마다 Test Code를 수정해야한다.
 * 따라서 내부 함수는 검증하지 않는다.
 */

TEST(HTMLTableTest, PrintTableBasic)
{
    // 2행 2열짜리 HTMLTable 생성
    HTMLTable table(2, 2);

    // Cell 생성: Cell(const std::string& data)
    auto cell1 = new StringCell(new std::string("Hello"));
    table.reg_cell(cell1, "A1");  // (0,0)에 등록

    auto cell2 = new NumberCell(new int(1234));
    table.reg_cell(cell2, "A2");  // (0,1)에 등록

    // (1,0), (1,1)는 비어있게 둠(nullptr)

    // 테이블 HTML 출력 문자열 생성
    std::string result = table.print_table();
    std::cout << "[DEBUG] HTML output:\n"
                << result << std::endl;
    // 최소한 "Hello", "1234" 문자열이 포함되어야 함
    EXPECT_NE(std::string::npos, result.find("Hello"));
    EXPECT_NE(std::string::npos, result.find("1234"));

    // HTML 태그 구조 확인: <table>, <tr>, <td>가 들어있는지
    EXPECT_NE(std::string::npos, result.find("<table>"));
    EXPECT_NE(std::string::npos, result.find("</table>"));
    EXPECT_NE(std::string::npos, result.find("<tr>"));
    EXPECT_NE(std::string::npos, result.find("<td>"));
    EXPECT_NE(std::string::npos, result.find("</td>"));

    // Label 포함 여부
    EXPECT_NE(std::string::npos, result.find("A"));
    EXPECT_NE(std::string::npos, result.find("B"));
}

TEST(HTMLTableTest, PrintTableEmpty)
{
    // 0행 또는 0열인 경우: print_table()는 빈 문자열 반환한다고 가정
    HTMLTable tableEmptyRow(0, 5);
    std::string resultRow = tableEmptyRow.print_table();
    EXPECT_TRUE(resultRow.empty()) << "0행일 때는 빈 문자열이어야 합니다.";

    HTMLTable tableEmptyCol(5, 0);
    std::string resultCol = tableEmptyCol.print_table();
    EXPECT_TRUE(resultCol.empty()) << "0열일 때는 빈 문자열이어야 합니다.";
}

TEST(HTMLTableTest, PrintTablePartialCells)
{
    // 일부분만 셀을 등록했을 때, 나머지 셀이 비어있는 <td></td>로 표현되는지 확인
    HTMLTable table(2, 2);

    auto cell = new StringCell(new std::string("Data"));
    table.reg_cell(cell, "A1");  // (0,0)에만 등록

    // (0,1), (1,0), (1,1)는 비어있음(nullptr)

    std::string result = table.print_table();

    // "Data"는 포함되어야 함
    EXPECT_NE(std::string::npos, result.find("Data"));

    // <td> 태그가 총 6번 나와야 함 (2행 x 2열) + Label 2열
    size_t tdCount = 0;
    size_t pos = 0;
    while ((pos = result.find("<td>", pos)) != std::string::npos) {
        ++tdCount;
        pos += 6; // "<td>".size() == 6
    }
    EXPECT_EQ(6u, tdCount) << "2행 x 2열 + Label 2열 이므로 <td>가 6개 있어야 합니다.";
}
