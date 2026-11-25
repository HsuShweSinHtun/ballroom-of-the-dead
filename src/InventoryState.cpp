#include "states/InventoryState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/StateMachine.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

InventoryState::InventoryState(StateContext& context)
    : m_Context(context),
      m_Selector(context.console, context.keyboard, {}, SelectorParams{7, 5, 1, 0, 2}),
      m_Typewriter(80),
      m_MessageColor(WHITE) {
    m_Selector.SetSelectionFunc([this](int index) { OnSelect(index); });
}

void InventoryState::OnEnter() { Refresh(); }
void InventoryState::OnExit() {}

void InventoryState::Say(const string& text, int color) {
    m_Typewriter.Start(text);
    m_MessageColor = color;
}

void InventoryState::Refresh() {
    Inventory& inventory = m_Context.gameData.player.getInventory();
    vector<string> labels;
    for (int i = 0; i < inventory.getWeaponCount(); ++i) {
        Weapon* w = inventory.getWeapon(i);
        labels.push_back(Utils::padRight(w->getName(), 16) +
                         Utils::padLeft(to_string(w->getDurability()) + "/" +
                                            to_string(w->getMaxDurability()),
                                        7) +
                         (w->isBroken() ? "  [BROKEN]" : ""));
    }
    labels.push_back("Medkit x" + to_string(inventory.getMedkits()));
    m_Selector.SetData(labels);
}

void InventoryState::OnSelect(int index) {
    Player& player = m_Context.gameData.player;
    Inventory& inventory = player.getInventory();

    if (index < inventory.getWeaponCount()) {
        Say("Weapons are used in battle. Repair them in the shop.", GRAY);
        return;
    }

    // The medkit row
    if (inventory.getMedkits() <= 0) {
        Say("You have no medkits.", LIGHT_RED);
    } else if (player.getHealth() >= player.getMaxHealth()) {
        Say("You are already at full health.", LIGHT_YELLOW);
    } else {
        inventory.useMedkit();
        player.heal(GameRules::MEDKIT_HEAL);
        Say("You use a medkit. Health: " + to_string(player.getHealth()) + "/" +
                to_string(player.getMaxHealth()),
            LIGHT_GREEN);
        Refresh();
    }
}

void InventoryState::Update() { m_Typewriter.Update(); }

void InventoryState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;
    if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE)) {
        m_Context.stateMachine.PopState();
        return;
    }
    m_Selector.ProcessInputs();
}

void InventoryState::Draw() {
    Console& c = m_Context.console;
    Player& player = m_Context.gameData.player;
    Inventory& inventory = player.getInventory();

    c.DrawHeader("~ INVENTORY ~");
    c.DrawPanelVert(46, 3, 20, BLUE);
    c.DrawPanelHorz(1, 23, Console::WIDTH - 2, BLUE);

    c.Write(7, 3, "ITEMS", LIGHT_AQUA);
    m_Selector.Draw();

    // Details of the selected row
    int x = 50;
    int index = m_Selector.GetIndex();
    c.Write(x, 3, "DETAILS", LIGHT_AQUA);
    if (index < inventory.getWeaponCount()) {
        Weapon* w = inventory.getWeapon(index);
        c.Write(x, 5, w->getName(), LIGHT_YELLOW);
        c.Write(x, 7, "Damage      " + to_string(w->getDamage()), WHITE);
        c.Write(x, 8, "Durability  " + to_string(w->getDurability()) + " / " +
                           to_string(w->getMaxDurability()),
                WHITE);
        c.DrawBar(x, 10, 20, w->getDurability(), w->getMaxDurability());
        if (w->isBroken())
            c.Write(x, 12, "Broken! Repair it in the shop.", LIGHT_RED);
        else if (w->needsRepair())
            c.Write(x, 12, "Worn. A repair costs " + to_string(GameRules::REPAIR_PRICE) + "g.", GRAY);
    } else {
        c.Write(x, 5, "Medkit", LIGHT_YELLOW);
        c.Write(x, 7, "Restores " + to_string(GameRules::MEDKIT_HEAL) + " HP.", WHITE);
        c.Write(x, 9, "You have " + to_string(inventory.getMedkits()), WHITE);
        c.Write(x, 11, "Press Enter to use one now.", GRAY);
        c.DrawBar(x, 13, 20, player.getHealth(), player.getMaxHealth());
        c.Write(x, 14, "HP " + to_string(player.getHealth()) + " / " + to_string(player.getMaxHealth()),
                WHITE);
    }

    c.Write(4, 25, m_Typewriter.GetVisibleText(), m_MessageColor);
    c.WriteCentered(28, "[Up/Down] Move    [Enter] Use    [Esc] Back", GRAY);
}
