#include "DateCell.h"
#include <sstream>
#include <iomanip>

DateCell::~DateCell() {
  delete static_cast<const time_t*>(this->data);
}

int DateCell::to_numeric() {
  const time_t* value = static_cast<const time_t*>(this->data);
  return static_cast<int>(*value);
};

/**
 * # std::tm
 * 1. std::tm의 설계상 항상 연도에서 1900을 빼서 저장한다.
 * 2. 월은 0에서부터 시작한다.
 * 
 * # 시간 데이터에 사용되는 자료형
 * 1. 메모리 효율을 위해 time_t 자료형으로 시간 데이터를 저장한다.
 * 2. 사람이 읽기 쉬운 형태로 시간을 표현하기 위해 std::tm 구조체를 사용한다.
 * 
 * # stream의 결합 순서와 평가 순서
 * 1. << 연산자 사이의 연산의 평가 순서는 정해져 있지 않다.
 * 2. << 연산자의 결합 순서는 왼쪽에서 오른쪽으로 정해져 있다.
 */
std::string DateCell::stringify() {
  std::ostringstream oss;
  const time_t* value = static_cast<const time_t*>(this->data);
  std::tm* newTimeStruct = std::localtime(value);
  if (newTimeStruct) {
      oss
        << (newTimeStruct->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << newTimeStruct->tm_mon + 1  << "-"
        << std::setw(2) << std::setfill('0') << newTimeStruct->tm_mday;
  }
  return oss.str();
};