#include "FTTTBase.hpp"
#include "Cell.hpp"
#include <array>
#include <limits>
#include <vector>

namespace fttt
{

FTTTBase::FTTTBase(const GameConfig& config) : m_config(config), m_board(config.capture_low_bound)
{
    if (config.decay < 0)
    {
        throw std::range_error{"Invalid decay value!"};
    }
    if (config.capture_low_bound < 0 || config.capture_low_bound > 100)
    {
        throw std::range_error{"Invalid percentage value!"};
    }
}

void FTTTBase::apply_decay()
{
    if (m_config.decay == 0)
        return;
    constexpr auto mpq_max = [](const mpq_class& a, const mpq_class& b) -> mpq_class {
        if (a < b)
            return b;
        return a;
    };

    for (auto& row : m_board.get_board())
    {
        for (auto& cell : row)
        {
            if (cell.get_cell_state() == CellState::EMPTY)
            {
                mpq_class x_val = cell.get_Xval();
                mpq_class o_val = cell.get_Oval();
                x_val = mpq_max(mpq_class{0}, x_val - m_config.decay);
                o_val = mpq_max(mpq_class{0}, o_val - m_config.decay);
                cell.set_cell(x_val, o_val);
            }
        }
    }
}

CellState FTTTBase::check_winner() const
{
    using U = std::underlying_type_t<CellState>;
    using Line = std::vector<const Cell*>;
    std::vector<Line> lines;
    const decltype(m_board)::board_t& board = m_board.get_board();
    std::size_t board_size = board.size();

    for (std::size_t i = 0; i < board_size; ++i)
    {
        Line row;
        Line column;
        row.reserve(board_size);
        column.reserve(board_size);
        for (std::size_t j = 0; j < board_size; ++j)
        {
            row.push_back(&m_board.get_board().at(i).at(j));
            column.push_back(&m_board.get_board().at(j).at(i));
        }
        lines.push_back(std::move(row));
        lines.push_back(std::move(column));
    }
    Line diagonal1;
    Line diagonal2;
    diagonal1.reserve(board_size);
    diagonal2.reserve(board_size);
    for (std::size_t i = 0, j = 0; i < board_size; ++i, ++j)
    {
        diagonal1.push_back(&m_board.get_board().at(i).at(j));
        diagonal2.push_back(&m_board.get_board().at(i).at(board_size - 1 - j));
    }
    lines.push_back(std::move(diagonal1));
    lines.push_back(std::move(diagonal2));

    for (const auto& l : lines)
    {
        U state_underlying = std::numeric_limits<U>::max();
        for (const auto& c : l)
        {
            state_underlying &= static_cast<U>(c->get_cell_state());
        }

        CellState state = static_cast<CellState>(state_underlying);
        if (state != CellState::EMPTY)
            return state;
    }

    return CellState::EMPTY;
}

bool FTTTBase::has_moves() const
{
    for (const auto& row : m_board.get_board())
    {
        for (const auto& cell : row)
        {
            if (cell.get_cell_state() == CellState::EMPTY && (cell.get_Xval() + cell.get_Oval()) < mpq_class{1})
            {
                return true;
            }
        }
    }
    return false;
}

bool FTTTBase::game_ended() const { return (!has_moves()) || (check_winner() != CellState::EMPTY); }

void FTTTBase::finalize()
{
    for (auto& row : m_board.get_board())
    {
        for (auto& col : row)
        {
            if (col.get_cell_state() == CellState::EMPTY)
            {
                if (col.get_Xval() > col.get_Oval())
                    col.set_cell(mpq_class{1}, mpq_class{0});
                else if (col.get_Oval() > col.get_Xval())
                    col.set_cell(mpq_class{0}, mpq_class{1});
            }
        }
    }
}

} // namespace fttt
