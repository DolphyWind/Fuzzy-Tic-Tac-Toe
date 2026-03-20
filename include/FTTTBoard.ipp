#ifndef __FTTTBOARD_IPP__
#define __FTTTBOARD_IPP__

#include "Cell.hpp"
#include "Exceptions.hpp"
#include "FTTTBoard.hpp"
#include <cstddef>
#include <gmpxx.h>

namespace fttt
{
template<std::size_t N>
FTTTBoard<N>::FTTTBoard() {}

template<std::size_t N>
void FTTTBoard<N>::place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value)
{
    Cell& current_cell = m_board.at(y).at(x);
    if (current_cell.get_cell_state() != CellState::EMPTY)
    {
        throw CellIsAlreadyOccupiedException();
    }

    mpf_class x_value = current_cell.get_Xval();
    mpf_class o_value = current_cell.get_Oval();
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

} // namespace fttt
#endif // !__FTTTBOARD_IPP__
