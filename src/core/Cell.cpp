// #include "Cell.h"
#include "core/Cell.h"

Cell::Cell(const std::string &data) : data(data) {}

int Cell::to_numeric()
{
    return 0;
}

std::string Cell::stringify()
{
    return data;
}