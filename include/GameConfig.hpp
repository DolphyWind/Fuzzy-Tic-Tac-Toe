#ifndef __GAME_CONFIG_HPP__
#define __GAME_CONFIG_HPP__

#include <gmpxx.h>

namespace fttt
{

struct GameConfig
{
    mpq_class capture_low_bound = 50;
    mpq_class decay = 0;
};

} // namespace fttt

#endif // !__GAME_CONFIG_HPP__
