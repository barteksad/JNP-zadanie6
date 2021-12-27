#ifndef ROVER_EXCEPTIONS_H
#define ROVER_EXCEPTIONS_H

#include <exception>

class BrokenRover : public std::exception
{

};

class UnknownCommand : public std::exception
{

};

#endif