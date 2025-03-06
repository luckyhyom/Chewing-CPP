#include <iostream>

template <int N>
struct INT {
  static const int num = N; // 템플릿은 컴파일시 매번 새로 만들어짐
};

// a::num이 가능한건가?
template <typename a, typename b>
struct add {
  typedef INT<a::num + b::num> result;
};

int main() {
  using a = INT<1>;
  INT<2> b;

  std::cout << a::num << std::endl;
  std::cout << b.num << std::endl;
}
