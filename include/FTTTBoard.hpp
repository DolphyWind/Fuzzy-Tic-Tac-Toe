#ifndef __FTTTBOARD_HPP__
#define __FTTTBOARD_HPP__
#include <array>
#include <cstdint>
#include <Cell.hpp>

namespace fttt
{

template<std::size_t N>
class FTTTBoard
{
public:
    typedef std::array<Cell, N> row_t;
    typedef std::array<row_t, N> board_t;
    FTTTBoard();

    void place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value);
    board_t& get_board();
    const board_t& get_board() const;
    CellState check_winner();
private:
    std::array<std::array<Cell, N>, N> m_board;
};

} // namespace fttt

#include "FTTTBoard.ipp"
#endif // !__FTTTBOARD_HPP__
