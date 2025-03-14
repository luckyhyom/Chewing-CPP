#include "NumberCell.h"

NumberCell::~NumberCell() {
  delete static_cast<const int*>(this->data);
}

int NumberCell::to_numeric() {
  const int* value = static_cast<const int*>(this->data);
  return *value;
};

std::string NumberCell::stringify() {
  const int* value = static_cast<const int*>(this->data);
  return std::to_string(*value);
};