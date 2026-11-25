// Ballroom of the Dead - a text-based zombie survival RPG
#include <cstdlib>
#include <ctime>
#include "Game.h"

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    game.Run();
    return 0;
}
