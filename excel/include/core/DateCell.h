#include "Cell.h"
#include <ctime>

class DateCell : public Cell {
  public:
    DateCell(const time_t *data) : Cell(data) {};
    int to_numeric();
    std::string stringify();
    ~DateCell();
};