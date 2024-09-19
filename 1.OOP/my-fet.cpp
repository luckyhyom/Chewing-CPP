/*
    1. 동물(struct Animal) 이라는 구조체를 정의해서 이름(char name[30]), 나이(int age),체력(int health), 배부른 정도(int food), 깨끗한 정도의(int clean) 값을 가진다.
    2. 처음에 동물 구조체의 포인터 배열(struct Animal* list[30])을 만들어서 사용자가 동물을 추가할 때 마다 하나씩 생성한다.
    3. play 라는 함수를 만들어서 동물의 상태를 변경하고 show_stat 함수를 만들어서 지정하는 동물의 상태를 출력한다.
    4. 1 턴이 지날 때 마다 동물의 상태를 변경한다.
*/
#include <iostream>

struct Animal {
    char name[30];
    int age;
    int health;
    int food;
    int clean;
};

struct Animal* create_fet(char *name);
void add_fet(struct Animal *new_fet, struct Animal* fet_family[], int &fet_count);
void play(struct Animal* list[], int count);

int main() {
    struct Animal* my_fets[30] = {nullptr};
    int fets_count = 0;
    //for (int i = 0; i < 30; i++) my_fets[i] = new Animal; // 잘못됨, 필요할때 메모리를 할당할 것.
    char first_fet_name[5] = "cuty";
    Animal *my_fet = create_fet(first_fet_name);
    add_fet(my_fet, my_fets, fets_count);
    
    while (1)
    {
        // 매 턴마다 펫의 상태가 업데이트 된다.
        play(my_fets, fets_count);

        int menu = 1;
        std::cout
            << "메뉴를 선택해주세요. \n"
            << "1. 펫 추가 \n"
            << "2. 펫 상태 보기 \n";
        std::cin >> menu;

        if(menu == 1) {
            char name[30];
            std::cout
                << "새로운 펫의 이름을 입력해주세요: \n";
            std::cin >> name;
            struct Animal *new_fet = create_fet(name);
            add_fet(new_fet, my_fets, fets_count);
        } else if(menu == 2) {
            std::cout
                << "상태를 확인하고 싶은 펫의 이름을 입력하세요 : "
                << std::endl;
            for (int i = 0; i < 30; i++)
            {
                if(my_fets[i] == nullptr) continue;
                std::cout
                    << my_fets[i]->name
                    << std::endl;
            }
            char name[30];
            int is_found = 0;
            std::cin >> name;
            for (int i = 0; i < 30; i++)
            {
                if(my_fets[i] == nullptr) continue;
                if(strcmp(name, my_fets[i]->name) != 0) continue;
                std::cout
                    << "이름: " << my_fets[i]->name
                    << std::endl
                    << "포만감: " << my_fets[i]->food
                    << std::endl
                    << "체력: " << my_fets[i]->health
                    << std::endl
                    << "청결: " << my_fets[i]->clean
                    << std::endl;
                is_found = 1;
                break;
            }
            if(!is_found) {
                std::cout << "그런 이름을 가진 펫은 없어요! \n";
            }
        }
    }
    
    return 0;
}

/*
    1. 참조가 아닌 포인터 반환하기
        ❌ struct Animal& create_fet(char *name, int &fets_count)
*/
struct Animal* create_fet(char *name) {
    Animal *new_animal = new Animal; // 이렇게 쓰는게 맞나?
    strcpy(new_animal->name, name);
    new_animal->age = 0;
    new_animal->clean = 3;
    new_animal->food = 3;
    new_animal->health = 3;
    return new_animal;
}

/*
    1. 포인터 배열에 주소값을 할당하기
        ❌ void add_fet(struct Animal &new_fet, struct Animal* (*fet_family)[30], int fet_count) {
            *(*fet_family)[fet_count] = new_fet;
        }
    ✅ why? 참조는.. 내부적으로는 주소를 사용하지만, 사용하는 방법은 일반 변수와 똑같다. 참조는 주소를 가지고 있지만, 참조를 대입하면 참조가 가리키는 값을 대입하게 된다.
 */
void add_fet(struct Animal *new_fet, struct Animal *fet_family[], int &fet_count) {
    fet_family[fet_count] = new_fet;
    fet_count++;
}

/*
    포인터를 다루는게 너무나 미숙하다..

    아래 문제점들이 개선되었다.
    1. struct Animal* (*fet_family)[30]에서 30을 쓰지 않는 방법?
    2. struct Animal* (*fet_family)[30] 매개변수를 좀더 다루기 쉽게 선언하는법?
    3. 이게 왜 안될까?
        *((*fet_family)[i])->clean -= 1;
*/
void play(struct Animal* list[], int count) {
    for (int i = 0; i < count; i++) {
        std::cout << list[i]->name;
        if (list[i] == nullptr || strlen(list[i]->name) == 0) continue;
        list[i]->health -= 1;
        list[i]->food -= 1;
        list[i]->clean -= 1;
        std::cout << list[i]->health;
    }
}