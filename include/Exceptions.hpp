#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__
#include <exception>

class InvalidCellStateException : std::exception
{
};

class CellIsAlreadyOccupiedException : std::exception
{
};

#endif // __EXCEPTIONS_HPP__
