#include <gtest/gtest.h>
#include "StringCell.h"
#include "NumberCell.h"
#include "ExprCell.h"
#include "DateCell.h"

/*
 * Cell 클래스 구조:
 *
 * Cell(const std::string &data)
 * int to_numeric()
 * std::string stringify()
 *
 * 현재 구현에서 to_numeric()은 항상 0을 반환.
 * stringify()는 생성자에 전달된 data를 그대로 반환.
 */

TEST(CellTest, ConstructorAndStringify)
{
    // 생성 시 문자열을 넣어두면, stringify()를 통해 동일한 문자열을 얻을 수 있어야 함
    std::string* value1 = new std::string("Hello");
    StringCell c1(value1);
    EXPECT_EQ(c1.stringify(), "Hello");

    int* value2 = new int(12345);
    NumberCell c2(value2);
    EXPECT_EQ(c2.stringify(), "12345");

    std::string* value3 = new std::string("");
    StringCell c3(value3);
    EXPECT_EQ(c3.stringify(), "");
}

TEST(CellTest, ToNumericDefault)
{
    // 현재 구현으로는 항상 0을 반환
    std::string* value = new std::string("Hello");
    StringCell c(value);
    EXPECT_EQ(c.to_numeric(), 0) << "Default to_numeric() should return 0.";
}
