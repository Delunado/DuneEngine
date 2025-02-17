#include <iostream>

#include "Game.h"
#include "SDL.h"

int main(int argc, char *argv[]) {
    Game game;

    game.Initialize();
    game.Run();
    game.Clean();

    return 0;
}
