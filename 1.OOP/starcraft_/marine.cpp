#include <string.h>
#include <iostream>

class Marine {
  int hp;                // 마린 체력
  int coord_x, coord_y;  // 마린 위치
  int damage;            // 공격력
  bool is_dead;
  char* name;  // 마린 이름

 public:
  Marine();                                       // 기본 생성자
  Marine(int x, int y, const char* marine_name);  // 이름까지 지정
  Marine(int x, int y);  // x, y 좌표에 마린 생성
  ~Marine();

  int attack();                       // 데미지를 리턴한다.
  void be_attacked(int damage_earn);  // 입는 데미지
  void move(int x, int y);            // 새로운 위치

  void show_status();  // 상태를 보여준다.
};
Marine::Marine() {
  hp = 50;
  coord_x = coord_y = 0;
  damage = 5;
  is_dead = false;
  name = NULL;
}
Marine::Marine(int x, int y, const char* marine_name) {
  name = new char[strlen(marine_name) + 1];
  strcpy(name, marine_name);

  coord_x = x;
  coord_y = y;
  hp = 50;
  damage = 5;
  is_dead = false;
}
Marine::Marine(int x, int y) {
  coord_x = x;
  coord_y = y;
  hp = 50;
  damage = 5;
  is_dead = false;
  name = NULL;
}
void Marine::move(int x, int y) {
  coord_x = x;
  coord_y = y;
}
int Marine::attack() { return damage; }
void Marine::be_attacked(int damage_earn) {
  hp -= damage_earn;
  if (hp <= 0) is_dead = true;
    Marine m1(1,2,"함수안의 지역 변수 정적 마린");
    Marine *m2 = new Marine(1,2,"함수안의 지역 변수 동적 마린");
}
void Marine::show_status() {
  std::cout << " *** Marine : " << name << " ***" << std::endl;
  std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
            << std::endl;
  std::cout << " HP : " << hp << std::endl;
}
Marine::~Marine() {
  std::cout << name << " 의 소멸자 호출 ! " << std::endl;
  if (name != NULL) {
    delete[] name;
  }
}


/**
    1. malloc은 메모리만 할당하지만 new는 생성자 함수도 함께 실행해준다.
    2. 문자열을 메모리에 동적 할당하기: char *name = new [strlen(new_name) + 1]; strcpy(name, new_name);
    3. 소멸자!
        - 객체의 멤버 변수가 메모리를 동적할당 하는 경우, 소멸될때 해당 멤버변수에 대해서도 메모리를 해제해야한다.
        - 객체의 소멸자 함수는 동적인 경우 꼭 delete로 메모리를 해제해야만 소멸자 함수가 실행된다.
            - 즉, 동적으로 생성한 객체는 메모리를 꼭 해제해주면 된다.

    (참고) 생성자 함수 호출하는 것과 메모리 동적 할당은 다른 개념이다.
        애초에 생성자 함수는 직접 호출하는게 아니라 객체 생성시 자동 호출되는 것이다.!!!! (아나.. new가 자동으로 생성자 함수 호출해준다는 말 때문에 헷갈렸네 ㅡㅡ new가 아니어도 원래 자동으로 호출하는건데)
        다만, new를 이용해서 메모리 동적 할당을 할 경우 생성자 함수가 자동으로 호출되는 것이다.
            1. 메모리 정적 할당, 암묵적 생성자 함수 호출
                Marine m(1,2);

            2. 메모리 정적 할당, 명시적 생성자 함수 호출
                Marine m = Marine(1,2);
                
            3. 메모리 동적 할당, 명시적 생성자 함수 호출
                *Marine m = new Marine(1,2);
 */

int main() {
  Marine* marines[100];

  marines[0] = new Marine(2, 3, "Marine 2");
  marines[1] = new Marine(1, 5, "Marine 1");

  marines[0]->show_status();
  marines[1]->show_status();

  std::cout << std::endl << "마린 1 이 마린 2 를 공격! " << std::endl;

  marines[0]->be_attacked(marines[1]->attack());

  marines[0]->show_status();
  marines[1]->show_status();

  delete marines[0];
  delete marines[1];
}