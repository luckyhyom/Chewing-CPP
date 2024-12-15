#include <gtest/gtest.h>
#include "HTMLTable.h"
#include "core/Cell.h"

/*
 * 가정:
 * - HTMLTable : Table를 상속
 * - Table은 row, col 멤버, data_base 멤버(Cell***), reg_cell(Cell*, int row, int col) 메서드
 * - Cell::stringify()는 셀에 저장된 문자열을 반환
 * - HTMLTable(int row, int col)
 * - HTMLTable::~HTMLTable()
 * - std::string HTMLTable::print_table()
 */

TEST(HTMLTableTest, ConstructorDestructor)
{
    // 생성 및 소멸자가 문제 없이 동작하는지 테스트
    {
        HTMLTable table(2, 2);
        // 소멸자 호출 시 "HTML Table Done" 메시지 출력
        // 별다른 예외나 충돌 없이 정상 동작 여부만 확인
    }
    SUCCEED() << "Constructor and destructor worked without crashing.";
}

TEST(HTMLTableTest, PrintEmptyTable)
{
    // 어떤 셀도 등록하지 않은 빈 테이블을 HTML 형태로 출력해본다
    HTMLTable table(2, 3);
    std::string output = table.print_table();

    EXPECT_FALSE(output.empty())
        << "Empty table HTML output should not be empty.";

    // 최소한의 구조 확인
    // <div> ... </div> 로 감싸져 있는지
    EXPECT_NE(output.find("<div>"), std::string::npos);
    EXPECT_NE(output.find("</div>"), std::string::npos);

    // 행 2, 열 3 => 중첩 div 구조를 예상:
    // <div>
    //    <div>
    //        <div></div>   // (0,0)
    //        <div></div>   // (0,1)
    //        <div></div>   // (0,2)
    //    </div>
    //    <div>
    //        <div></div>   // (1,0)
    //        <div></div>   // (1,1)
    //        <div></div>   // (1,2)
    //    </div>
    // </div>
    // 모든 셀이 nullptr 이므로 닫는 태그만 들어 있음
    // 여기서는 전체 구조를 완벽하게 비교하는 대신,
    // div 갯수가 어느정도 맞는지 또는 검색을 통해 부분적으로 확인

    // div 태그가 최소한 1개 이상 존재하는지 등 간단히 체크
    // 아래는 적절한 수치로 대략 확인 (행*열 + 감싸는 div들)
    size_t divCount = 0;
    size_t pos = 0;
    while ((pos = output.find("<div>", pos)) != std::string::npos)
    {
        divCount++;
        pos += 5; // len("<div>")
    }
    // row=2, col=3일 때 대략 1개의 최상위 <div> + 2개의 행 div + 6개의 셀 div = 9개
    EXPECT_GE(divCount, 9u) << "Expected at least 9 <div> tags for a 2x3 table structure.";
}

TEST(HTMLTableTest, PrintTableWithData)
{
    // 2x2 테이블에 간단히 셀 몇 개 등록
    HTMLTable table(2, 2);
    table.reg_cell(new Cell("Hello"), 0, 0);
    table.reg_cell(new Cell("HTMLTest"), 0, 1);
    table.reg_cell(new Cell("123"), 1, 0);
    // (1,1)은 비워둠

    std::string output = table.print_table();
    std::cout << "[DEBUG] HTML output:\n"
              << output << std::endl;

    EXPECT_FALSE(output.empty()) << "HTML output is empty, unexpected.";

    // 간단한 HTML 구조 점검
    EXPECT_NE(output.find("<div>"), std::string::npos);
    EXPECT_NE(output.find("</div>"), std::string::npos);

    // 등록한 셀들의 문자열이 들어있는지
    EXPECT_NE(output.find("Hello"), std::string::npos)
        << "HTML should contain the string 'Hello' for (0,0).";
    EXPECT_NE(output.find("HTMLTest"), std::string::npos)
        << "HTML should contain the string 'HTMLTest' for (0,1).";
    EXPECT_NE(output.find("123"), std::string::npos)
        << "HTML should contain '123' for (1,0).";

    // 비어있는 셀(1,1)은 <div></div> 형태 (아무 내용 없음)
    // 부정확하게나마 검색
    // row=1, col=1 => 2행째에 마지막 셀이 '빈 div'로 출력
    // 굳이 엄밀히는 안 해도 되지만 참고로 추가 점검 가능
    // 여기서는 그냥 "table.print_table()" 출력에 문제 없는지만 확인

    // 마지막으로 div 태그가 일정 개수 이상 있는지 대략 점검
    size_t divCount = 0;
    size_t pos = 0;
    while ((pos = output.find("<div>", pos)) != std::string::npos)
    {
        divCount++;
        pos += 5; // move past "<div>"
    }
    EXPECT_GE(divCount, 6u) << "We expect at least 6 <div> tags for a 2x2 table (1 outer + 2 row + 2 cell + etc).";
}