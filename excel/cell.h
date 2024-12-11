#include "table.h"
#pragma once

class Cell {
    friend Table;
    friend TextTable;
    Table* table;  // 어느 테이블에 속해있는지
    std::string data;
    int x, y;  // 테이블 에서의 위치
    public:
        Cell(const std::string& data) : data(data){};
        int to_numeric();
        std::string stringify();
};