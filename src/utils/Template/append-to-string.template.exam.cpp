#include <iostream>
#include <string>

size_t GetStringSize(const char* s) {
  return std::strlen(s);  // 문자열 길이를 정확히 계산
}

size_t GetStringSize(const std::string& s) {
  return s.size();
}


/**
 * 오버로딩된 템플릿 함수
 * 1. 기본 함수
 * 2. 재귀 종료 함수
 * 3. 단일 인자 처리 함수
 * 
 * 반환 타입을 void로 하면 불필요한 복사를 막는다.
 */
template<typename String>
void AppendToString(std::string& concat_str, const String& s) {
  concat_str.append(s);
}

/**
 * 1. 재귀 호출 함수
 * 2. 가변 인자 처리 함수
 */
template<typename String, typename... Strings>
void AppendToString(std::string& concat_str, const String& s, const Strings&... strs) {
  /**
   * 단일 인자 처리 함수를 한번만 호출하고 있으므로
   * 가변 인자 처리 함수에서도 실질적인 기능, 즉 append를 해줘야함.
   */
  concat_str.append(s);
  AppendToString(concat_str, strs...);  // 재귀적으로 호출
}


template<typename String, typename... Strings>
std::string StrCat(const String& s, Strings... strs) {
  size_t str_size;
  str_size = GetStringSize(s, strs...);

  std::string concat_str;
  concat_str.reserve(str_size);

  AppendToString(concat_str, s, strs...);
  return concat_str;
}

int main () {
  // std::string 과 const char* 을 혼합해서 사용 가능하다.
  std::cout << StrCat(std::string("this"), " ", "is", " ", std::string("a"),
                      " ", std::string("sentence"));
  return 0;
}