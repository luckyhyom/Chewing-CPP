#include "Cell.h"
#include <ctime>

class StringCell : public Cell {
  public:
    StringCell(const std::string *data) : Cell(data) {};
    int to_numeric();
    std::string stringify();
};
