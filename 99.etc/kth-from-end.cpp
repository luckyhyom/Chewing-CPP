// 10:16PM ~ 10:43PM

#include <iostream>

class Node {
    public:
        int value;
        Node* next;
        Node(int value):value(value){};
};

class LinkedList {
    Node* head;

    public:
        LinkedList() {
            std::cout << "??" << std::endl;
            head = new Node(0);
        }
        LinkedList& add(int n) {
            Node* node = head;
            while (node->next)
            {
                node = node->next;
            }
            node->next = new Node(n);
            return *this;
        }
        void printAll() {
            Node* node = head;
            while (node->next)
            {
                node = node->next;
                std::cout << node->value << std::endl;
            }
        }
        Node& findKthFromEnd(int k) {
            Node* runner = head;
            Node* result = head;

            for (int i = 0; i < k; i++)
            {
                runner = runner->next;
            }
            
            while (runner)
            {
                runner = runner->next;
                result = result->next;
            }
            return *result;
        }
};

int main() {
    LinkedList* list = new LinkedList();
    list->add(100);
    list->add(200);
    list->add(300);
    list->add(400);
    list->printAll(); // 100,200,300

    Node& found = list->findKthFromEnd(2);
    std::cout << found.value << std::endl; // 300
    return 0;
}