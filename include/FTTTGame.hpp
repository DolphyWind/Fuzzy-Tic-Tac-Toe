#ifndef __FTTT_GAME_HPP__
#define __FTTT_GAME_HPP__

#include "Cell.hpp"
#include "FTTTBase.hpp"
#include "GameConfig.hpp"
#include "FTTTBoard.hpp"
#include <optional>

namespace fttt
{

class FTTTGame : public FTTTBase
{
public:
    FTTTGame(const GameConfig& config);

    void print(int highlight_x=-1, int highlight_y=-1);
    void input();
    virtual void main_loop() override;
private:
    static constexpr std::uint8_t CELL_PRINT_SIZE = 5;
    bool m_xturn;

    void print_row(const FTTTBoard::row_t& row, int highlight_col=-1);
    void print_cell_inner(const Cell& cell, std::uint8_t row, bool highlighted=false);
    std::optional<int> parse_percentage(const std::string& percentage_input, int remaining, int current_val);
};

} // namespace fttt

#endif // !__FTTT_GAME_HPP__
