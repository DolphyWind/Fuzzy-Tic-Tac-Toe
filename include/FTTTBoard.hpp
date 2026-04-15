#ifndef __FTTTBOARD_HPP__
#define __FTTTBOARD_HPP__
#include <array>
#include <cstdint>
#include <Cell.hpp>
#include <gmpxx.h>

namespace fttt
{

template<std::size_t N>
class FTTTBoard
{
public:
    constexpr static std::size_t BOARD_SIZE = N;
    typedef std::array<Cell, N> row_t;
    typedef std::array<row_t, N> board_t;

    FTTTBoard(const mpq_class& capture_low_bound=mpq_class{1, 2});

    void place(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value);
    board_t& get_board();
    const board_t& get_board() const;
    bool is_valid_placement(std::uint8_t x, std::uint8_t y, bool is_x, const mpq_class& value) const;
private:
    board_t m_board;
};

} // namespace fttt

#include "FTTTBoard.tpp"
#endif // !__FTTTBOARD_HPP__
