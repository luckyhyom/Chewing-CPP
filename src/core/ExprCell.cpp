#include "ExprCell.h"

int ExprCell::to_numeric() {
  // 1. Tokenize
  // 2. Shunting Yard Algorithm
  return 777;
};

std::string ExprCell::stringify() {
  const std::string* value = static_cast<const std::string*>(this->data);
  return *value;
};