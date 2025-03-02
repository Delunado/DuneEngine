#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <glm.hpp>

class Registry;

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

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

    Registry *_registry;

    bool _isRunning;
    int millisecondsPreviousFrame = 0;

    int _windowWidth = 800;
    int _windowHeight = 600;

    glm::vec2 _playerPosition;
    glm::vec2 _playerVelocity;

    void Setup();

    void ProcessInput();

    void Update();

    void Render();
};

#endif //GAME_H
