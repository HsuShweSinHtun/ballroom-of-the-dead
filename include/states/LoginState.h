// LoginState.h - the player types a username (register a new player or load a saved one)
#pragma once

#include <string>
#include "states/IState.h"
using namespace std;

class LoginState : public IState {
public:
    explicit LoginState(StateContext& context);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
    string m_Name;
    string m_Message;
    int m_MessageColor;
    int m_Frame;

    void Submit();
};
