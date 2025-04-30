#include <cassert>
#include "../gamelogic.h"

int main() {
    GameLogic game;

    game.makeMove(0, 0, 'X');
    game.makeMove(0, 1, 'X');
    game.makeMove(0, 2, 'X');

    assert(game.checkWin('X') == true);

    return 0;
}
