﻿#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <glm.hpp>
#include <memory>

class Entity;
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

    std::unique_ptr<Registry> _registry;

    bool _isRunning;
    int millisecondsPreviousFrame = 0;

    int _windowWidth = 800;
    int _windowHeight = 600;

    void Setup();

    void ProcessInput();

    void Update();

    void Render();
};

#endif //GAME_H
