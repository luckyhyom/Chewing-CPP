#include <iostream>

namespace MyArray {    
class Array;
class Int;
class Array {
    friend Int;

    const int dim;
    int* size;


    // 재귀 호출을 통해 N개의 Address를 초기화
    struct Address {
        int level;
        void* next;
    };

    Address* top; // Address를 가리키는 포인터라는 것을 컴파일러에게 알려준다. 메모리는 할당되지 않음

    
    void init_address(Address* current) {
        if(!current) return;
        if(current->level == dim - 1) {
            current->next = new int[size[current->level]];
            for (int i = 0; i < size[dim-1]; i++) static_cast<int*>(current->next)[i] = i;
            return;
        }
        current->next = new Address[size[current->level]]; 
        for (int i = 0; i < size[current->level]; i++) {
            Address& next = static_cast<Address*>(current->next)[i];
            next.level = current->level + 1;
            init_address(&next);
        }
    }

    /*
        return (종료)가 필요한 경우와 아닌 경우는?
        init과 delete 함수의 로직이 잘 이해가 안된다. 각 함수 로직의 순서를 바꿀순 없나?
    */
    void delete_address(Address* current) {
        if(!current) return;
        /*
            [2][3][4]
            
            1. 두번 돈다.
            2. 세번 돈다.
            3. 네번 돈다.

            [for문에서 current->level < dim - 1 역할]
            - Level 2는 for문 실행하지 않도록 해야함, next를 제거하고 있으므로 Level2에서 Level3을 제거하게 되므로.
                3차원 배열에서 Level4는 없기때문에.. Level3까지만 제거하면 됨
            - 재귀 멈추는 역할
        */
        for (int i = 0; i < size[current->level] && current->level < dim - 1; i++) {
            delete_address(&static_cast<Address*>(current->next)[i]);
        }
        if(current->level == dim - 1) {
            delete[] static_cast<int*>(current->next);
        } else {
            delete[] static_cast<Address*>(current->next);
        }
        std::cout << "Deleting level: " << current->level << std::endl;
    }

    //void delete_address(Address* current) {
    //    if(!current) return;
    //    if(current->level == dim - 1) {
    //        delete[] static_cast<int*>(current->next);
    //        return;
    //    }
    //    for (int i = 0; i < size[current->level]; i++) {
    //        delete_address(&static_cast<Address*>(current->next)[i]);
    //    }
    //    std::cout << "Deleting level: " << current->level << std::endl;
    //}

    void copy_values(Address* dest, Address* source) {
        if(!dest || !source) return;
        if(dest->level == dim - 1 && source->level == dim - 1) {
            for (int i = 0; i < size[dim-1]; i++) static_cast<int*>(dest->next)[i] = static_cast<int*>(source->next)[i] + 1;
            return;
        } else {
            for (int i = 0; i < size[dest->level]; i++) {
                copy_values(static_cast<Address*>(dest->next) + i, static_cast<Address*>(source->next) + i);
            }
        }
    }

    void print_all_(Address* addr) {
        if(addr->level == dim - 1) {
            for (int i = 0; i < size[dim-1]; i++) {
                std::cout << static_cast<int*>(addr->next)[i];
            }
            std::cout << std::endl;
        } else {
            for (int i = 0; i < size[addr->level]; i++) {
                print_all_(static_cast<Address*>(addr->next) + i);
            }
        }
    }

    public:
        class Iterator {
            int* locations;
            Array* arr;
            public:
                Iterator(Array* arr, int* loc = NULL): arr(arr) {
                    locations = new int[arr->dim];
                    for (int i = 0; i < arr->dim; i++) locations[i] = (loc != NULL ? loc[i] : 0);
                }
                Iterator(const Iterator& it): arr(it.arr) {
                    locations = new int [arr->dim];
                    for (int i = 0; i < arr->dim; i++) locations[i] = it.locations[i];
                }
                ~Iterator() {
                    delete[] locations;
                }
                Iterator& operator++() {
                    if(locations[0] >= arr->size[0]) return (*this);
                    bool carry = false;
                    int i = arr->dim - 1;
                    do
                    {
                        locations[i]++;
                        if(locations[i] >= arr->size[i] && i >= 1) {
                            locations[i] -= arr->size[i];
                            carry = true;
                            i--;
                        } else {
                            carry = false;
                        }
                    } while (i >= 0 && carry);
                    
                    return (*this);
                }

                Iterator operator++(int) {
                    Iterator itr(*this);
                    ++(*this);
                    return itr;
                }

                Iterator& operator=(const Iterator& itr) {
                    arr = itr.arr;
                    delete[] locations;
                    locations = new int[itr.arr->dim]; // Q1.
                    for (int i = 0; i < arr->dim; i++)
                    {
                        locations[i] = itr.locations[i];
                    }
                    return (*this);
                }

                bool operator!= (const Iterator& itr) {
                    if(arr->dim != itr.arr->dim) return true;
                    if(arr != itr.arr) return true;

                    for (int i = 0; i != arr->dim; i++)
                    {
                        if(locations[i] != itr.locations[i]) return true;
                    }
                    return false;
                }

                Int operator*();
        };
        Array(int dim, int* arr_size):dim(dim) {
            size = new int[dim];
            for (int i = 0; i < dim; i++) size[i] = arr_size[i];
            top = new Address;
            top->level = 0;
            init_address(top);
        }

        Array(const Array& arr): dim(arr.dim) {
            std::cout << "COPY!" << std::endl;
            size = new int[dim];
            for (int i = 0; i < dim; i++) size[i] = arr.size[i];
            top = new Address;
            top->level = 0;
            init_address(top);

            copy_values(top, arr.top);
        }

        ~Array() {
            delete_address(top);
            delete top;
            delete[] size;
        }

        void print_info() {
            std::cout << "array info: array";
            for (int i = 0; i < dim; i++) {
                std::cout << "[" << size[i] << "]";
            }
            std::cout << std::endl;
        }

        void print_all() {
            print_all_(top);
        }

        Iterator begin() {
            int *locations = new int[dim];
            for (int i = 0; i < dim; ++i) locations[i] = 0;
            Iterator itr(this, locations);
            delete[] locations;
            return itr;
        };
        
        Iterator end() {
            int *locations = new int[dim];
            locations[0] = size[0];
            for (int i = 1; i < dim; ++i) locations[i] = 0;
            Iterator itr(this, locations);
            delete[] locations;
            return itr;
        };

        Int operator[](const int index);
};


class Int {
    void* data;
    int level;
    Array* array;

    public:
        Int(int index, int _level = 0, void* _data = NULL, Array* _array = NULL): level(_level), data(_data), array(_array) {
            if(_level < 1 || index >= array->size[_level - 1]) {
                data = NULL;
                return;
            };
            if(level == array->dim) {
                data = static_cast<void*>((static_cast<int*>(static_cast<Array::Address*>(data)->next) + index));
            } else {
                data = static_cast<void*>(static_cast<Array::Address*>(static_cast<Array::Address*>(data)->next) + index);
            }
        }

        Int(const Int& i) : data(i.data), level(i.level), array(i.array) {}
        
        operator int() {
            if(data) return *static_cast<int*>(data);
            return 0;
        }
        
        Int& operator=(const int& a) {
            if (data) *static_cast<int*>(data) = a;
            return *this;
        }

        Int operator[](const int index) {
            /*
                new 유무의 차이..............!
                new가 있다면
                1. Heap에 메모리 할당
                2. 포인터를 반환

                new가 없다면
                1. 스택에 메모리 할당
                2. 넘길 때.. 지역변수는 데이터가 소멸되는데 괜찮나?
            */
            if(!data) return 0;
            return Int(index, level + 1, data, array);
        }

};

Int Array::operator[](const int index) { 
    /*
        new 유무의 차이..............!
        new가 있다면
        1. Heap에 메모리 할당
        2. 포인터를 반환

        new가 없다면
        1. 스택에 메모리 할당
        2. 넘길 때.. 지역변수는 데이터가 소멸되는데 괜찮나?
    */
    return Int(index, 1, static_cast<void*>(top), this);
}

Int Array::Iterator::operator*() {
    Int start = (*arr)[locations[0]];
    for (int i = 1; i < arr->dim; ++i)
    {
        start = start[locations[i]];
    }
    return start;
}
}

int main() {
    int dim = 3;
    int size[] = {2,3,4};
    MyArray::Array* arr = new MyArray::Array(dim, size);
    MyArray::Array::Iterator itr = arr->begin();
    for (int i = 0; itr != arr->end(); itr++, i++)
    {
        (*itr) = i + 1;
    }
    
    for (itr = arr->begin(); itr != arr->end(); itr++)
    {
        std::cout << *itr << std::endl;
    }
    delete arr;
    return 0;
}