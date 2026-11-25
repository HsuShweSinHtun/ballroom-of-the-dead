// Game.h - controls the main game loop: input -> update -> draw, state by state
#pragma once

#include <memory>
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "states/IState.h"
#include "states/StateMachine.h"
#include "ui/Console.h"
using namespace std;

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    Console m_Console;
    Keyboard m_Keyboard;
    StateMachine m_StateMachine;
    GameData m_GameData;
    bool m_bQuit;
    StateContext m_Context;
};
