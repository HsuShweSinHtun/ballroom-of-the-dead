// InventoryState.h - look at your weapons and use medkits
#pragma once

#include <string>
#include "states/IState.h"
#include "ui/Selector.h"
#include "utilities/Typewriter.h"
using namespace std;

class InventoryState : public IState {
public:
    explicit InventoryState(StateContext& context);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
    Selector m_Selector;
    Typewriter m_Typewriter;
    int m_MessageColor;

    void Refresh();
    void OnSelect(int index);
    void Say(const string& text, int color);
};
