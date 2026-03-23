#include "Cell.hpp"
#include "Color.hpp"
#include "Exceptions.hpp"
#include "FTTTBoard.hpp"
#include <FTTT.hpp>
#include <cstdlib>
#include <exception>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <optional>
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
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+\n";
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
        std::cout << color << symbol << COLOR_RESET << "'s TURN\n";
        std::cout << "You have " << total << "% of " << symbol << " to put down.\n";
    enter_row:
        std::cout << "Row number [0-2]: ";
        std::cin >> row;
        if (std::cin.eof())
            std::exit(0);
        if (row < 0 || row >= FTTTBoard::BOARD_SIZE)
        {
            std::cout << "Invalid row number!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_row;
        }
    enter_col:
        std::cout << "Col number [0-2]: ";
        std::cin >> col;
        if (std::cin.eof())
            std::exit(0);
        if (col < 0 || col >= FTTTBoard::BOARD_SIZE)
        {
            std::cout << "Invalid col number!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_col;
        }
        this->print(col, row);
    enter_percentage:
        std::string percentage_input;
        std::cout << '\n'
                  << "Fast Options\n"
                  << "[a]ll\n"
                  << "[h]alf\n"
                  << "[q]uarter\n"
                  << "[c]omplete to\n";
        std::cout << "Enter percentage: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, percentage_input);
        mpq_class current_val;
        const Cell& current_cell = m_board.get_board().at(row).at(col);

        if (m_xturn)
        {
            current_val = current_cell.get_Xval();
        }
        else
        {
            current_val = current_cell.get_Oval();
        }
        int current_val_int = static_cast<int>((100 * current_val.get_num().get_si()) / current_val.get_den().get_si());

        std::optional<int> percentage = this->parse_percentage(percentage_input, total, current_val_int);
        perc = percentage.value_or(-1);

        if (std::cin.eof())
            std::exit(0);
        if (perc < 1 || perc > total)
        {
            std::cout << "Invalid percentage!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            goto enter_percentage;
        }
    confirm:
        std::cout << "Confirm putting " << perc << "% " << symbol << " to the highlighted cell? [Y/n]: ";
        std::string yn;
        std::cin >> yn;
        if (std::cin.eof())
            std::exit(0);
        if (yn.starts_with('n') || yn.starts_with('N'))
        {
            std::cout << '\n';
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
                std::cout << "Cell is already occupied!\n";
            }
            catch (const InvalidCellStateException& ex)
            {
                std::cout << "Placement is invalid!\n";
            }
        }
        else
        {
            std::cout << "Invalid selection!\n";
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

    std::cout << "GAME CONFIGURATION\n";
    std::cout << "Lower bound to capture: " << m_config.capture_low_bound << "%\n";
    std::cout << "Decay: " << m_config.decay << "\n";
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

    if (m_board.check_winner() == CellState::EMPTY)
    {
        std::cout << "Game Ended! Finalizing...\n";
        this->m_board.finalize();
    }
    this->print();

    CellState winner = m_board.check_winner();
    if (winner == CellState::X_CAPTURED)
    {
        std::cout << "X WON!\n";
    }
    else if (winner == CellState::O_CAPTURED)
    {
        std::cout << "O WON!\n";
    }
    else
    {
        std::cout << "TIE\n";
    }
}

void FTTTGame::print_row(const FTTTBoard::row_t& row, int highlight_col)
{
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+";
    std::cout << std::string(CELL_PRINT_SIZE, '-');
    std::cout << "+\n";
    for (std::uint8_t j = 0; j < CELL_PRINT_SIZE; ++j)
    {
        std::cout << "|";
        print_cell_inner(row.at(0), j, highlight_col == 0);
        std::cout << "|";
        print_cell_inner(row.at(1), j, highlight_col == 1);
        std::cout << "|";
        print_cell_inner(row.at(2), j, highlight_col == 2);
        std::cout << "|\n";
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
        std::cout << highlight << std::string(CELL_PRINT_SIZE, ' ') << COLOR_RESET;
        break;
    }
    case 1: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::cout << highlight << ' ' << COLOR_RED << x << "% " << COLOR_RESET;
            break;
        }
        case CellState::X_CAPTURED: {
            std::cout << highlight << COLOR_RED << " \\ / " << COLOR_RESET;
            break;
        }
        case CellState::O_CAPTURED: {
            std::cout << highlight << COLOR_BLUE << " /-\\ " << COLOR_RESET;
            break;
        }
        }
        break;
    }
    case 2: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::cout << highlight << std::string(CELL_PRINT_SIZE, ' ') << COLOR_RESET;
            break;
        }
        case CellState::X_CAPTURED: {
            std::cout << highlight << COLOR_RED << "  /  " << COLOR_RESET;
            break;
        }
        case CellState::O_CAPTURED: {
            std::cout << highlight << COLOR_BLUE << " | | " << COLOR_RESET;
            break;
        }
        }
        break;
    }
    case 3: {
        switch (cell.get_cell_state())
        {
        case CellState::EMPTY: {
            std::cout << highlight << ' ' << COLOR_BLUE << o << "% " << COLOR_RESET;
            break;
        }
        case CellState::X_CAPTURED: {
            std::cout << highlight << COLOR_RED << " / \\ " << COLOR_RESET;
            break;
        }
        case CellState::O_CAPTURED: {
            std::cout << highlight << COLOR_BLUE << " \\-/ " << COLOR_RESET;
            break;
        }
        }
        break;
    }
    }
}

std::optional<int> FTTTGame::parse_percentage(const std::string& percentage_input, int remaining, int current_val)
{
    static constexpr auto strip_space = [](const std::string& s) -> std::string {
        std::size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) { ++start; }
        if (start == s.size())
            return ""; // string is all whitespace
        std::size_t end = s.size() - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(s[end]))) { --end; }
        return s.substr(start, end - start + 1);
    };
    std::string stripped = strip_space(percentage_input);

    if (stripped == "a")
    {
        return remaining;
    }
    else if (stripped == "h")
    {
        return remaining / 2;
    }
    else if (stripped == "q")
    {
        return remaining / 2;
    }
    else if (stripped.starts_with('c'))
    {
        stripped = strip_space(stripped.substr(1));
        try
        {
            int target = std::stoi(stripped);
            int delta = target - current_val;
            if (delta > remaining || delta < 1)
                throw std::out_of_range{""};
            return delta;
        }
        catch (const std::exception& e)
        {
            return std::nullopt;
        }
    }
    else
    {
        try
        {
            return std::stoi(stripped);
        }
        catch (const std::exception& e)
        {
            return std::nullopt;
        }
    }

    return std::nullopt;
}

} // namespace fttt
