// IState.h - every screen of the game (title, menu, battle, shop...) is a state
#pragma once

class IState {
public:
    virtual ~IState() {}
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;
    virtual void OnResume() {}  // called when the state above this one is removed
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void ProcessInputs() = 0;
};

// Everything a state needs to talk to the rest of the game
struct StateContext {
    class Console& console;
    class Keyboard& keyboard;
    class StateMachine& stateMachine;
    struct GameData& gameData;
    bool& quitRequested;
};
