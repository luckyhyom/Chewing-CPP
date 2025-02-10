#include <iostream>
#include <string>

template <typename T>
struct Compare {
  bool operator()(const T& a, const T& b) const { return a > b; }
};

template <typename T, typename Comp = Compare<T> >
T max(T& a, T& b) {
  Comp comp;
  return Comp(a, b) ? a : b;
}

int main() {
  int a = 1, b = 2;
  std::cout << "Max (" << a << "," << b << ") ? : " << max(a, b) << std::endl;
  std:: string s = "hello", t = "world";
  std::cout << "Max (" << s << "," << t << ") ? : " << max(s, t) << std::endl;
}