#ifndef COMMAND_H
#define COMMAND_H

#include "sensor.h"

#include <vector>
#include <initializer_list>
#include <compare>

class Command
{
private:
    char name;

public:
    Command();

    virtual void execute(Position &position, std::vector<Sensor> &sensors) const;

    std::strong_ordering operator <=> (const Command & other) const;

    void set_name(char _name);
};

class ComposedCommand : public Command
{
private:
    std::vector<Command> components;

public:

    ComposedCommand(std::initializer_list<Command> _components)
        : components(_components) {};

    void execute(Position &position, std::vector<Sensor> &sensors) const override;
};


class RotateCommand : public Command
{

};

class RotateRight : public RotateCommand
{

};

class RotateLeft : public RotateCommand
{

};


class MoveCommand : public Command
{

};

class MoveForward : public MoveCommand
{

};

class MoveBackward : public MoveCommand
{

};

#endif