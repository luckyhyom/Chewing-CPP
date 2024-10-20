#include <iostream>

class Employee {
    protected:
        std::string name;
        int age;
        std::string position;
        int rank;

    public:
        Employee(std::string name, int age, std::string position, int rank): name(name), age(age), position(position), rank(rank) {}
        Employee(Employee& emp) {
            name = emp.name;
            age = emp.age;
            position = emp.position;
            rank = emp.rank;
        }

        void print_info() {
            std::cout << name << "(" << position << ", " << age << ") ==> " << calculate_pay() << "만원" << std::endl;
        }

        int calculate_pay() {
            return 200 + rank * 50;
        }
};

class Manager : public Employee {
    int year_of_service;
    public: 
        Manager(std::string name, int age, std::string position, int rank, int year_of_service): Employee(name,age,position,rank), year_of_service(year_of_service) {}
        Manager(Manager& manager): Employee(manager.name, manager.age, manager.position, manager.rank), year_of_service(manager.year_of_service) {}

        /*
            1. 직급에 따라서 메서드 오버라이딩 (if문으로 처리하지 않아도 된다.)
            2. 직급별로 다른 메서드를 가질 수 있다.
        */
        int calculate_pay() { return 200 + rank * 50 + 5 * year_of_service; }
        void print_info() {
            std::cout << name << " (" << position << " , " << age << ", "
                    << year_of_service << "년차) ==> " << calculate_pay() << "만원"
                    << std::endl;
        }
};

class EmployeeList {
    int alloc_employee;

    int current_employee;
    int current_manager;

    Employee** employee_list;
    Manager** manager_list;

    public:
        EmployeeList(int alloc_employee): alloc_employee(alloc_employee) {
            employee_list = new Employee*[alloc_employee];
            manager_list = new Manager*[alloc_employee];
            current_employee = 0;
            current_manager = 0;
        }

        void add_emplyee(Employee* emp) {
            ++current_employee;
            if(current_employee >= alloc_employee) {
                Employee** new_emplyee_list = new Employee*[current_employee * 2];
                for (int i = 0; i < current_employee; i++) new_emplyee_list[i] = employee_list[i];
                delete[] employee_list;
                employee_list = new_emplyee_list;
            }
            employee_list[current_employee - 1] = emp;
        }

        void add_manager(Manager* mg) {
            ++current_manager;
            if(current_manager >= alloc_employee) {
                Manager** new_manager_list = new Manager*[current_manager * 2];
                for (int i = 0; i < current_manager; i++) new_manager_list[i] = manager_list[i];
                delete[] manager_list;
                manager_list = new_manager_list;
            }
            manager_list[current_manager - 1] = mg;
        }

        int current_emplyee_num() { return current_employee + current_manager; }
        void print_employee_info() {
            int total_pay = 0;
            for (int i = 0; i < current_employee; i++)
            {
                employee_list[i]->print_info();
                total_pay+= employee_list[i]->calculate_pay();
            }
            
            for (int i = 0; i < current_manager; i++)
            {
                manager_list[i]->print_info();
                total_pay+= manager_list[i]->calculate_pay();
            }
            
            std::cout << "total pay: " << total_pay << std::endl;
        }

        ~EmployeeList() {
            for (int i = 0; i < current_employee; i++)
            {
                delete employee_list[i];
            }
            for (int i = 0; i < current_manager; i++)
            {
                delete manager_list[i];
            }
            delete[] employee_list;
            delete[] manager_list;
        }
};

int main () {
    Employee emp("Hyomin", 30, "Software Engineer", 3);
    Employee emp2("Hanni", 20, "Idol", 3);
    Manager emp3("Min HeeJin", 46, "Producer", 3, 10);
    EmployeeList emp_list(10);
    emp_list.add_emplyee(&emp);
    emp_list.add_emplyee(&emp2);
    emp_list.add_manager(&emp3);
    emp_list.print_employee_info();
    return 0;
}