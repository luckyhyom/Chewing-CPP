#include "Cell.h"
#include <ctime>

class NumberCell : public Cell {
  /**
   * # 멤버 변수를 overriding?
   * 1. 동일한 이름으로 멤버 변수 만들기
   * 2. 다른 이름으로 멤버 변수 만들기 (같은 데이터라면 쓸모 없음)
   * 3. 파생 클래스에서 사용할때 static_cast 하기 (쓸모 있을까)
   */
  //int *data;
  public:
    NumberCell(const int *data) : Cell(data) {}; // new로 초기화된 인자만 받을 수 있도록 포인터를 사용한다.
    int to_numeric();
    std::string stringify();
    ~NumberCell();
};


/**
 * 자료형이 동적인 멤버 변수 (입력 받은 데이터를 자료형을 구분하여 저장)
 * 1. Cell 클래스의 data 멤버 변수를 자료형 void* 로 선언한다.
 * 2. 파생 클래스는 생성자의 매개변수에 자료형을 명시한다. (StringCell에 int data를 저장할 수 없도록)
 * 3. 파생 클래스는 data를 원하는 자료형으로 캐스팅해서 반환한다.
 * 4. data가 가리키는 데이터가 어딘가에서는 한번 초기화 되어야 한다.
 * 이처럼 하려는 이유는, data라는 멤버 변수 초기화를 강제하면서도 각 파생된 class에서 data를 사용할수 있도록 하기 위해서다.
 * 
 * 캐스팅하는 것이 아직 미숙하다.
 */