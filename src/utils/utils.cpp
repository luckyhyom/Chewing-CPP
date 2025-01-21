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

Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}


StringCell::StringCell(string data, int x, int y, Table* t)
  : data(data), Cell(x, y, t) {}
string StringCell::stringify() { return data; }
int StringCell::to_numeric() { return 0; }

NumberCell::NumberCell(int data, int x, int y, Table* t)
  : data(data), Cell(x, y, t) {}
string NumberCell::stringify() { return std::to_string(data); }
int NumberCell::to_numeric() { return data; }


string DateCell::stringify() { return 
  char buf[50];
  tm temp;
  // cppcheck-suppress uninitvar
  localtime_s(&temp, &data); // int 자료형으로 저장된 시간 데이터를 tm 구조체에 맞게 변환한다.

  strftime(buf, 50, "%F", &temp); // tm 구조체의 데이터를 yyyy-mm-dd 꼴로 변환

  return string(buf); // std::string 자료형으로 변환한다.
}
int DateCell::to_numeric() { return static_cast<int>(data); }

DateCell::DateCell(string data, int x, int y, Table* t)
  : data(data), Cell(x, y, t) {
    // 입력받는 Date 형식은 항상 yyyy-mm-dd 꼴이라 가정한다.
    int year = atoi(s.c_str());
    int month = atoi(s.c_str() + 5);
    int day = atoi(s.c_str() + 8);

    // 시간 데이터 구조체
    tm timeinfo;

    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_day = day;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // 시간 데이터 구조체 데이터를 int 자료형으로 변환한다.
    data = mktime(&timeinfo);
}

ExprCell::ExprCell(string data, int x, int y, Table* t): data(data), Cell(x, y, y) {}

// 이미 배열의 데이터는 정렬이 되어있는 상태다.
int ExprCell::to_numeric() {
  double result = 0;
  NumStack stack;

  // 배열의 크기만큼 순회
  for (int i = 0; i < exp_vec.size(); i++) {
    string s = exp_vec[i]; // 배열

    // 셀 일 경우 (A3, B2 따위) -> 이해 안됨
    if (isalpha(s[0])) { //
      stack.push(table->to_numeric(s));
    } // 숫자일 경우
    else if (isdigit(s[0])) {
      stack.push(atoi(s.c_str()));
    } else { // 연산자일 경우
      double y = stack.pop(); // 
      double x = stack.pop();
      switch (s[0]) {
        case '+':
          stack.push(x + y);
          break;
        case '-':
          stack.push(x - y);
          break;
        case '*':
          stack.push(x * y);
          break;
        case '/':
          stack.push(x / y);
          break;
      }
    }
  }
  return stack.pop();
}

int ExprCell::precedence(char c) {
  switch (c)
  {
  case '(':
  case '[':
  case '{':
    return 0;
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  }
  return 0;
}

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
    for (int j = 0; j < max_col_size; j++) {
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

std::ostream& operator<<(std::ostream& o, Table& table) {
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
  total_table += "    "; // 모서리 공백
  int total_wide = 4;  // 엑셀 넓이?
  for (int i = 0; i < max_col_size; i++) { // 열 개수만큼 순회한다.
    if(col_max_wide[i]) { // 각 열의 크기
      int max_len = std::max(2, col_max_wide[i]); // 2칸 혹은 현재 열 길이
      total_table += " | " + col_num_to_str(i); // 알파벳 추가
      total_table += repeat_char(max_len - col_num_to_str(i).length(), ' '); // 컬럼 길이 - 알파벳 길이 만큼 공백 추가

      total_wide += (max_len + 3); // 컬럼 길이 + 3만큼 엑셀 넓이를 늘린다.
    }
  }

  total_table += "\n"; // 한칸 내리고
  for (int i = 0; i < max_row_size; i++) { // 행 수만큼 순회한다.
    total_table += repeat_char(total_wide, '-'); // 엑셀 넓이만큼 '-' 추가
    total_table += "\n" + std::to_string(i + 1); // 한칸 내리고, 행 번호 입력
    total_table += repeat_char(4 - std::to_string(i + 1).length(), ' '); // 행 번호 길이만큼 빼고 공백 처리

    for (int j = 0; j < max_col_size; j++) { // 열 수만큼 순회한다.
      if (col_max_wide[i]) { // 각 열의 크기
        int max_len = std::max(2, col_max_wide[j]); // 2칸 혹은 현재 열의 길이
        string s = "";
        if (data_table[i][j]) { // 데이터가 있으면
          s = data_table[i][j]->stringify(); // 데이터로 치환
        }
        total_table += " | " + s; // 셀에 데이터 입력
        total_table += repeat_char(max_len - s.length(), ' '); // 최대 길이에서 현재 글자수 길이 뺀 만큼 공백 추가
      }
    }
    total_table += "\n"; // 한칸 내리고
  }

  return total_table;
}

string TxtTable::repeat_char(int n, char c) {
  string s = "";
  for (int i = 0; i < n; i++) s.push_back(c);
  
  return s;
}

// 숫자로 된 열 번호를 A, B, .... Z, AA, AB, ... 이런 순으로 매겨준다.
string TxtTable::col_num_to_str(int n) {
  string s = "";
  if (n < 26) {
    s.push_back('A' + n);
  } else {
    char first = 'A' + n / 26 - 1;
    char second = 'A' + n % 26;

    s.push_back(first);
    s.push_back(second);
  }

  return s;
}
}