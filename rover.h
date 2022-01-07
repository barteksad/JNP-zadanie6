#ifndef ROVER_H
#define ROVER_H

#include "sensor.h"
#include "command.h"
#include "rover_exceptions.h"

#include <string>
#include <memory>
#include <map>

class Rover
{
private:
    Position position;
    std::map<char, Command> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;
    bool is_broken;
    bool has_landed;

    Rover(std::map<char, Command>& _commands, std::vector<std::unique_ptr<Sensor>>& _sensors)
    : commands(_commands), sensors(_sensors), is_broken(false), has_landed(false) {};

public:
    Rover()=delete;
    Rover(const Rover&)=delete;
    Rover(Rover&&)=delete;

    class RoverBuilder;

    template<typename... T>
    void land(T... args)
    {
        if(has_landed)
            throw RoverHasAlreadyLanded();
            
        position.set(args...);
        has_landed=true;
    }

    void execute(const std::string & s)
    {
        if(is_broken)
            return;
        if(!has_landed)
            throw RoverNotLanded();

        for(const char c : s)
        {
            auto it = commands.find(c);
            if(it == commands.end())
                throw UnknownCommand();
            else
            {
                try
                {
                    it->second.execute(position, sensors);
                }
                catch(const BrokenRover& br)
                {
                    is_broken = true;
                    return;
                }
            }
        }

    }

    friend class RoverBuilder;
};

class Rover::RoverBuilder
{
private:
    std::map<char, Command> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:

    RoverBuilder();

    void program_command(char name, Command& command)
    {
        commands.insert({name, command});
    }

    void add_sensor(std::unique_ptr<Sensor> &&sensor)
    {
        sensors.emplace_back(sensor);
    }

    Rover build()
    {
        return Rover{commands, sensors};
    }
};

Rover::RoverBuilder RoverBuilder()
{
    return Rover::RoverBuilder{};
}


Command& move_forward()
{
    static MoveForward command;

    return command;
}

Command& move_backward()
{
    static MoveBackward command;

    return command;
}

Command& rotate_right()
{
    static RotateRight command;

    return command;
}

Command& rotate_left()
{
    static RotateLeft command;

    return command;
}


#endif