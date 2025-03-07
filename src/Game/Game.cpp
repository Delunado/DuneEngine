#include "Game.h"

#include <filesystem>
#include <Registry.h>
#include <SDL_image.h>

#include "MovementSystem.h"
#include "RenderSystem.h"
#include "AnimationSystem.h"

#include "AnimationComponent.h"
#include "TransformComponent.h"

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
    _registry->AddSystem<AnimationSystem>();

    _assetDatabase->AddTexture(_renderer, "Tilemap", "tileset.png");
    _assetDatabase->AddTexture(_renderer, "RockAsteroid", "RockAsteroid.png");

    LoadLevel();
}

void Game::LoadLevel() const {
    Tilemap tilemap = Tilemap();
    tilemap.LoadFromLDtk(AssetDatabase::GetAssetPath("test.ldtk"));

    auto player = _registry->CreateEntity();
    player.AddComponent<TransformComponent>(glm::vec2(300, 45), glm::vec2(4.0f, 4.0f));
    player.AddComponent<RigidbodyComponent>(glm::vec2(0, 25));
    player.AddComponent<SpriteComponent>("Tilemap", 16, 16, 1, 0, 19 * 16);
    player.AddComponent<AnimationComponent>(2, 12, true);

    for (auto &tile: tilemap.GetTiles()) {
        auto tileEntity = _registry->CreateEntity();
        tileEntity.AddComponent<TransformComponent>(glm::vec2(tile.x * 4, tile.y * 4), glm::vec2(4.0f, 4.0f));
        tileEntity.AddComponent<SpriteComponent>("Tilemap", 16, 16, 0, tile.tilesetCoordX, tile.tilesetCoordY);
    }
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
    _registry->GetSystem<AnimationSystem>().Update(deltaTime);

    _registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(_renderer, 3, 3, 3, 255);
    SDL_RenderClear(_renderer);

    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetDatabase);

    SDL_RenderPresent(_renderer);
}
