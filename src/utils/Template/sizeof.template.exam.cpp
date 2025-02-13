#include <iostream>

// 재귀 호출 종료를 위한 함수
int sum_all(int num = 0) {
  return num;
}

template <typename... Ints>
int sum_all(int num, Ints... ints) {
  return num + sum_all(ints...);
}

template <typename... Ints>
double average(Ints... ints) {
  return static_cast<double>(sum_all(ints...)) / sizeof...(ints);
}

int main() {
  // (1 + 4 + 2 + 3 + 10) / 5
  std::cout << average(1, 4, 2, 3, 10) << std::endl;
}
