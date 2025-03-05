#include "Game.h"

#include <filesystem>
#include <Registry.h>
#include <SDL_image.h>

#include "MovementSystem.h"
#include "RenderSystem.h"

#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "Logger.h"
#include "../Tilemap.h"
#include "../Assets/AssetDatabase.h"

Game::Game(): _registry(std::make_unique<Registry>()), _assetDatabase(std::make_unique<AssetDatabase>()) {
    Logger::Log("Game created");
    Logger::Log(std::filesystem::current_path().string());
}

Game::~Game() {
    Logger::Log("Game destroyed");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Error("Error initializing SDL.");
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
        Logger::Error("Error creating SDL window.");
        return;
    }

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        0);

    if (!_renderer) {
        Logger::Error("Error creating SDL renderer.");
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        Logger::Error("Error initializing SDL_image.");
        return;
    }

    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    _isRunning = true;
}

void Game::Setup() {
    _registry->AddSystem<MovementSystem>();
    _registry->AddSystem<RenderSystem>();

    _assetDatabase->AddTexture(_renderer, "Tilemap", "tileset.png");

    Tilemap tilemap = Tilemap();
    tilemap.LoadFromLDtk("D:\\GameDev\\C++\\DuneEngine\\assets\\test.ldtk");

    for (auto &tile: tilemap.GetTiles()) {
        auto tileEntity = _registry->CreateEntity();
        tileEntity.AddComponent<TransformComponent>(glm::vec2(tile.x * 2, tile.y * 2), glm::vec2(2.0f, 2.0f));
        tileEntity.AddComponent<RigidbodyComponent>(glm::vec2(0.0f, 2.0f));
        tileEntity.AddComponent<SpriteComponent>("Tilemap", 16, 16, tile.tilesetCoordX, tile.tilesetCoordY);
    }

    // for (int i = 0; i < _windowWidth / 32; i++) {
    //     auto entity = _registry->CreateEntity();
    //     entity.AddComponent<TransformComponent>(glm::vec2(i * 16.0f * 2, _windowHeight - 32),
    //                                             glm::vec2(2.0f, 2.0f));
    //     entity.AddComponent<SpriteComponent>("RockAsteroid", 16, 16, 16 * 8, 16 * 4);
    // }
    //
    // //Create moving entities
    // for (int i = 0; i < 10; i++) {
    //     auto entity = _registry->CreateEntity();
    //     entity.AddComponent<TransformComponent>(glm::vec2(i * 10, i * 20),
    //                                             glm::vec2(2.0f, 2.0f));
    //     entity.AddComponent<RigidbodyComponent>(glm::vec2(15.0f * i, 10.0f * i / 2));
    //     entity.AddComponent<SpriteComponent>("RockAsteroid", 16, 16, 16 * 0, 16 * 1);
    // }
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
    _assetDatabase->Clear();

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

    double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

    millisecondsPreviousFrame = SDL_GetTicks();

    _registry->GetSystem<MovementSystem>().Update(deltaTime);

    _registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 3, 3, 3, 255);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetDatabase);

    SDL_RenderPresent(_renderer);
}
