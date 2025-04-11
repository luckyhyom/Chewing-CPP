#include "Cell.h"
#include <ctime>

class ExprCell : public Cell {
  public:
    ExprCell(const std::string *data) : Cell(data) {};
    int to_numeric();
    std::string stringify();
    ~ExprCell();
};
