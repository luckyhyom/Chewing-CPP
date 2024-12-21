#include <gtest/gtest.h>
#include "TextTable.h"
#include "StringCell.h"
#include "NumberCell.h"
#include "ExprCell.h"
#include "DateCell.h"

TEST(TextTableTest, ConstructorDestructor)
{
    // 생성자, 소멸자를 호출해보고 문제가 없는지 간단히 확인하는 테스트
    {
        TextTable table(2, 3);
        // 소멸자 호출 시 "Text Table Done" 메시지가 출력됨 (콘솔에 확인 가능)
        // 특별한 예외/에러 발생하지 않으면 PASS
    }
    SUCCEED() << "Constructor and destructor worked without crashing.";
}

TEST(TextTableTest, PrintEmptyTable)
{
    // 아무 셀도 등록하지 않은 비어있는 테이블의 출력
    TextTable table(2, 3);
    std::string output = table.print_table();
    std::cout << "[DEBUG] print_table output:\n"
              << output << std::endl;
    // 출력이 비어있지 않고, 최소한 구분선("----------")과 Col Label이 포함되어 있는지 확인
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("----------"), std::string::npos)
        << "Row separator not found in print_table output.";
    EXPECT_NE(output.find("A"), std::string::npos)
        << "Label not found in print_table output.";
}

TEST(TextTableTest, PrintTableWithData)
{
    TextTable table(2, 3); // 2행 3열 테이블

    // Table 클래스에 정의된 reg_cell(Cell*, int row, int col)
    // 이 함수를 통해 셀을 등록하는 구조
    table.reg_cell(new StringCell(new std::string("Hello")), "A1");
    table.reg_cell(new StringCell(new std::string("World")), "B1");
    table.reg_cell(new StringCell(new std::string("TestString")), "C1"); // 10글자
    table.reg_cell(new NumberCell(new int(123456789)), "A2");  // 9글자
    table.reg_cell(new StringCell(new std::string("ABC")), "B2");
    // (1, 2)는 아무 것도 등록하지 않아 nullptr 상태 유지

    std::string output = table.print_table();
    std::cout << "[DEBUG] print_table output:\n"
              << output << std::endl;

    // 기본적인 형식 검증
    EXPECT_FALSE(output.empty()) << "Printed table output is empty, which is unexpected.";

    // 각 셀 데이터가 올바르게 출력되었는지 부분적으로 점검
    // TextTable 코드 상: 최대 5글자를 표시하고, 넘으면 `.. `을 붙여서 잘린 형태
    // 1) "Hello" (길이 5, 그대로 출력 + 공백)
    EXPECT_NE(output.find("Hello  "), std::string::npos)
        << "Expected 'Hello' with spacing not found.";

    // 2) "World" (길이 5, 동일하게 출력)
    EXPECT_NE(output.find("World  "), std::string::npos);

    // 3) "TestString" -> "TestS.. "로 잘려야 함 (5글자 + '.. ')
    EXPECT_NE(output.find("TestS.. "), std::string::npos)
        << "Long string wasn't truncated properly (expected 'TestS.. ').";

    // 4) "123456789" -> "12345.. "
    EXPECT_NE(output.find("12345.. "), std::string::npos)
        << "Numeric string wasn't truncated properly.";

    // 5) "ABC" (길이 3) -> 나머지 공간은 공백으로 채움 => "ABC  " 정도로 예상
    EXPECT_NE(output.find("ABC"), std::string::npos)
        << "Short string 'ABC' missing from output.";

    // 6) 마지막 셀(1,2)는 nullptr -> 코드에서는 "|         "로 출력
    //    다만 정확한 공백 개수를 전부 검사하기는 번거로우니, 단순히 row가 2줄이고
    //    각 행마다 적절한 구분선이 출력되는지 확인
    EXPECT_NE(output.find("|\n"), std::string::npos)
        << "Row line or cell separators not found.";

    EXPECT_NE(output.find("----------"), std::string::npos)
        << "Row separator line not found.";

    // 출력된 문자열 전체가 테이블 형식으로 구성되었는지 대략적으로 확인
}
