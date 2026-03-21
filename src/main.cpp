#include "FTTT.hpp"
#include "args.hxx"
#include <iostream>
#include <print>

int main(int argc, char** argv)
{
    args::ArgumentParser parser("Fuzzy Tic-Tac-Toe Implementation");
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
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
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
    std::println("{}", percentage);

    fttt::FTTTGame fttt_game{percentage, decay};
    fttt_game.main_loop();
    return 0;
}
