#include <iostream>

StringCell::~StringCell() {
  delete static_cast<const std::string*>(this->data);
}

int StringCell::to_numeric() {
  return 0;
};

std::string StringCell::stringify() {
  const std::string* value = static_cast<const std::string*>(this->data);
  return *value;
};