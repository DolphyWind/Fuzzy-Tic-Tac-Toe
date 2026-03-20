#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#ifdef _WIN32
#define COLOR_RED ""
#define COLOR_BLUE ""
#define COLOR_RESET ""
#else
#define COLOR_RED "\033[31m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#endif


#endif // !__COLOR_HPP__
