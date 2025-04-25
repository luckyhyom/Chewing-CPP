#include <string>
#include <iostream>

using namespace std;

/**
 * 1. i를 순회하면서 UTF-8 첫 바이트를 감지하고
 * 2. 그 크기만큼 substr로 잘라내고
 * 3. 터미널이 UTF-8로 해석하게 하는 방식
 */
int main() {
  std::string str = u8"이건 UTF-8 문자열 입니다";
  cout << str[1] << endl; // 한글은 3바이트 단위로 나누어야하는데, string은 char 단위로 원소가 저장됨
  size_t i = 0;
  size_t len = 0;

  while (i < str.size()) {
    int char_size = 0;

    // 4 바이트로 인코딩되는 문자들은 첫 번째 바이트가 11110xxx 꼴입니다. 
    if ((str[i] & 0b11111000) == 0b11110000) {
      char_size = 4;
    } else if ((str[i] & 0b11110000) == 0b11100000) {
      char_size = 3;
    } else if ((str[i] & 0b11100000) == 0b11000000) {
      char_size = 2;
    } else if ((str[i] & 0b10000000) == 0b00000000) {
      char_size = 1;
    } else {
      std::cout << "이상한 문자 발견!" << std::endl;
      char_size = 1;
    }

    std::cout << str.substr(i, char_size) << std::endl;

    i += char_size;
    len++;
  }
  std::cout << "문자열의 실제 길이 : " << len << std::endl;
  return 0;
}