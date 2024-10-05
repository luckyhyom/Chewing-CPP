#include <iostream>

class Array {
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
            //for (int i = 0; i < size[dim-1]; i++) static_cast<int*>(current->next)[i] = i;
            return;
        }
        current->next = new Address[size[current->level]]; 
        for (int i = 0; i < size[current->level]; i++) {
            Address& next = static_cast<Address*>(current->next)[i];
            next.level = current->level + 1;
            init_address(&next);
        }
    }

    void delete_address(Address* current) {
        if(!current) return;
        if(current->level == dim - 1) {
            delete[] static_cast<int*>(current->next);
            return;
        }
        for (int i = 0; i < size[current->level]; i++) {
            delete_address(&static_cast<Address*>(current->next)[i]);
        }
        delete[]  static_cast<Address*>(current);
    }

    void print_all_(Address* addr) {
        if(addr->level == dim - 1) {
            for (int i = 0; i < size[dim-1]; i++) {
                std::cout << static_cast<int*>(addr->next)[i];
            }
        } else {
            print_all_(static_cast<Address*>(addr->next));
        }
    }

    public:
        Array(int dim, int* arr_size):dim(dim) {
            size = new int[dim];
            for (int i = 0; i < dim; i++) size[i] = arr_size[i];
            top = new Address;
            top->level = 0;
            init_address(top);
        }

        ~Array() {
            delete_address(top);
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
};

int main() {
    int dim = 2;
    int size[] = {2,10};
    Array* arr = new Array(dim, size);

    arr->print_info();
    arr->print_all();

    delete arr;
    return 0;
}