#include <string>
#include <iostream>

/**
 * 동적 메모리 할당은 시간이 오래 걸린다.
 * 짧은 문자열의 경우 동적할당 하지 않고 그냥 string 객체에 저장한다.
 * 
 */
void* operator new(size_t count) {
  std::cout << count << "byte 할당" << std::endl;
  return malloc(count);
}

int main() {
  std::string a = "i love njz, yerin baek! i want to be good at playing bass guitar!";
  std::cout << "size of a: " << sizeof(a) << std::endl;
  std::cout << "a ptr: " << static_cast<const void*>(&a) << std::endl;
  std::cout << "a data ptr: " << static_cast<const void*>(a.data()) << std::endl;

  std::string b = "this is short string.";
  std::cout << "size of b: " << sizeof(b) << std::endl;
  std::cout << "b ptr: " << static_cast<const void*>(&b) << std::endl;
  std::cout << "b data ptr: " << static_cast<const void*>(b.data()) << std::endl;

  return 0;
}