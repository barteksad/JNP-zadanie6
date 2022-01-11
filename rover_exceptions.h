#ifndef ROVER_EXCEPTIONS_H
#define ROVER_EXCEPTIONS_H

#include <exception>

class BrokenRover : public std::runtime_error
{
public:
    BrokenRover()
        : std::runtime_error("Rover has broken!") {}
};

class UnknownCommand : public std::runtime_error
{
public:
    UnknownCommand()
        : std::runtime_error("Unknown command!") {}
};

class RoverNotLanded : public  std::runtime_error
{
public:
    RoverNotLanded()
        : std::runtime_error("Rover has not laned yet!") {}
};

#endif