#ifndef __FTTTBOARD_HPP__
#define __FTTTBOARD_HPP__
#include <array>
#include <cstdint>
#include <Cell.hpp>
#include <gmpxx.h>

namespace fttt
{

class FTTTBoard
{
public:
    static constexpr std::uint8_t BOARD_SIZE = 3;
    typedef std::array<Cell, BOARD_SIZE> row_t;
    typedef std::array<row_t, BOARD_SIZE> board_t;
    FTTTBoard(const mpq_class& capture_low_bound=mpq_class{1, 2});

    void place(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value);
    board_t& get_board();
    const board_t& get_board() const;
    bool is_valid_placement(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value) const;
private:
    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> m_board;
};

} // namespace fttt

#endif // !__FTTTBOARD_HPP__
