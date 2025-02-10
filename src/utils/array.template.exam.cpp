#include <array>
#include <iostream>

/**
 * 타입이 아닌 템플릿 인자
 * 배열의 크기를 매개변수로 전달하는 과정을 제거하여 실수를 방지한다.
 */
template <typename T, std::size_t N>
void printArray(const std::array<T, N>& arr) {
    std::cout << "배열 크기: " << N << std::endl;  // 크기 정보 유지됨
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    printArray(arr);
}
