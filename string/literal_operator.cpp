#include <string>
#include <iostream>
using namespace std::literals;

// 정의되어 있는 리터럴 연산자
//std::string operator"" s(const char *str, std::size_t len);

// 사용자 정의 리터럴: _p
std::string operator"" _p(const char* str, std::size_t len) {
  return std::string(str, len) + " 메롱";
}

int main() {
  // "hello"s 는 컴파일 과정에서 operator""s("hello", 5); 로 바뀌게 됩니다.
  auto s1 = "hello"s;
  std::cout << "s1의 길이: " << s1.size() << std::endl;

  auto s2 = "world"_p;
  std::cout << s2 << std::endl;

  // Raw string
  std::string str = R"foo(가나다라마바사
!@#$%^
1234
)";
)foo";
  std::cout << str;
  return 0;
}