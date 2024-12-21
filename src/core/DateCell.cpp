#include "DateCell.h"

int DateCell::to_numeric() {
  const time_t* value = static_cast<const time_t*>(this->data);
  return static_cast<int>(*value);
};

std::string DateCell::stringify() {
  const time_t* value = static_cast<const time_t*>(this->data);
  return std::to_string(*value); // date는.. 포맷은 문자열인데.. 타입은 longint라고하니 어떻게 선언하지
};