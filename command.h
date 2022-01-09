#ifndef COMMAND_H
#define COMMAND_H

#include "sensor.h"

#include <vector>
#include <initializer_list>
#include <memory>
#include <algorithm>

class command
{
protected:

    bool check_safety(Position& unchecked_new_position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        return std::all_of
        (
            sensors.begin(),
            sensors.end(),
            [&] (const std::unique_ptr<Sensor> & sensor) 
            {
                const auto& [x, y] = unchecked_new_position.get_coords();
                return sensor->is_safe(x, y);
            }
        );
    }

    void check_and_possibly_set(Position &position, Position& unchecked_new_position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        bool is_safe = check_safety(unchecked_new_position, sensors);
        if(is_safe)
            position = unchecked_new_position;
        else
            throw BrokenRover();    
    }

public:

    virtual ~command() {}
    virtual void execute([[maybe_unused]] Position &position, [[maybe_unused]] const std::vector<std::unique_ptr<Sensor>> &sensors) const {};

};

class compose : public command
{
private:
    std::vector<command> components;

public:
    compose(std::initializer_list<command> _components)
        : components(_components) {}

    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        std::for_each(
            components.begin(),
            components.end(),
            [&] (const command& c) { c.execute(position, sensors); }
        );

    }
};

class rotate_right : public command
{
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_right();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }   
};

class rotate_left : public command
{
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_left();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }   
};

class move_forward : public command
{
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        Position unchecked_new_position = position;
        unchecked_new_position.move_forward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }  
};

class move_backward : public command
{
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        Position unchecked_new_position = position;
        unchecked_new_position.move_backward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }  
};

#endif