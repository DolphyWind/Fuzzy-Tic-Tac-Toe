#include <Cell.hpp>
#include <Exceptions.hpp>
#include <FuzzyTTT.hpp>
#include <gmpxx.h>

namespace fttt
{
FTTTBoard::FTTTBoard() {}

void FTTTBoard::place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value)
{
    Cell& current_cell = m_board.at(y).at(x);
    if (current_cell.get_cell_state() != CellState::EMPTY)
    {
        throw CellIsAlreadyOccupiedException();
    }

    mpf_class x_value = current_cell.get_Xval();
    mpf_class o_value = current_cell.get_Oval();
    if(is_x) x_value += value;
    else o_value += value;
    current_cell.set_cell(x_value, o_value);
}

} // namespace fttt
