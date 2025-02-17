#ifndef GAME_H
#define GAME_H

class Game {
public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void Clean();

private:
    void ProcessInput();

    void Update();

    void Render();
};

#endif //GAME_H
