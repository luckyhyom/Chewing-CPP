#include <iostream>
/*
    1. 각 차원의 크기를 정하는 것 부터가 애매함.
    2. n차원 선언을 어떻게 하지.. 3차원이면 int*** 또는 int[][][] 이렇게
        -> 연결 리스트
    3. []연산자 오버로드 실풰!
*/
class Array {
    int n;
    int size;
    int* data;

    public:
        Array(int n_, int size_);

        // int / int* 두가지 경우의 수가 반환될수도 있는 경우에는?
        int operator[](int index) {
            return data[index];
        }
        Array& operator[](Array& arr) {
            std::cout << "good?" << std::endl; // 뭔가 포인터(커서)를 이동시킬 것 같은 느낌? 체이닝되어야할것같은데.. 그게 되나?
            return *this;
        }
};

Array::Array(int n_, int size_) {
    n = n_;
    size = size_;
    data = new int[n * size];
    for (size_t i = 0; i < n * size; i++)
    {
        data[i] = i + 1;
    }
    
}

int main() {
    Array arr = Array(2,12);
    int t = arr[0];
    std::cout << t << std::endl;
    //[2][3];
    return 0;
}



/*
    int *arr = [2][2][3];를 만들고 싶다.
    int 자료형 12개 크기의 메모리를 할당한다.

    int arr = new int[12];

    arr[1][1][2]을 조회하면..
    [x][y][z]일때 x = y*z, y = z 만큼이므로
    (int * 6 * 1) + (int * 3 * 1) + (int * 2)번째 메모리 주소를 조회한다.

            (x * (ym*zm)        ) + (y * zm) + (z)
    index = (x * (y_max * z_max)) + (y * z_max) + z
*/

