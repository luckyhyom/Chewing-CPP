// 문자열 비교 알고리즘 수정하기
#include <string>
#include <iostream>

/**
 * 문제는, cpp는 알고리즘을 수정하기 위해서 어느 메서드를 수정해야하는지 파악하기가 비교적 쉽지 않은 것 같음
 */
struct my_char_traits : public std::char_traits<char> {
    static int get_real_rank(char c) {
      if (isdigit(c)) {
        return c + 256;
      }
      return c;
    }

    static bool lt(char __c1, char __c2) {
      return get_real_rank(__c1) < get_real_rank(__c2);
    }

    static int compare(const char* s1, const char* s2, size_t n) {
      while (n-- != 0) {
        if (get_real_rank(*s1) < get_real_rank(*s2)) {
          return -1;
        }
        if (get_real_rank(*s1) > get_real_rank(*s2)) {
          return 1;
        }
        ++s1;
        ++s2;
      }
      return 0;
    }
};

void* operator new(size_t count) {
  std::cout << count << "byte 할당" << std::endl;
  return malloc(count);
}

int main() {
  std::string a = "1a";
  std::string b = "a1";
  std::cout << "우선 순위가 낮은 문자열: "  << std::boolalpha << (a < b) << std::endl;
  std::cout << "우선 순위가 낮은 문자열: "  << std::boolalpha << (b < a) << std::endl;

  std::basic_string<char, my_char_traits> c = "1a";
  std::basic_string<char, my_char_traits> d = "a1";
  std::cout << "우선 순위가 낮은 문자열: "  << std::boolalpha << (c < d) << std::endl;
  std::cout << "우선 순위가 낮은 문자열: "  << std::boolalpha << (d < c) << std::endl;
}