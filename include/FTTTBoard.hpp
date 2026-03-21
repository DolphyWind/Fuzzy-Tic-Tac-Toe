#ifndef __FTTTBOARD_HPP__
#define __FTTTBOARD_HPP__
#include <array>
#include <cstdint>
#include <Cell.hpp>

namespace fttt
{

class FTTTBoard
{
public:
    static constexpr std::uint8_t BOARD_SIZE = 3;
    typedef std::array<Cell, BOARD_SIZE> row_t;
    typedef std::array<row_t, BOARD_SIZE> board_t;
    FTTTBoard(int capture_low_bound=50);

    void place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value);
    board_t& get_board();
    const board_t& get_board() const;
    CellState check_winner() const;
    bool has_moves() const;
private:
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> m_board;
};

} // namespace fttt

#endif // !__FTTTBOARD_HPP__
