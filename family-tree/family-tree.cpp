#include<vector>
#include <iostream>
#include <memory>

/**
 * 부모     부모
 *   나 - 아내
 *     자식
 */

class Member : public std::enable_shared_from_this<Member> {
private:
  std::vector<std::shared_ptr<Member>> children; 
  std::vector<std::weak_ptr<Member>> parents;
  std::vector<std::weak_ptr<Member>> spouse;

public:
  void AddParent(const std::shared_ptr<Member>& parent);
  void AddSpouse(const std::shared_ptr<Member>& spouse);
  void AddChild(const std::shared_ptr<Member>& child);
};

void Member::AddParent(const std::shared_ptr<Member>& parent) {
  this->parents.emplace_back(parent);
  parent->AddChild(shared_from_this());
}
void Member::AddSpouse(const std::shared_ptr<Member>& spouse) {
  for (auto& wk : this->spouse) {
    std::shared_ptr<Member> a = wk.lock();
    if(a && a.get() == spouse.get()) {
      return;
    }
  }

  this->spouse.emplace_back(spouse);
  spouse->AddSpouse(shared_from_this());
}
void Member::AddChild(const std::shared_ptr<Member>& child) {
  this->children.emplace_back(child);
  child->AddParent(shared_from_this());
}

class FamilyTree {
private:
  std::vector<std::shared_ptr<Member>> entire_family;

public:
  // 두 사람 사이의 촌수를 계산한다.
  int CalculateChon(Member* mem1, Member* mem2);
};

int main() {
  std::shared_ptr<Member> m1 = std::make_shared<Member>();
  std::shared_ptr<Member> m2 = std::make_shared<Member>();
  std::shared_ptr<Member> m3 = std::make_shared<Member>();
  std::shared_ptr<Member> m4 = std::make_shared<Member>();
  std::shared_ptr<Member> m5 = std::make_shared<Member>();
  std::shared_ptr<Member> m6 = std::make_shared<Member>();
  m1->AddSpouse(m2);
  m1->AddParent(m3);
  m1->AddParent(m4);
  m1->AddChild(m5);
  m1->AddChild(m6);

  return 0;
}