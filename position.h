#ifndef POSITION_H
#define POSITION_H

#include "direction.h"

#include "utility"

using coordinate_t = int;

class Position
{
    using pos_point_t = std::pair<coordinate_t, coordinate_t>;
public:
    Position(pos_point_t _position, Direction _direction);

    pos_point_t get() const;
    void set(pos_point_t new_position);
};

#endif