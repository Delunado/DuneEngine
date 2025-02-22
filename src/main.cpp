#include <iostream>

#include "SDL.h"
#include "Game.h"

int main(int argc, char *argv[]) {
    Game game;

    game.Initialize();
    game.Run();
    game.Clean();

    return 0;
}
