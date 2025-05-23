#ifndef UTILS_H
#define UTILS_H
#include <string>
using std::string;

namespace MyExcel {
class Vector {
  string* data;
  int capacity;
  int length;

 public:
  // 생성자
  Vector(int n = 1);

  // 맨 뒤에 새로운 원소를 추가한다.
  void push_back(string s);

  // 임의의 위치의 원소에 접근한다.
  string operator[](int i);

  // x 번째 위치한 원소를 제거한다.
  void remove(int x);

  // 현재 벡터의 크기를 구한다.
  int size();

  ~Vector();
};

class Stack {
  struct Node {
    Node* prev;
    string s;

    Node(Node* prev, string s) : prev(prev), s(s) {}
  };

  Node* current;
  Node start;

 public:
  Stack();

  // 최상단에 새로운 원소를 추가한다.
  void push(string s);

  // 최상단의 원소를 제거하고 반환한다.
  string pop();

  // 최상단의 원소를 반환한다. (제거 안함)
  string peek();

  // 스택이 비어있는지의 유무를 반환한다.
  bool is_empty();

  ~Stack();
};
class NumStack {
  struct Node {
    Node* prev;
    double s;

    Node(Node* prev, double s) : prev(prev), s(s) {}
  };

  Node* current;
  Node start;

 public:
  NumStack();
  void push(double s);
  double pop();
  double peek();
  bool is_empty();

  ~NumStack();
};

class Table;

std::ostream& operator<<(std::ostream& o, Table& table);
class Cell {
  protected:
    int x,y;
    MyExcel::Table* table;
  public:
    virtual string stringify() = 0;
    virtual int to_numeric() = 0;

    Cell(int x, int y, MyExcel::Table* table);
    virtual ~Cell() = default; // 가상 소멸자
};

class StringCell : public Cell {
  string data;
  public:
    string stringify();
    int to_numeric();

    StringCell(string data, int x, int y, Table* t);
};

class NumberCell : public Cell {
  int data;
  public:
    string stringify();
    int to_numeric();

    NumberCell(int data, int x, int y, Table* t);
};

class DateCell : public Cell {
  time_t data;
  public:
    string stringify();
    int to_numeric();
    
    DateCell(string s, int x, int y, Table* t);
};

class ExprCell : public Cell {
  string data;
  //string* parsed_expr;

  Vector exp_vec;

  // 연산자 우선 순위를 반환합니다.
  int precedence(char c);

  // 수식을 분석합니다.
  void parse_expression();

  public:
    ExprCell(string data, int x, int y, Table* t);
    string stringify();
    int to_numeric();
};
class Table{
  protected:
    int max_row_size, max_col_size;
    Cell*** data_table;
  
  public:
    Table(int max_row_size, int max_col_size);
    ~Table();

    void reg_cell(Cell* c, int row, int col);
    int to_numeric(const string& s);
    int to_numeric(int row, int col);
    string stringify(const string& s);
    string stringify(int row, int col);
    virtual string print_table() = 0;
};


class TxtTable: public Table {
  string repeat_char(int n, char c);

  // 숫자로 된 열 번호를 A, B, .... Z, AA, AB, ... 이런 순으로 매겨준다.
  string col_num_to_str(int n);

  public:
    TxtTable(int row, int col);

    // 텍스트로 표를 깨끗하게 출력해준다.
    string print_table();
};

class Excel {
  Table* current_table;
  public:
    Excel(int max_row, int max_col, int choice);

    int parse_user_input(string s);
    void command_line();
};

};
#endif