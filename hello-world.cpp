#include <iostream>

int main() {
    std::cout
        << "Hello, World!!!" << std::endl
        << "What is your name?" << std::endl;

    char user_name[20];
    std::cin >> user_name;

    std::cout
        << "Welcome to this here " << user_name << "!!" << std::endl
        << "What is your favorate number?" << std::endl;

    int favorate_number;
    std::cin >> favorate_number;

    std::cout << user_name << "'s favorate number is " << favorate_number << "! :D" << std::endl;

    return 0;
}