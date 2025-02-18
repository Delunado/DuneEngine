#ifndef GAME_H
#define GAME_H
#include <SDL.h>

class Game {
public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void Clean();

private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;

    bool _isRunning;

    void ProcessInput();

    void Update();

    void Render();
};

#endif //GAME_H
