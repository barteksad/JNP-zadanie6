#ifndef ROVER_H
#define ROVER_H

#include "sensor.h"
#include "command.h"
#include "rover_exceptions.h"


#include <string>

class Rover
{
private:
    Position position;
public:
    void land(Position _position);
    void execute(const std::string & s);
};

class RoverBuilder
{
public:
    void program_command(char name, Command& command);
    void program_command(std::unique_ptr<Sensor> sensor);
    Rover build();
};

// tak chyba będzie ok
Command& move_forward()
{
    static MoveForward command;

    return command;
}

#endif