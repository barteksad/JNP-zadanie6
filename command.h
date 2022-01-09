#ifndef COMMAND_H
#define COMMAND_H

#include "sensor.h"

#include <vector>
#include <initializer_list>
#include <memory>
#include <algorithm>

class Command
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

    virtual ~Command() {}
    virtual void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const;

};

class ComposedCommand : public Command
{
private:
    std::vector<std::unique_ptr<Command>> components;

public:
    ComposedCommand(std::vector<std::unique_ptr<Command>> _components)
        : components(std::move(_components)) {}
        // {
        //     components = std::vector<std::unique_ptr<Command>> {std::make_move_iterator(commands.begin()),std::make_move_iterator(commands.end())};
        // };

    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const override
    {
        std::for_each(
            components.begin(),
            components.end(),
            [&] (const std::unique_ptr<Command>& c) { c->execute(position, sensors); }
        );

    }
};

class RotateRight : public Command
{
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_right();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }   
};

class RotateLeft : public Command
{
public:
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        Position unchecked_new_position = position;
        unchecked_new_position.rotate_left();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }   
};

class MoveForward : public Command
{
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        Position unchecked_new_position = position;
        unchecked_new_position.move_forward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }  
};

class MoveBackward : public Command
{
    void execute(Position &position, const std::vector<std::unique_ptr<Sensor>> &sensors) const
    {
        Position unchecked_new_position = position;
        unchecked_new_position.move_backward();
        check_and_possibly_set(position, unchecked_new_position, sensors);
    }  
};

std::unique_ptr<Command> move_forward()
{
    return std::make_unique<MoveForward>();
}

std::unique_ptr<Command> move_backward()
{
    return std::make_unique<MoveBackward>();
}

std::unique_ptr<Command> rotate_left()
{
    return std::make_unique<RotateLeft>();
}

std::unique_ptr<Command> rotate_right()
{
    return std::make_unique<RotateRight>();
}

std::unique_ptr<Command> compose(std::vector<std::unique_ptr<Command>>&& components)
{
    // std::vector<std::unique_ptr<Command>> components{std::make_move_iterator(std::begin(_components)),std::make_move_iterator(std::end(_components))};;
    return std::make_unique<ComposedCommand>(std::move(components));
}

#endif