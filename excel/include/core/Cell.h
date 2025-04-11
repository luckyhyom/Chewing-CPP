#ifndef CELL_H
#define CELL_H

#include <string>

class Table;

class Cell
{
friend class Table;
protected:
  Table *table;
  const void* data;
  int x, y;

public:
  Cell(const void *data);
  virtual int to_numeric() = 0;
  virtual std::string stringify() = 0;
  virtual ~Cell() {};
};

#endif // CELL_H