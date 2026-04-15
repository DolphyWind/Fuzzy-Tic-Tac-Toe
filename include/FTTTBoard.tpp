#ifndef __FTTTBOARD_TPP__
#define __FTTTBOARD_TPP__

#include "FTTTBoard.hpp"
#include <gmpxx.h>

namespace fttt
{
template<std::size_t N>
FTTTBoard<N>::FTTTBoard(const mpq_class& capture_low_bound)
{
    for (auto& row : m_board)
    {
        for (auto& col : row) { col = Cell(capture_low_bound); }
    }
}

template<std::size_t N>
void FTTTBoard<N>::place(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value)
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

template<std::size_t N>
FTTTBoard<N>::board_t& FTTTBoard<N>::get_board() { return m_board; }

template<std::size_t N>
const FTTTBoard<N>::board_t& FTTTBoard<N>::get_board() const { return m_board; }

template<std::size_t N>
bool FTTTBoard<N>::is_valid_placement(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value) const
{
    const Cell& cell = m_board.at(y).at(x);
    if (cell.get_cell_state() != CellState::EMPTY)
        return false;

    const mpq_class& current_value = is_x ? cell.get_Xval() : cell.get_Oval();
    if (current_value + value > mpq_class{1}) return false;
    return true;
}

} // namespace fttt
#endif // !__FTTTBOARD_TPP__
