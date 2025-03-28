/**
 * 내가 만든 클래스를 원소로 넣기
 * 
 */

#include <iostream>
#include <unordered_set>

template<typename K>
void printHashSet(const std::unordered_set<K>& m) {
  for (const auto& elem : m) {
    std::cout << elem << std::endl;
  }
}

// 무엇을 기준으로 값의 유무를 판단하는거지? 
// 그냥 set<Todo>.find()
template<typename K>
bool is_exist(const std::unordered_set<K>& m, K key) {
  auto itr = m.find(key);
  if(itr != m.end()) {
    std::cout << key << "가 존재!" << std::endl;
  } else {
    std::cout << key << "가 존재하지 않음!" << std::endl;
  }
  return true;
}

class Todo {
  int priority;
  std::string job_desc;

  public:
    Todo(int priority, std::string job_desc)
      : priority(priority), job_desc(job_desc) {}
  friend std::ostream& operator<<(std::ostream& os, const Todo& todo);
};






int main() {

}