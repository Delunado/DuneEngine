﻿#include "Game.h"

#include <iostream>

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

    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    _isRunning = true;
}

void Game::Setup() {
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
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 15, 30, 8, 255);
    SDL_RenderClear(_renderer);

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_Rect player = {200, 100, 50, 50};
    SDL_RenderFillRect(_renderer, &player);

    SDL_RenderPresent(_renderer);
}
