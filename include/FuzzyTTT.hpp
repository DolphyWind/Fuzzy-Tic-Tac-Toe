#ifndef __FUZZY_TTT_INCLUDE_GUARD__
#define __FUZZY_TTT_INCLUDE_GUARD__
#include <array>
#include <cstdint>
#include <Cell.hpp>

namespace fttt
{

class FTTTBoard
{
public:
    FTTTBoard();

    void place(std::uint8_t x, std::uint8_t y, bool is_x, const mpf_class& value);

private:
    constexpr static std::uint8_t BOARD_SIZE = 3;

    std::array<std::array<Cell, BOARD_SIZE>, BOARD_SIZE> m_board;
};

} // namespace fttt

#endif // !__FUZZY_TTT_INCLUDE_GUARD__
