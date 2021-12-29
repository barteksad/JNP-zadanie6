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

    Rover(std::map<char, Command>& _commands, std::vector<std::unique_ptr<Sensor>>& _sensors)
    : commands(_commands), sensors(_sensors) {};

public:
    Rover()=delete;
    Rover(const Rover&)=delete;
    Rover(Rover&&)=delete;

    template<class... T>
    void land(T... args)
    {
        position.set(args);
    }

    void execute(const std::string & s)
    {
        for(char c : s)
        {
            auto it = commands.find(c);
            if(it == commands.end())
                throw UnknownCommand();
            else
                it->second.execute(position, sensors);
        }

    }

    friend class RoverBuilder;
};

class RoverBuilder
{
private:
    std::map<char, Command> commands;
    std::vector<std::unique_ptr<Sensor>> sensors;

public:
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
        return Rover(commands, sensors);
    }
};

// tak chyba będzie ok
Command& move_forward()
{
    static MoveForward command;

    return command;
}

#endif