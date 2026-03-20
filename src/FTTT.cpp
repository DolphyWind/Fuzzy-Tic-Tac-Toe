#include <FTTT.hpp>
#include <gmp.h>
#include <gmpxx.h>
#include <print>
#include <string>
#include "Color.hpp"

namespace fttt
{

FTTTGame::FTTTGame() {}

void FTTTGame::print()
{
    const auto& board = m_board.get_board();
    for (std::uint8_t i = 0; i < BOARD_SIZE; ++i)
    {
        const auto& row = board.at(i);
        print_row(row);
    }
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::println("+");
}

void FTTTGame::input() {}

void FTTTGame::main_loop()
{
    do
    {
        this->print();
        this->input();
    } while (false);
}

void FTTTGame::print_row(const FTTTBoard<BOARD_SIZE>::row_t& row)
{
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::println("+");
    for (std::uint8_t j = 0; j < CELL_PRINT_SIZE; ++j)
    {
        std::print("|");
        print_cell_inner(row.at(0), j);
        std::print("|");
        print_cell_inner(row.at(1), j);
        std::print("|");
        print_cell_inner(row.at(2), j);
        std::println("|");
    }
}

void FTTTGame::print_cell_inner(const Cell& cell, std::uint8_t row)
{
    const mpf_class& x_val = cell.get_Xval();
    const mpf_class& o_val = cell.get_Oval();

    mp_exp_t exponent;
    std::string x = x_val.get_str(exponent).substr(0, 2);
    if (x.size() < 2)
        x = std::string(2 - x.size(), '0') + x;
    std::string o = o_val.get_str(exponent).substr(0, 2);
    if (o.size() < 2)
        o = std::string(2 - o.size(), '0') + o;
    switch (row)
    {
    case 0:
    case 4: {
        std::print("{}", std::string(CELL_PRINT_SIZE, ' '));
        break;
    }
    case 1: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{} {}% {}", COLOR_RED, x, COLOR_RESET);
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{} \\ / {}", COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{} /-\\ {}", COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    case 2: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{}", std::string(CELL_PRINT_SIZE, ' '));
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{}  /  {}", COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{} | | {}", COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    case 3: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{} {}% {}", COLOR_BLUE, o, COLOR_RESET);
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{} / \\ {}", COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{} \\-/ {}", COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    }
}

} // namespace fttt
