#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s) {
  std::cout << "[";
  for (typename std::set<T>::iterator itr = s.begin(); itr != s.end(); ++itr) {
    std::cout << *itr << " ";
  }
  std::cout << "]" << std::endl;
}

int main() {
  std::set<int> s;
  s.insert(10);
  s.insert(50);
  s.insert(20);
  s.insert(40);

  std::cout << "it is sorted" << std::endl;
  print_set(s);

  std::cout << "is '20' element of s?: ";
  auto itr = s.find(20);
  if(itr != s.end()) {
    std::cout << "Yes" << std::endl;
  } else{
    std::cout << "No" << std::endl;
  }
}
