// TitleState.h - the main menu
#pragma once

#include "states/IState.h"
#include "ui/Selector.h"

class TitleState : public IState {
public:
    explicit TitleState(StateContext& context);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
    Selector m_Selector;
    int m_Frame;
    int m_NoticeStart;

    void OnMenuSelect(int index);
    void DrawTitleScreen();
};
