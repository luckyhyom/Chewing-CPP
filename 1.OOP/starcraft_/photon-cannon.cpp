// 복사 생성자의 중요성
#include <string.h>
#include <iostream>

class Photon_Cannon {
  int hp, shield;
  int coord_x, coord_y;
  int damage;

  char *name;

 public:
  Photon_Cannon(int x, int y);
  Photon_Cannon(int x, int y, const char *cannon_name);
  Photon_Cannon(const Photon_Cannon &pc);
  ~Photon_Cannon();

  void show_status();
};

Photon_Cannon::Photon_Cannon(int x, int y) {
  hp = shield = 100;
  coord_x = x;
  coord_y = y;
  damage = 20;

  name = NULL;
}


/*
    1. const 키워드를 사용하여 매개변수를 사용은 하되 수정은하지 못하도록 지켜주자
        C언어는 주소가 가리키는 데이터를 수정 못하게 한다.
        javascript는 변수의 주소만 수정을 못하고 데이터는 수정 가능하다.
*/
Photon_Cannon::Photon_Cannon(const Photon_Cannon &pc) {
  std::cout << "복사 생성자 호출! " << std::endl;
  hp = pc.hp;
  shield = pc.shield;
  coord_x = pc.coord_x;
  coord_y = pc.coord_y;
  damage = pc.damage;

  name = new char[strlen(pc.name) + 1];
  strcpy(name, pc.name);
}
Photon_Cannon::Photon_Cannon(int x, int y, const char *cannon_name) {
  hp = shield = 100;
  coord_x = x;
  coord_y = y;
  damage = 20;

  name = new char[strlen(cannon_name) + 1];
  strcpy(name, cannon_name);
}
Photon_Cannon::~Photon_Cannon() {
  if (name) delete[] name;
}
void Photon_Cannon::show_status() {
  std::cout << "Photon Cannon :: " << name << std::endl;
  std::cout << " Location : ( " << coord_x << " , " << coord_y << " ) "
            << std::endl;
  std::cout << " HP : " << hp << std::endl;
}
int main() {
  Photon_Cannon pc1(3, 3, "Cannon");
  Photon_Cannon pc2 = pc1;  // 오버로딩된 복사 생성자가 호출된다.
  Photon_Cannon *pc3 = new Photon_Cannon(pc1);  // 오버로딩된 복사 생성자가 호출된다.

    /*
        복사 생성자는 오직 객체 생성시에만 호출됨.
        데이터 복사는 이루어지지만, 복사생성자 안에 실행해야하는 함수같은 것이 있다면..
    */
    //  Photon_Cannon pc4(1,2,"GOOD");
    //  pc4 = pc1; // 복사 생성자는 호출되지 않는다, 웬 에러도 발생한다.

  pc1.show_status();
  pc2.show_status();
  pc3->show_status();
  pc4.show_status();

}