/*
    1. 문자(char) 로 부터의 문자열 생성, C 문자열 (char *) 로 부터의 생성
    2. 문자열 길이를 구하는 함수
    3. 문자열 뒤에 다른 문자열 붙이기
    4. 문자열 내에 포함되어 있는 문자열 구하기
    5. 문자열이 같은지 비교
    6. 문자열 크기 비교 (사전 순)
 */

#include <iostream>

class String {
    char *str; // 동적인 크기의 데이터는 포인터를 사용해야한다. / 소멸자 필수!
    int length;

    public:
        String(char c);
        String(char *string);
        ~String();
        int get_length() const;
        void concat(char *string);
        int find(char *string);
        int compare_str(char *string);
        int compare_size(char *string);
        void print() const;
};

String::String(char c) {
    str = new char[1];
    *str = c; // 동적 할당도 초기화 리스트 사용 가능?
    length = 1;

}

String::String(char *string) {
    str = new char[strlen(string) + 1];
    strcpy(str, string);
    length = strlen(string);
}

String::~String() {
    delete str[];
}

int String::get_length() const {
    return length;
}

void String::concat(char *string) {
    int new_length = strlen(str) + strlen(string);
    char *new_str = new char[new_length + 1];
    char *p_new_str = new_str;
    char *old_str = str;

    while (*old_str)
    {
        *p_new_str = *old_str;
        p_new_str++;
        old_str++;
    }

    while (*string)
    {
        *p_new_str = *string;
        p_new_str++;
        string++;
    }

    new_str[new_length] = '\0';

    delete[] str;
    str = new_str;
    length = new_length;
}

int String::find(char *string) {
    char *pstr = str;
    char find_str_length = strlen(string);

    for (int i = 0; i < length; i++)
    {
        int found = 1;
        for (int j = 0; j < find_str_length; j++)
        {
            if(pstr[j] != string[j]) {
                found = 0;
                break;
            }
        }
        // 찾았으면 리턴
        if(found) {
            return i;
        }
        pstr++; // 한칸씩 전진
    }
    return -1;
}

int String::compare_str(char *string) {
    char *pstr = str;
    int count = 0;
    while (*pstr)
    {
        if(*pstr != *string) return 1;
        string++;
        pstr++;
    }
    return 0;
}

int String::compare_size(char *string) {
    char *pstr = str;
    while (*string)
    {
        if(*pstr > *string) {
            return 1;
        } else if(*pstr < *string) {
            return -1;
        }
        pstr++;
        string++;
    }

    if(!*pstr && !*string) {
        return 0;
    }

    return (*pstr == '\0') ? -1 : 1;
}

void String::print() const {
    std::cout << str << std::endl;
}

int main() {
    char c[5] = "abcd";
    String *str = new String(c);
    str->concat(c);
    str->print(); // "abcdabcd"

    char find[7] = "da";
    std::cout << str->find(find) << std::endl; // 3
    std::cout << str->compare_size(find) << std::endl; // -1
    return 0;
}