#include<vector>
#include <iostream>
#include <memory>

/**
 * 부모     부모
 *   나 - 아내
 *     자식
 */

class Member {
private:
  std::vector<std::shared_ptr<Member>> children; 
  std::vector<std::weak_ptr<Member>> parents;
  std::vector<std::weak_ptr<Member>> spouse;

public:
  void AddParent(const std::shared_ptr<Member>& parent);
  void AddSpouse(const std::shared_ptr<Member>& spouse);
  void AddChild(const std::shared_ptr<Member>& child);
};

class FamilyTree {
private:
  std::vector<std::shared_ptr<Member>> entire_family;

public:
  // 두 사람 사이의 촌수를 계산한다.
  int CalculateChon(Member* mem1, Member* mem2);
};


int main() {
  //Member m1; // me
  //Member m2; // wife
  //Member m3; // mother
  //Member m4; // father
  //Member m5; // sun
  //Member m6; // daughter

  // 본체가 있어야하는 것 아닌가? 포인터만 만드는게 아닌가?
  std::shared_ptr<Member> m1 = std::make_shared<Member>();
  std::shared_ptr<Member> m2 = std::make_shared<Member>();

  m1->AddSpouse(m2);
  //m1.AddParent(m3);
  //m1.AddParent(m4);
  //m1.AddChild(m5);
  //m1.AddChild(m6);

  return 0;
}