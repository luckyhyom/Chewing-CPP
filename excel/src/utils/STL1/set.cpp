#include <iostream>
#include <set>

//template <typename T>
//void print_set(std::set<T>& s) {
//  std::cout << "[";
//  for (typename std::set<T>::iterator itr = s.begin(); itr != s.end(); ++itr) {
//    std::cout << *itr << " ";
//  }
//  std::cout << "]" << std::endl;
//}

template <typename T>
void print_set(std::set<T>& s) {
  for (const auto& elem : s) {
    std::cout << elem << " " << std::endl;
  }
}

class Todo {
  int priority;
  std::string desc;

  public:
    Todo(int priority, std::string desc) : priority(priority), desc(desc) {}

    // 📌 const 키워드를 사용해야한다.
    bool operator<(const Todo& t) const {
      if(t.priority == priority) {
        return desc < t.desc;
      }
      return priority > t.priority;
    }

    // 비멤버 함수이지만 `friend`로 선언 → private 멤버 접근 가능
    friend std::ostream& operator<<(std::ostream& o, const Todo& td);
};

std::ostream& operator<<(std::ostream& o, const Todo& td);

int main() {
  std::set<Todo> todos;

  todos.insert(Todo(3, "베이스 연주하기"));
  todos.insert(Todo(2, "카페 가기"));
  todos.insert(Todo(3, "qwer 자컨 시청하기"));
  todos.insert(Todo(1, "운동 하기"));
  print_set(todos);
}
