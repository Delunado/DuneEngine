#include "Game.h"

Game::Game() {
}

Game::~Game() {
}

void Game::Initialize() {
}

void Game::Run() {
    while (true) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Clean() {
}


void Game::ProcessInput() {
}

void Game::Update() {
}

void Game::Render() {
}
