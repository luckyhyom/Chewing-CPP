#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <regex>
#include "TextTable.h"
#include "HTMLTable.h"
#include "Table.h"
#include "StringCell.h"
#include "NumberCell.h"
#include "ExprCell.h"
#include "DateCell.h"
#include "utils.h"

bool isValidCellReference(const std::string& input);
std::string identify_data_type(const std::string& value);
time_t* convertToTimeT(const std::string& value);

int main() {
  std::cout
      << "테이블 (타입) (최대 행 크기) (최대 열 크기) 를 순서대로 입력해주세요"
      << std::endl;
  std::cout << "* 참고 * " << std::endl;
  std::cout << "1 : 텍스트 테이블, 2 : CSV 테이블, 3 : HTML 테이블"
            << std::endl;

  int type, max_row, max_col;
  std::cin >> type >> max_row >> max_col;
  MyExcel::Excel m(max_row, max_col, type - 1);
  m.command_line();
}


//// 생략
//int main() {
//  MyExcel::TxtTable table(5, 5);
//  table.reg_cell(new MyExcel::NumberCell(2, 1, 1, &table), 1, 1);
//  table.reg_cell(new MyExcel::NumberCell(3, 1, 2, &table), 1, 2);

//  table.reg_cell(new MyExcel::NumberCell(4, 2, 1, &table), 2, 1);
//  table.reg_cell(new MyExcel::NumberCell(5, 2, 2, &table), 2, 2);
//  table.reg_cell(new MyExcel::ExprCell("B2+B3*(C2+C3-2)", 3, 3, &table), 3, 2);
//  table.reg_cell(new MyExcel::StringCell("B2 + B3 * ( C2 + C3 - 2 ) = ", 3, 2, &table), 3, 1);

//  std::cout << table;
//}


//int main() {
//  MyExcel::TxtTable table(5, 5);
//  std::ofstream out("test.txt");

//  table.reg_cell(new MyExcel::Cell("Hello~", 0, 0, &table), 0, 0);
//  table.reg_cell(new MyExcel::Cell("C++", 0, 1, &table), 0, 1);

//  table.reg_cell(new MyExcel::Cell("Programming", 1, 1, &table), 1, 1);
//  std::cout << std::endl << table;
//  out << table;
//}

//int main()
//{
//    std::cout << "Program started" << std::endl; // 디버깅 메시지
//    TextTable table(8, 8);
//    std::string* example = new std::string("Hello World");
//    table.reg_cell(new StringCell(example), "B2");

//    while (1)
//    {
//        std::cout << table << std::endl;

//        std::string location;
//        std::string value;

//        std::cout << "Please enter the cell location (e.g., A1, C5) where you would like to input the data. : \n";
//        if (!(std::cin >> location) || !isValidCellReference(location))
//        {
//            std::cerr << "Invalid cell location. Please enter a valid cell reference (e.g., A1, C5)." << std::endl;
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            continue;
//        }

//        std::cout << "input value: ";
//        if (!(std::cin >> value))
//        {
//            std::cerr << "Invalid input for value. Please enter a valid string." << std::endl;
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//            continue;
//        }
      
//        // value를 new로 초기화 하지 않으면.. 입력 받은 값은 이 함수의 지역 변수로서 소멸되기때문에 의미없는 값을 가리키는 포인터만 남는다.
//        Cell* cell;
//        std::string data_type = identify_data_type(value);
//        if(data_type == "int") {
//          std::istringstream iss(value);
//          int intValue;
//          iss >> intValue;
//          int* cell_data = new int(intValue);
//          cell = new NumberCell(cell_data);
//        } else if(data_type == "date") {
//          cell = new DateCell(convertToTimeT(value));
//        } else {
//          cell = new StringCell(new std::string(value));
//        }
//        table.reg_cell(cell, location);
//    }

//    return 0;
//}

bool isValidCellReference(const std::string& input) {
    // ^[A-Za-z]+[0-9]+$:
    // ^          : 문자열의 시작
    // [A-Za-z]+  : 하나 이상의 알파벳 문자
    // [0-9]+     : 하나 이상의 숫자
    // $          : 문자열의 끝
    std::regex cellRegex("^[A-Za-z]+[0-9]+$");
    return std::regex_match(input, cellRegex);
}

// 함수: 입력값의 자료형 식별
std::string identify_data_type(const std::string& value) {
    // 1. 정수 검사
    std::istringstream iss(value);
    int intValue;
    if (iss >> intValue && iss.eof()) { // 입력값이 정수 변환에 성공하고 추가 문자가 없는지 확인
        return "int";
    }

    // 2. 날짜 검사 (YYYY-MM-DD 형식)
    std::regex date_regex(R"(\d{4}-\d{2}-\d{2})"); // 정규식: 연도-월-일
    if (std::regex_match(value, date_regex)) {
        return "date";
    }

    // 3. 그 외는 문자열로 간주
    return "string";
}

time_t* convertToTimeT(const std::string& value) {
    std::tm timeStruct = {};
    std::istringstream iss(value);
    char delimiter; // For the '-' character
    int year, month, day;

    // Parse the input string
    iss >> year >> delimiter >> month >> delimiter >> day;
    if (iss.fail() || delimiter != '-') {
        throw std::invalid_argument("Invalid date format");
    }

    // Fill the tm structure
    timeStruct.tm_year = year - 1900; // tm_year starts from 1900
    timeStruct.tm_mon = month - 1;    // tm_mon is 0-based
    timeStruct.tm_mday = day;

    // Convert to time_t
    time_t timeValue = std::mktime(&timeStruct);
    if (timeValue == -1) {
        throw std::runtime_error("Failed to convert date to time_t");
    }

    // Dynamically allocate memory for time_t
    return new time_t(timeValue);
}

