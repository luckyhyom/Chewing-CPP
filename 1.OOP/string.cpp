#include <iostream>

class string {
    char *str;
    int len;

    public:
    string(char c, int n);  // 문자 c 가 n 개 있는 문자열로 정의
    string(const char *s);
    string(const string &s);
    ~string();

    void add_string(const string &s);   // str 뒤에 s 를 붙인다.
    void copy_string(const string &s);  // str 에 s 를 복사한다.
    int strlen();                       // 문자열 길이 리턴
    void print() {
        std::cout << str << std::endl;
    }
};

string:: string(char c, int n){
    str = new char[n];
    for (int i = 0; i < n; i++)
    {   
        str[i] = c;
    }
}
string:: string(const char *s){
    str = new char[std::strlen(s)];
    copy_string(s);
}
string:: string(const string &s){
    str = new char[s.len + 1];
    copy_string(s);
}
string:: ~string(){
    if(str) delete str;
}
void string:: add_string(const string &s) {
    strcat(str,s.str);
}  
void string:: copy_string(const string &s) {
    strcpy(str,s.str); // 포인터로 받았으면 문자는 *s, 포인터는 s / 레퍼런스로 받았으면 문자는 s, 포인터는 &s ?
    /*
        1. 레퍼런스는 그냥 변수의 별칭이다. 이름만 다른 똑같은 변수.
        2. 문자열 레퍼런스를 매개변수로 정의하는 법
            a. func(const std::string& s)
            b. func(const char s[])
    */
}
int string:: strlen() {
    int count = 0;
    char *pstr = str; 
    while (*pstr)
    {
        count++;
        pstr++; // 멤버변수는 포인터 연산이 가능하네.. 사용하지 않도록 주의할 것
    }
    return count;
}

int main() {
    string *s1 = new string('C', 10);
    std::cout << s1->strlen() << std::endl;
    std::cout << s1->strlen() << std::endl;
    std::cout << s1->strlen() << std::endl;
    s1->print(); // C x 10

    // 레퍼런스 선언하는 법도 모르네, 인자로 넣는 법도 모르네.
    // string &rs1 = s1;

    /*
        레퍼런스는 변수의 별칭임. 변수와 동일하다고 생각하고 다루면 된다.
    */
    string *s2 = new string(*s1);
    s2->add_string(*s1);
    std::cout << s2->strlen() << std::endl;
    s2->print(); // C x 20

    s2->copy_string(*s1);
    s2->print(); // C x 10


    return 0;
}