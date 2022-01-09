#ifndef DIRECTION_H
#define DIRECTION_H

#include <iostream>

enum Direction : int
{
    WEST, NORTH, EAST, SOUTH
};

inline std::ostream& operator<<(std::ostream& os, const Direction& direction)
{
    switch(direction)
    {
        case WEST:
            os << "WEST";
            break;
        case NORTH:
            os << "NORTH";
            break;
        case EAST:
            os << "EAST";
            break;
        case SOUTH:
            os << "SOUTH";
            break;
    }

    return os;
}


#endif