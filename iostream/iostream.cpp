/*
    stream: 데이터 입/출력
        - stream 라이브러리는 입/출력중인 데이터 흐름을 제어하는 라이브러리다.
            - 1/2/4바이트 단위로 읽거나, 하는 등
        - buffer: I/O 효율을 위해 stream되는 데이터를 메모리에 일정량 모아놓았다가 하드에 I/O한다.
    flash: 버퍼 안의 모든 데이터를 출력하여 버퍼를 비우는 것
*/
#include <iostream>
using namespace std;
int main() {
    std::string s;
    std::cin >> s;

    char peek = std::cin.rdbuf()->snextc();
    if(std::cin.fail()) std::cout << "Failed";
    std::cout << "두 번째 단어 맨 앞글자 : " << peek << std::endl;
    std::cin >> s;
    std::cout << "다시 읽으면 : " << s << std::endl;
}