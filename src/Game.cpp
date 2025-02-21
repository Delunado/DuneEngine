#include "Game.h"

#include <iostream>
#include <SDL_image.h>

Game::Game() {
}

Game::~Game() {
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    _window = SDL_CreateWindow(
        "Dune Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowWidth,
        _windowHeight,
        0);

    if (!_window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        0);

    if (!_renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "Error initializing SDL_image." << std::endl;
        return;
    }

    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    _isRunning = true;
}

void Game::Setup() {
    _playerPosition = glm::vec2(100, 100);
    _playerVelocity = glm::vec2(1.0f, 0);
}

void Game::Run() {
    Setup();

    while (_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Clean() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();

    _renderer = nullptr;
    _window = nullptr;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                    _isRunning = false;
                break;
        }
    };
}

void Game::Update() {
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME)
        SDL_Delay(timeToWait);

    millisecondsPreviousFrame = SDL_GetTicks();

    _playerPosition.x += _playerVelocity.x;
    _playerPosition.y += _playerVelocity.y;
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 15, 30, 8, 255);
    SDL_RenderClear(_renderer);

    SDL_Surface *playerSurface = IMG_Load("../../../assets/RockAsteroid.png");
    SDL_Texture *playerTexture = SDL_CreateTextureFromSurface(_renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    SDL_Rect destRect = {static_cast<int>(_playerPosition.x), static_cast<int>(_playerPosition.y), 64, 64};
    SDL_RenderCopy(_renderer, playerTexture, nullptr, &destRect);
    SDL_DestroyTexture(playerTexture);

    SDL_RenderPresent(_renderer);
}
