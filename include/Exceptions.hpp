#ifndef __EXCEPTIONS_INCLUDE_GUARD__
#define __EXCEPTIONS_INCLUDE_GUARD__
#include <exception>

class InvalidCellStateException : std::exception
{
};

class CellIsAlreadyOccupiedException : std::exception
{
};

#endif // __EXCEPTIONS_INCLUDE_GUARD__
