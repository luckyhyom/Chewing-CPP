#include <iostream>
#include <map>

/**
 * 1. 없는 키를 참조하면 값을 초기화한다. (find를 사용할 것)
 * 2. 키는 고유하다. (덮어 씌울 때 insert말고 [] 연산자 사용할 것)
 */
template<typename K, typename V>
void print(const std::map<K,V>& m) {
  for (const auto& kv : m) {
    std::cout << kv.first << " " << kv.second << std::endl;
  }
}

int main() {
  std::map<std::string, double> my_map;
  my_map["Minji"] = 169;
  my_map["Hani"] = 162;
  print(my_map);

  std::cout << "해린의 키는? " << my_map["Haerin"] << std::endl;
}