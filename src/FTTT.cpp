#include "Cell.hpp"
#include "Color.hpp"
#include "Exceptions.hpp"
#include "FTTTBoard.hpp"
#include <FTTT.hpp>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>

namespace fttt
{

FTTTGame::FTTTGame(const GameConfig& config)
    : m_config(config), m_board(mpq_class(config.capture_low_bound, 100)), m_xturn(true)
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

void FTTTGame::print(int highlight_x, int highlight_y)
{
    const auto& board = m_board.get_board();
    for (std::uint8_t i = 0; i < FTTTBoard::BOARD_SIZE; ++i)
    {
        const auto& row = board.at(i);
        int high_col = -1;
        if (highlight_y == i)
            high_col = highlight_x;
        print_row(row, high_col);
    }
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::print("+");
    std::print("{}", std::string(CELL_PRINT_SIZE, '-'));
    std::println("+");
}

void FTTTGame::input()
{
    int total = 100;
    int perc = 0;
    int row = 0;
    int col = 0;

    char symbol = 'X';
    std::string color = COLOR_RED;
    if (!m_xturn)
    {
        symbol = 'O';
        color = COLOR_BLUE;
    }
    while (total > 0)
    {
        this->print();
        std::println("{}{}{}'s TURN", color, symbol, COLOR_RESET);
        std::println("You have {}% of {} to put down.", total, symbol);
    enter_percentage:
        std::print("Enter a percentage as integer: ");
        std::cin >> perc;
        if (perc < 1 || perc > total)
        {
            std::println("Invalid percentage!");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_percentage;
        }
    enter_row:
        std::print("Row number [0-2]: ");
        std::cin >> row;
        if (row < 0 || row >= FTTTBoard::BOARD_SIZE)
        {
            std::println("Invalid row number!");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_row;
        }
    enter_col:
        std::print("Col number [0-2]: ");
        std::cin >> col;
        if (col < 0 || col >= FTTTBoard::BOARD_SIZE)
        {
            std::println("Invalid col number!");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_col;
        }
        this->print(col, row);
    confirm:
        std::print("Confirm putting {}% {} to the highlighted cell? [Y/n]: ", perc, symbol);
        std::string yn;
        std::cin >> yn;
        if (yn.starts_with('n') || yn.starts_with('N'))
        {
            std::println();
            continue;
        }
        else if (yn.starts_with('y') || yn.starts_with('Y'))
        {
            try
            {
                m_board.place(col, row, m_xturn, mpq_class(perc, 100));
                total -= perc;
                if (m_board.game_ended())
                {
                    break;
                }
            }
            catch (const CellIsAlreadyOccupiedException& ex)
            {
                std::println("Cell is already occupied!");
            }
            catch (const InvalidCellStateException& ex)
            {
                std::println("Placement is invalid!");
            }
        }
        else
        {
            std::println("Invalid selection!");
            goto confirm;
        }
    }
    m_xturn = !m_xturn;
}

void FTTTGame::main_loop()
{
    auto mpf_max = [](const mpq_class& a, const mpq_class& b) -> mpq_class {
        if (a < b)
            return b;
        return a;
    };

    std::println("GAME CONFIGURATION");
    std::println("Lower bound to capture: {}%", m_config.capture_low_bound);
    std::println("Decay: {}", m_config.decay);
    mpq_class decay_value = mpq_class(m_config.decay, 100);

    do
    {
        this->input();
        if (m_board.game_ended())
            break;
        if (m_config.decay != 0)
        {
            for (auto& row : m_board.get_board())
            {
                for (auto& cell : row)
                {
                    if (cell.get_cell_state() == CellState::EMPTY)
                    {
                        mpq_class x_val = cell.get_Xval();
                        mpq_class o_val = cell.get_Oval();
                        x_val = mpf_max(mpq_class{0}, x_val - decay_value);
                        o_val = mpf_max(mpq_class{0}, o_val - decay_value);
                        cell.set_cell(x_val, o_val);
                    }
                }
            }
        }
    } while (true);

    std::println("Game Ended! Finalizing...");
    this->m_board.finalize();
    this->print();
    CellState winner = m_board.check_winner();
    if (winner == CellState::X_CAPTURED)
    {
        std::println("X WON!");
    }
    else if (winner == CellState::O_CAPTURED)
    {
        std::println("O WON!");
    }
    else
    {
        std::println("TIE");
    }
}

void FTTTGame::print_row(const FTTTBoard::row_t& row, int highlight_col)
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
        print_cell_inner(row.at(0), j, highlight_col == 0);
        std::print("|");
        print_cell_inner(row.at(1), j, highlight_col == 1);
        std::print("|");
        print_cell_inner(row.at(2), j, highlight_col == 2);
        std::println("|");
    }
}

void FTTTGame::print_cell_inner(const Cell& cell, std::uint8_t row, bool highlighted)
{
    const mpq_class& x_val = cell.get_Xval();
    const mpq_class& o_val = cell.get_Oval();
    std::string highlight = (highlighted ? COLOR_HIGHLIGHT : "");

    std::ostringstream x_oss, o_oss;
    x_oss << x_val * 100;
    o_oss << o_val * 100;
    std::string x = x_oss.str().substr(0, 2);
    std::string o = o_oss.str().substr(0, 2);
    if (x.size() < 2)
        x = std::string(2 - x.size(), '0') + x;
    if (o.size() < 2)
        o = std::string(2 - o.size(), '0') + o;
    switch (row)
    {
    case 0:
    case 4: {
        std::print("{}{}{}", highlight, std::string(CELL_PRINT_SIZE, ' '), COLOR_RESET);
        break;
    }
    case 1: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{}{} {}% {}", highlight, COLOR_RED, x, COLOR_RESET);
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{}{} \\ / {}", highlight, COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{}{} /-\\ {}", highlight, COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    case 2: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{}{}{}", highlight, std::string(CELL_PRINT_SIZE, ' '), COLOR_RESET);
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{}{}  /  {}", highlight, COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{}{} | | {}", highlight, COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    case 3: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::print("{}{} {}% {}", highlight, COLOR_BLUE, o, COLOR_RESET);
            break;
        }
        case CellState::X_CAPTURED: {
            std::print("{}{} / \\ {}", highlight, COLOR_RED, COLOR_RESET);
            break;
        }
        case CellState::O_CAPTURED: {
            std::print("{}{} \\-/ {}", highlight, COLOR_BLUE, COLOR_RESET);
            break;
        }
        }
        break;
    }
    }
}

} // namespace fttt
