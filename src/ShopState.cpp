#include "states/ShopState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/StateMachine.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

ShopState::ShopState(StateContext& context)
    : m_Context(context),
      m_Selector(context.console, context.keyboard, {}, SelectorParams{7, 5, 1, 0, 2}),
      m_RepairSelector(context.console, context.keyboard, {}, SelectorParams{7, 5, 1, 0, 2}),
      m_Mode(Mode::Main),
      m_Typewriter(80),
      m_MessageColor(WHITE) {
    m_Selector.SetSelectionFunc([this](int index) { OnSelect(index); });
    m_RepairSelector.SetSelectionFunc([this](int index) { OnRepairSelect(index); });

    // Weapons from weapons.txt (the free starter weapon is not for sale)
    const vector<Weapon>& catalog = m_Context.gameData.catalog;
    for (size_t i = 0; i < catalog.size(); ++i)
        if (catalog[i].getPrice() > 0)
            m_Items.push_back({ItemType::Weapon, static_cast<int>(i)});
    m_Items.push_back({ItemType::Medkit, -1});
    m_Items.push_back({ItemType::Armor, -1});
    m_Items.push_back({ItemType::Repair, -1});
    m_Items.push_back({ItemType::Leave, -1});
}

void ShopState::OnEnter() {
    RefreshItems();
    Say("Welcome! Spend your coins wisely.", WHITE);
}

void ShopState::OnExit() {}

void ShopState::Say(const string& text, int color) {
    m_Typewriter.Start(text);
    m_MessageColor = color;
}

void ShopState::RefreshItems() {
    GameData& data = m_Context.gameData;
    vector<string> labels;
    for (const ShopItem& item : m_Items) {
        switch (item.type) {
            case ItemType::Weapon: {
                const Weapon& w = data.catalog[item.catalogIndex];
                string label = Utils::padRight(w.getName(), 16) +
                               Utils::padLeft(to_string(w.getPrice()) + "g", 5);
                if (data.player.getInventory().hasWeapon(w.getName())) label += "  [owned]";
                labels.push_back(label);
                break;
            }
            case ItemType::Medkit:
                labels.push_back(Utils::padRight("Medkit", 16) +
                                 Utils::padLeft(to_string(GameRules::MEDKIT_PRICE) + "g", 5));
                break;
            case ItemType::Armor:
                labels.push_back(Utils::padRight("Armor Plate", 16) +
                                 Utils::padLeft(to_string(GameRules::ARMOR_PRICE) + "g", 5));
                break;
            case ItemType::Repair:
                labels.push_back(Utils::padRight("Repair Weapon", 16) +
                                 Utils::padLeft(to_string(GameRules::REPAIR_PRICE) + "g", 5));
                break;
            case ItemType::Leave:
                labels.push_back("Leave Shop");
                break;
        }
    }
    m_Selector.SetData(labels);
}

void ShopState::OnSelect(int index) {
    GameData& data = m_Context.gameData;
    Player& player = data.player;
    const ShopItem& item = m_Items[index];

    switch (item.type) {
        case ItemType::Weapon: {
            const Weapon& w = data.catalog[item.catalogIndex];
            if (player.getInventory().hasWeapon(w.getName())) {
                Say("You already own the " + w.getName() + ".", LIGHT_YELLOW);
            } else if (!player.spendCoins(w.getPrice())) {
                Say("Not enough coins for the " + w.getName() + ".", LIGHT_RED);
            } else {
                player.getInventory().addWeapon(w);
                Say("You bought the " + w.getName() + "!", LIGHT_GREEN);
                RefreshItems();
            }
            break;
        }
        case ItemType::Medkit:
            if (player.spendCoins(GameRules::MEDKIT_PRICE)) {
                player.getInventory().addMedkits(1);
                Say("Medkit bought.", LIGHT_GREEN);
            } else {
                Say("Not enough coins.", LIGHT_RED);
            }
            break;
        case ItemType::Armor:
            if (player.spendCoins(GameRules::ARMOR_PRICE)) {
                player.increaseDefense(1);
                Say("Armor plate fitted. Defense is now " + to_string(player.getDefense()) + ".",
                    LIGHT_GREEN);
            } else {
                Say("Not enough coins.", LIGHT_RED);
            }
            break;
        case ItemType::Repair:
            OpenRepairMenu();
            break;
        case ItemType::Leave:
            m_Context.stateMachine.PopState();
            break;
    }
}

void ShopState::OpenRepairMenu() {
    Inventory& inventory = m_Context.gameData.player.getInventory();
    m_RepairIndices.clear();
    vector<string> labels;
    for (int i = 0; i < inventory.getWeaponCount(); ++i) {
        Weapon* w = inventory.getWeapon(i);
        if (!w->needsRepair()) continue;
        m_RepairIndices.push_back(i);
        labels.push_back(Utils::padRight(w->getName(), 16) +
                         Utils::padLeft(to_string(w->getDurability()) + "/" +
                                            to_string(w->getMaxDurability()),
                                        7));
    }
    if (labels.empty()) {
        Say("None of your weapons need repair.", LIGHT_YELLOW);
        return;
    }
    m_RepairIndices.push_back(-1);
    labels.push_back("Cancel");
    m_RepairSelector.SetData(labels);
    m_RepairSelector.SetIndex(0);
    m_Mode = Mode::Repair;
    Say("Which weapon should I repair? (" + to_string(GameRules::REPAIR_PRICE) + "g)", WHITE);
}

void ShopState::OnRepairSelect(int index) {
    int inventoryIndex = m_RepairIndices[index];
    if (inventoryIndex < 0) {
        m_Mode = Mode::Main;
        return;
    }
    Player& player = m_Context.gameData.player;
    if (player.spendCoins(GameRules::REPAIR_PRICE)) {
        Weapon* w = player.getInventory().getWeapon(inventoryIndex);
        w->repair();
        Say("The " + w->getName() + " is as good as new.", LIGHT_GREEN);
    } else {
        Say("Not enough coins.", LIGHT_RED);
    }
    m_Mode = Mode::Main;
}

void ShopState::Update() { m_Typewriter.Update(); }

void ShopState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;
    if (m_Mode == Mode::Repair) {
        if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE))
            m_Mode = Mode::Main;
        else
            m_RepairSelector.ProcessInputs();
        return;
    }
    if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE)) {
        m_Context.stateMachine.PopState();
        return;
    }
    m_Selector.ProcessInputs();
}

void ShopState::DrawDetails() {
    Console& c = m_Context.console;
    GameData& data = m_Context.gameData;
    int x = 46;
    int index = m_Selector.GetIndex();
    if (index < 0 || index >= static_cast<int>(m_Items.size())) return;
    const ShopItem& item = m_Items[index];

    c.Write(x, 4, "DETAILS", LIGHT_AQUA);
    switch (item.type) {
        case ItemType::Weapon: {
            const Weapon& w = data.catalog[item.catalogIndex];
            c.Write(x, 6, w.getName(), LIGHT_YELLOW);
            c.Write(x, 8, "Damage      " + to_string(w.getDamage()), WHITE);
            c.Write(x, 9, "Durability  " + to_string(w.getMaxDurability()), WHITE);
            c.Write(x, 10, "Price       " + to_string(w.getPrice()) + " coins", WHITE);
            c.Write(x, 12, "Added to your attack damage.", GRAY);
            c.Write(x, 13, "Loses 1 durability per hit.", GRAY);
            break;
        }
        case ItemType::Medkit:
            c.Write(x, 6, "Medkit", LIGHT_YELLOW);
            c.Write(x, 8, "Restores " + to_string(GameRules::MEDKIT_HEAL) + " HP.", WHITE);
            c.Write(x, 9, "Use it in battle (uses your turn)", GRAY);
            c.Write(x, 10, "or in the Inventory screen.", GRAY);
            break;
        case ItemType::Armor:
            c.Write(x, 6, "Armor Plate", LIGHT_YELLOW);
            c.Write(x, 8, "+1 Defense, permanently.", WHITE);
            c.Write(x, 9, "Current defense: " + to_string(data.player.getDefense()), GRAY);
            break;
        case ItemType::Repair:
            c.Write(x, 6, "Repair Weapon", LIGHT_YELLOW);
            c.Write(x, 8, "Restores one weapon to full", WHITE);
            c.Write(x, 9, "durability.", WHITE);
            break;
        case ItemType::Leave:
            c.Write(x, 6, "Leave Shop", LIGHT_YELLOW);
            c.Write(x, 8, "Return to the hotel.", WHITE);
            break;
    }
}

void ShopState::Draw() {
    Console& c = m_Context.console;
    GameData& data = m_Context.gameData;

    c.DrawHeader("~ HOTEL SHOP ~");
    c.DrawPanelVert(42, 3, 20, BLUE);
    c.DrawPanelHorz(1, 23, Console::WIDTH - 2, BLUE);

    if (m_Mode == Mode::Main) {
        c.Write(7, 3, "FOR SALE", LIGHT_AQUA);
        m_Selector.Draw();
        DrawDetails();
    } else {
        c.Write(7, 3, "REPAIR", LIGHT_AQUA);
        m_RepairSelector.Draw();
        c.Write(46, 4, "Pick a weapon to repair.", GRAY);
    }

    c.Write(7, 21, "Your coins: " + to_string(data.player.getCoins()), LIGHT_YELLOW);
    c.Write(4, 25, m_Typewriter.GetVisibleText(), m_MessageColor);
    c.WriteCentered(28, "[Up/Down] Move    [Enter] Buy    [Esc] Leave", GRAY);
}
