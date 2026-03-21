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

    void print(int highlight_y=-1, int highlight_x=-1);
    void input();
    void main_loop();
private:
    static constexpr std::uint8_t BOARD_SIZE = 3;
    static constexpr std::uint8_t CELL_PRINT_SIZE = 5;
    FTTTBoard<BOARD_SIZE> m_board;
    bool m_xturn;

    void print_row(const FTTTBoard<BOARD_SIZE>::row_t& row, int highlight_col=-1);
    void print_cell_inner(const Cell& cell, std::uint8_t row, bool highlighted=false);
};

} // namespace fttt

#endif // !__FTTT_HPP__
