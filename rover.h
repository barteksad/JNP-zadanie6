#ifndef ROVER_H
#define ROVER_H

#include "sensor.h"
#include "command.h"
#include "position.h"
#include "rover_exceptions.h"

#include <iostream>
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

    void land(std::pair<coordinate_t, coordinate_t> _position, Direction _direction)
    {
        if(has_landed)
            throw RoverHasAlreadyLanded();
            
        position.set(_position, _direction);
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

    friend std::ostream& operator<<(std::ostream& os, const Rover& rover);
    friend class RoverBuilder;
};

std::ostream& operator<<(std::ostream& os, const Rover& rover)
{
    os << rover.position;
    return os;
}


class Rover::RoverBuilder
{
private:
    std::map<char, Command> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:

    Rover::RoverBuilder& program_command(char name, const Command& command)
    {
        commands.insert({name, command});
        return *this;
    }

    Rover::RoverBuilder& add_sensor(std::unique_ptr<Sensor> &&sensor)
    {
        sensors.emplace_back(std::move(sensor));
        return *this;
    }

    Rover build()
    {
        return Rover{commands, sensors};
    }
};

Rover::RoverBuilder RoverBuilder()
{
    Rover::RoverBuilder builder;
    return builder;
}


#endif