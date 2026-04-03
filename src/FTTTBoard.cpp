#ifndef __FTTTBOARD_CPP__
#define __FTTTBOARD_CPP__

#include "FTTTBoard.hpp"
#include "Cell.hpp"
#include <gmpxx.h>

namespace fttt
{
FTTTBoard::FTTTBoard(const mpq_class& capture_low_bound)
{
    for (auto& row : m_board)
    {
        for (auto& col : row) { col = Cell(capture_low_bound); }
    }
}

void FTTTBoard::place(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value)
{
    Cell& current_cell = m_board.at(y).at(x);
    mpq_class x_value = current_cell.get_Xval();
    mpq_class o_value = current_cell.get_Oval();
    if (is_x)
        x_value += value;
    else
        o_value += value;
    current_cell.set_cell(x_value, o_value);
}

FTTTBoard::board_t& FTTTBoard::get_board() { return m_board; }

const FTTTBoard::board_t& FTTTBoard::get_board() const { return m_board; }

bool FTTTBoard::is_valid_placement(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value) const
{
    const Cell& cell = m_board[y][x];
    if (cell.get_cell_state() != CellState::EMPTY)
        return false;

    const mpq_class& current_value = is_x ? cell.get_Xval() : cell.get_Oval();
    if (current_value + value > mpq_class{1}) return false;
    return true;
}

} // namespace fttt
#endif // !__FTTTBOARD_CPP__
