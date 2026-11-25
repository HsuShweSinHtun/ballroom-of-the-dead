// HubState.h - the menu between fights: Fight, Shop, Inventory, Status, Save & Quit
#pragma once

#include "states/IState.h"
#include "ui/Selector.h"

class HubState : public IState {
public:
    explicit HubState(StateContext& context);

    void OnEnter() override;
    void OnExit() override;
    void OnResume() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
    Selector m_Selector;
    int m_NoticeFrames;

    void OnMenuSelect(int index);
    void DrawRooms();
    void DrawPlayerCard();
};
