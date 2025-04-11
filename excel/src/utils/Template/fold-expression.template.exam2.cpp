#include <iostream>
/**
 * To do
 * 1. 파라미터 팩의 인자를 하나씩 처리하기 ("," 사용)
 * 2. average의 베이스 함수 없애기 (fold expression 사용)
 */
template<typename... Ints>
int diff_from(int start, Ints... nums) {
  return (start - ... - nums);
}

int main() {
  // 100 - 1 - 4 - 2 - 3 - 10
  std::cout << diff_from(100, 1, 4, 2, 3, 10) << std::endl;
  return 0;
}