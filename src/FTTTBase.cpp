#include "FTTTBase.hpp"
#include <array>
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
    using Triplet = std::array<const Cell* const, 3>;
    std::vector<Triplet> triplets;

    for (std::size_t i = 0; i < 3; ++i) {
        triplets.push_back({
            &m_board.get_board()[i][0],
            &m_board.get_board()[i][1],
            &m_board.get_board()[i][2],
        });
        triplets.push_back({
            &m_board.get_board()[0][i],
            &m_board.get_board()[1][i],
            &m_board.get_board()[2][i],
        });
    }
    triplets.push_back({
        &m_board.get_board()[0][0],
        &m_board.get_board()[1][1],
        &m_board.get_board()[2][2],
    });
    triplets.push_back({
        &m_board.get_board()[0][2],
        &m_board.get_board()[1][1],
        &m_board.get_board()[2][0],
    });

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
