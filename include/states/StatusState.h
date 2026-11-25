// StatusState.h - the character sheet
#pragma once

#include "states/IState.h"

class StatusState : public IState {
public:
    explicit StatusState(StateContext& context) : m_Context(context) {}

    void OnEnter() override {}
    void OnExit() override {}
    void Update() override {}
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
};
