# Fuzzy Tic-Tac-Toe

Fuzzy Tic-Tac-Toe, FTTT for short, is a variant of the classic Tic-Tac-Toe game. Instead of putting one symbol each turn, in FTTT, you get to distribute your one symbol however you like. 

Each cell has capacity of `1`, so for example you can put at most `0.4` X to a cell that already has `0.6` O. If a player puts an amount of symbols that is greater than a lower bound value `l` to a cell, that player captures it. This `l` value is currently experimental and there is not standardization.
However it should be noted that, at the end of the game if there are no valid moves and no one wins, the game gets "finalized". Which makes the player with the most amount of symbols to own an unoccupied cell regardless of the value of lower bound.

There is also an optional setting named decay. If set, after each turn, every single symbol in a cell decays by the specified amount. This has been implement to reduce ties and punish distributing a lot.

I myself prefer a lower bound of `0.66` and a decay of `0.01`.

I kept it simple and made this version of the game only support two digits after the decimal point (Well, also for being able to print cell's value nicely). However, the backend (the FTTTBoard class) supports arbitrary rational values via GNU GMP library, so you can freely take it, plug it into something else and have any precision you like.
