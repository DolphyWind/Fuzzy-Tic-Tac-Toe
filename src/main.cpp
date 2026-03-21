#include "FTTT.hpp"
#include "GameConfig.hpp"
#include "args.hxx"
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
    args::ArgumentParser parser(
        "Fuzzy Tic-Tac-Toe",
        "Fuzzy Tic-Tac-Toe, FTTT for short, is a variant of the classic Tic-Tac-Toe game. Instead "
        "of putting one symbol each turn, in FTTT, you get to distribute it however you like."
    );
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<int> percetage_flag(
        parser, "percentage", "Control the minimum percentage required to claim a cell", {'p', "percentage"}, 50);
    args::ValueFlag<int> decay_flag(parser, "decay", "Decay value each turn.", {'d', "decay"}, 0);
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return EXIT_SUCCESS;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return EXIT_SUCCESS;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    int percentage = 50;
    int decay = 0;

    if (percetage_flag)
    {
        percentage = args::get(percetage_flag);
    }
    if (decay_flag)
    {
        decay = args::get(decay_flag);
    }

    try
    {
        fttt::FTTTGame fttt_game{fttt::GameConfig{.capture_low_bound = percentage, .decay = decay}};
        fttt_game.main_loop();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
