// ShopState.h - buy weapons, medkits, armor and repairs
#pragma once

#include <string>
#include <vector>
#include "states/IState.h"
#include "ui/Selector.h"
#include "utilities/Typewriter.h"
using namespace std;

class ShopState : public IState {
public:
    explicit ShopState(StateContext& context);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    enum class ItemType { Weapon, Medkit, Armor, Repair, Leave };
    struct ShopItem {
        ItemType type;
        int catalogIndex;  // only used for weapons
    };
    enum class Mode { Main, Repair };

    StateContext& m_Context;
    vector<ShopItem> m_Items;
    Selector m_Selector;
    Selector m_RepairSelector;
    vector<int> m_RepairIndices;
    Mode m_Mode;
    Typewriter m_Typewriter;
    int m_MessageColor;

    void RefreshItems();
    void OnSelect(int index);
    void OnRepairSelect(int index);
    void OpenRepairMenu();
    void Say(const string& text, int color);
    void DrawDetails();
};
