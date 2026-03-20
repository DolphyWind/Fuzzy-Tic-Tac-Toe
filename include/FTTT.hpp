#ifndef __FTTT_HPP__
#define __FTTT_HPP__

#include "Cell.hpp"
#include "FTTTBoard.hpp"

namespace fttt
{

class FTTTGame
{
public:
    FTTTGame();

    void print();
    void input();
    void main_loop();
private:
    static constexpr std::uint8_t BOARD_SIZE = 3;
    static constexpr std::uint8_t CELL_PRINT_SIZE = 5;
    FTTTBoard<BOARD_SIZE> m_board;

    void print_row(const FTTTBoard<BOARD_SIZE>::row_t& row);
    void print_cell_inner(const Cell& cell, std::uint8_t row);
};

} // namespace fttt

#endif // !__FTTT_HPP__
