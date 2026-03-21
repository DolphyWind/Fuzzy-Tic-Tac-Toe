#ifndef __FTTTBOARD_CPP__
#define __FTTTBOARD_CPP__

#include "FTTTBoard.hpp"
#include "Cell.hpp"
#include <gmpxx.h>
#include <stdexcept>
#include <vector>

namespace fttt
{
FTTTBoard::FTTTBoard(int capture_low_bound)
{
    if (capture_low_bound < 0 || capture_low_bound > 100)
    {
        throw std::range_error{"Invalid percentage value!"};
    }
    for (auto& row : m_board)
    {
        for (auto& col : row) { col = Cell(mpq_class(capture_low_bound, 100)); }
    }
}

void FTTTBoard::place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value)
{
    Cell& current_cell = m_board.at(y).at(x);
    mpf_class x_value = current_cell.get_Xval();
    mpf_class o_value = current_cell.get_Oval();
    if (is_x)
        x_value += value;
    else
        o_value += value;
    current_cell.set_cell(x_value, o_value);
}

FTTTBoard::board_t& FTTTBoard::get_board() { return m_board; }

const FTTTBoard::board_t& FTTTBoard::get_board() const { return m_board; }

CellState FTTTBoard::check_winner() const
{
    using U = std::underlying_type_t<CellState>;
    using Triplet = std::array<const Cell* const, 3>;
    std::vector<Triplet> triplets{
        {&m_board[0][0], &m_board[1][0], &m_board[1][0]},
        {&m_board[0][1], &m_board[1][1], &m_board[2][1]},
        {&m_board[0][2], &m_board[1][2], &m_board[2][2]},
        {&m_board[0][0], &m_board[0][1], &m_board[0][2]},
        {&m_board[1][0], &m_board[1][1], &m_board[1][2]},
        {&m_board[2][0], &m_board[2][1], &m_board[2][2]},
        {&m_board[0][0], &m_board[1][1], &m_board[2][2]},
        {&m_board[2][0], &m_board[1][1], &m_board[0][2]},
    };

    for (const auto& t : triplets)
    {
        const auto [c1, c2, c3] = t;
        CellState state =
            static_cast<CellState>(static_cast<U>(c1->get_cell_state()) & static_cast<U>(c2->get_cell_state()) &
                                   static_cast<U>(c3->get_cell_state()));
        if (state != CellState::EMPTY)
            return state;
    }

    return CellState::EMPTY;
}

bool FTTTBoard::has_moves() const
{
    for (const auto& row : m_board)
    {
        for (const auto& cell : row)
        {
            if (cell.get_cell_state() == CellState::EMPTY && (cell.get_Xval() + cell.get_Oval()) < 1)
            {
                return true;
            }
        }
    }
    return false;
}

bool FTTTBoard::game_ended() const
{
    return (!has_moves()) || (check_winner() != CellState::EMPTY);
}

} // namespace fttt
#endif // !__FTTTBOARD_CPP__
