#include <iostream>
#include <sstream>
#include <limits>
#include <regex>
#include "TextTable.h"
#include "HTMLTable.h"
#include "Table.h"
#include "StringCell.h"
#include "NumberCell.h"
#include "ExprCell.h"
#include "DateCell.h"

bool isValidCellReference(const std::string& input);
std::string identify_data_type(const std::string& value);

int main()
{
    std::cout << "Program started" << std::endl; // 디버깅 메시지
    TextTable table(8, 8);
    std::string* example = new std::string("Hello World");
    table.reg_cell(new StringCell(example), "B2");

    while (1)
    {
        std::cout << table << std::endl;

        std::string location;
        std::string value;

        std::cout << "Please enter the cell location (e.g., A1, C5) where you would like to input the data. : \n";
        if (!(std::cin >> location) || !isValidCellReference(location))
        {
            std::cerr << "Invalid cell location. Please enter a valid cell reference (e.g., A1, C5)." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "input value: ";
        if (!(std::cin >> value))
        {
            std::cerr << "Invalid input for value. Please enter a valid string." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
      
        // value를 new로 초기화 하지 않으면.. 입력 받은 값은 이 함수의 지역 변수로서 소멸되기때문에 의미없는 값을 가리키는 포인터만 남는다.
        Cell* cell;
        std::string data_type = identify_data_type(value);
        if(data_type == "int") {
          std::istringstream iss(value);
          int intValue;
          iss >> intValue;
          int* cell_data = new int(intValue);
          cell = new NumberCell(cell_data);
        } else if(data_type == "date") {
          std::istringstream iss(value);
          time_t intValue;
          iss >> intValue;
          time_t* cell_data = new time_t(intValue);
          cell = new DateCell(cell_data);
        } else {
          cell = new StringCell(new std::string(value));
        }
        table.reg_cell(cell, location);
    }

    return 0;
}

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