#include <iostream>

template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << std::endl;  // fold expression으로 재귀 없이 처리
}

int main() {
  print(1,2.5,"Good");
  return 0;
}