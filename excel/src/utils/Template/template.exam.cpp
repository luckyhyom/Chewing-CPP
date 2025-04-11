#include <iostream>
#include <string>

template <typename T>

class Summer {
  T data;

  public:
    Summer(T data);
    T sum(T input);
};

template<>
class Summer<bool> {
  unsigned data;

  public:
    Summer(bool data);
    bool sum(bool input);
};


template <typename T>
Summer<T>::Summer(T data): data(data) {}

template <typename T>
T Summer<T>::sum(T input) {
  return data + input;
}

template <>
Summer<bool>::Summer(bool data): data(data) {}

template <>
bool Summer<bool>::sum(bool input) {
  return data && input;
}

int main() {
  Summer<int> summer1(1);
  Summer<std::string> summer2("My ");
  //Summer<bool> summer3(true);
  std::cout << summer1.sum(1) << std::endl;
  std::cout << summer2.sum("1") << std::endl;
  //std::cout << summer3.sum(true) << std::endl;
  //std::cout << summer3.sum(false) << std::endl;
  return 0;
}