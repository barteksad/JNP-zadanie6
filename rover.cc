#include "rover.h"

[[maybe_unused]]
std::ostream& operator<<(std::ostream& os, const Rover& rover)
{
    os << rover.position;
    return os;
}