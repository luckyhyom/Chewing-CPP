#include "DateCell.h"
#include <sstream>

int DateCell::to_numeric() {
  const time_t* value = static_cast<const time_t*>(this->data);
  return static_cast<int>(*value);
};

std::string DateCell::stringify() {
  std::ostringstream oss;
  const time_t* value = static_cast<const time_t*>(this->data);
  // time_t를 다시 날짜로 변환 (현지 시간대 기준)
  std::tm* newTimeStruct = std::localtime(value);
  if (newTimeStruct) {
      oss
        << (newTimeStruct->tm_year + 1900) << "-"
        << (newTimeStruct->tm_mon + 1 < 10 ? "0" : "") << (newTimeStruct->tm_mon + 1) << "-"
        << (newTimeStruct->tm_mday < 10 ? "0" : "") << newTimeStruct->tm_mday;
  }

  return oss.str(); // date는.. 포맷은 문자열인데.. 타입은 longint라고하니 어떻게 선언하지
};