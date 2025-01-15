#include "utils.h"
#include <string>

namespace MyExcel {
Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {}
void Vector::push_back(string s) {
  if(capacity <= length) {
    std::string* temp = new string[capacity * 2];
    for (size_t i = 0; i < length; i++) {
      temp[i] = this->data[i];
    }
    delete[] data;
    data = temp;
    capacity *= 2;
  }

  data[length] = s;
  length++;
}
string Vector::operator[](int i) { return data[i]; }
void Vector::remove(int x) {
  for (int i = x + 1; i < length; i++) {
    data[i - 1] = data[i];
  }
  length--;
}
int Vector::size() { return length; }
Vector::~Vector() {
  if(data) {
    delete[] data;
  }
}

Stack::Stack() : start(NULL, "") { current = &start; }
void Stack::push(string s) {
  Node* n = new Node(current, s);
  current = n;
}
string Stack::pop() {
  if(current == &start) { return ""; }
  string s = current->s;
  Node* popped = current;
  current = current->prev;

  delete popped;
  return s;
}
string Stack::peek() { return current->s; }
bool Stack::is_empty() {
  if(current == &start) return true;
  return false;
}
Stack::~Stack() {
  while (current != &start) {
    Node* popped = current;
    current = current->prev;
    delete popped;
  }
}
NumStack::NumStack() : start(NULL, 0) { current = &start; }
void NumStack::push(double s) {
  Node* n = new Node(current, s);
  current = n;
}
double NumStack::pop() {
  if (current == &start) return 0;

  double s = current->s;
  Node* prev = current;
  current = current->prev;

  // Delete popped node
  delete prev;
  return s;
}
double NumStack::peek() { return current->s; }
bool NumStack::is_empty() {
  if (current == &start) return true;
  return false;
}
NumStack::~NumStack() {
  while (current != &start) {
    Node* prev = current;
    current = current->prev;
    delete prev;
  }
}


Cell::Cell(string data, int x, int y, Table* table)
  : data(data), x(x), y(y), table(table) {}

string Cell::stringify() { return data; }
int Cell::to_numeric() { return 0; }

Table::Table(int max_row_size, int max_col_size)
    : max_row_size(max_row_size), max_col_size(max_col_size) {
  data_table = new Cell**[max_row_size];
  for (size_t i = 0; i < max_row_size; i++) {
    data_table[i] = new Cell*[max_col_size];
    for (size_t j = 0; j < max_col_size; j++)
    {
      data_table[i][j] = NULL;
    }
  }
}

Table::~Table() {
  for (int i = 0; i < max_row_size; i++) {
    for (int i = 0; i < max_col_size; i++) {
      if(data_table[i][j]) delete data_table[i][j];
    }
  }
  for (size_t i = 0; i < max_row_size; i++) {
    delete[] data_table[i];
  }
  delete[] data_table;
}

void Table::reg_cell(Cell* c, int row, int col) {
  if(!(row < max_row_size && col < max_col_size)) return;
  if (data_table[row][col]) {
    delete data_table[row][col];
  }
  data_table[row][col] = c;
}

int Table::to_numeric(const string& s) {
  // Cell 이름으로 받는다.
  int col = s[0] - 'A';
  int row = atoi(s.c_str() + 1) - 1;

  if (row < max_row_size && col < max_col_size) {
    if( data_table[row][col]) {
      return data_table[row][col]->to_numeric();
    }
  }
}
int Table::to_numeric(int row, int col) {
  if (row < max_row_size && col < max_col_size && data_table[row][col]) {
    return data_table[row][col]->to_numeric();
  }
  return 0;
}
string Table::stringify(const string& s) {
  // Cell 이름으로 받는다.
  int col = s[0] - 'A';
  int row = atoi(s.c_str() + 1) - 1;

  if(row < max_row_size && col < max_col_size) {
    if (data_table[row][col]) {
      return data_table[row][col]->stringify();
    }
  }
  return 0;
}

string Table::stringify(int row, int col) {
  if (row < max_row_size && col < max_col_size && data_table[row][col]) {
    return data_table[row][col]->stringify();
  }
  return "";
}
std::ostream& operator<<(std::stream& o, Table& table) {
  o << table.print_table();
  return o;
}

TxtTable::TxtTable(int row, int col) : Table(row, col) {}

string TxtTable::print_table() {
  string total_table;
  
  // 열 개수만큼의 크기의 int 배열을 만든다.
  int* col_max_wide = new int[max_col_size];

  // 열 A부터 Z까지 순회한다.
  for (int i = 0; i < max_col_size; i++) {
    unsigned int max_wide = 2; // 최대 넓이 2
    for (int j = 0; j < max_row_size; j++) { // 행을 순회 한다.


      if (data_table[j][i] && // 데이터가 있으면서
          data_table[j][i]->stringify().length() > max_wide // 최대 넓이보다 길다면
          ) {
            max_wide = data_table[j][i]->stringify().length(); // 최대 넓이를 해당 데이터의 길이만큼으로 늘린다.
      }
    }
    col_max_wide[i] = max_wide; // i열의 최대 넓이가 정해진다. -> 열 별로 넓이가 다르다.
  }

  // 맨 상단에 열 정보 표시
  total_table += "    ";
  int total_wide = 4;
  for (int i = 0; i < max_col_size; i++) {
    //if()
  }
}
}