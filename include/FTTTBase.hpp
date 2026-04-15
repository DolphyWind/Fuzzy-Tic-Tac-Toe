#ifndef __FTTT_BASE_HPP__
#define __FTTT_BASE_HPP__

#include "Cell.hpp"
#include "GameConfig.hpp"
#include "FTTTBoard.hpp"

namespace fttt
{

class FTTTBase
{
public:
    FTTTBase(const GameConfig& config);
    virtual ~FTTTBase() = default;

    virtual void main_loop() = 0;
    virtual void apply_decay();

    CellState check_winner() const;
    bool has_moves() const;
    bool game_ended() const;
    void finalize();
protected:
    GameConfig m_config;
    FTTTBoard<3> m_board;
};

}

#endif // !__FTTT_BASE_HPP__
