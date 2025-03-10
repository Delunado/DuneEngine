#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <memory>

class EventBus;
class AssetDatabase;
class Entity;
class Registry;

constexpr int FPS = 60;
constexpr int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void Clean();

    static int _windowWidth;
    static int _windowHeight;
    static int _mapWidth;
    static int _mapHeight;

private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;

    std::unique_ptr<Registry> _registry;
    std::unique_ptr<AssetDatabase> _assetDatabase;
    std::unique_ptr<EventBus> _eventBus;

    bool _isRunning;
    bool _isDebugMode = false;
    int millisecondsPreviousFrame = 0;

    SDL_Rect _camera;

    void Setup();

    void LoadLevel() const;

    void ProcessInput();

    void Update();

    void Render();
};

#endif //GAME_H
