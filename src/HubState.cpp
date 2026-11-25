#include "states/HubState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/BattleState.h"
#include "states/InventoryState.h"
#include "states/ShopState.h"
#include "states/StateMachine.h"
#include "states/StatusState.h"
#include "states/TitleState.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

HubState::HubState(StateContext& context)
    : m_Context(context),
      m_Selector(context.console, context.keyboard,
                 {"Fight", "Shop", "Inventory", "Status", "Save & Quit"},
                 SelectorParams{36, 7, 1, 0, 3}),
      m_NoticeFrames(0) {
    m_Selector.SetSelectionFunc([this](int index) { OnMenuSelect(index); });
}

void HubState::OnEnter() { m_NoticeFrames = 0; }
void HubState::OnExit() {}
void HubState::OnResume() { m_NoticeFrames = 0; }

void HubState::Update() {
    if (!m_Context.gameData.notice.empty()) {
        ++m_NoticeFrames;
        if (m_NoticeFrames > 150) m_Context.gameData.notice.clear();  // about 5 seconds
    }
}

void HubState::ProcessInputs() { m_Selector.ProcessInputs(); }

void HubState::OnMenuSelect(int index) {
    GameData& data = m_Context.gameData;
    switch (index) {
        case 0:
            data.notice.clear();
            m_Context.stateMachine.PushState(
                make_unique<BattleState>(m_Context, data.player.getCurrentStage()));
            break;
        case 1: m_Context.stateMachine.PushState(make_unique<ShopState>(m_Context)); break;
        case 2: m_Context.stateMachine.PushState(make_unique<InventoryState>(m_Context)); break;
        case 3: m_Context.stateMachine.PushState(make_unique<StatusState>(m_Context)); break;
        case 4:
            data.SaveProgress();
            data.SetNotice("Progress saved. See you next time!", LIGHT_GREEN);
            m_Context.stateMachine.ReplaceAll(make_unique<TitleState>(m_Context));
            break;
        default: break;
    }
}

void HubState::DrawRooms() {
    Console& c = m_Context.console;
    int current = m_Context.gameData.player.getCurrentStage();

    c.Write(5, 4, "THE HOTEL", LIGHT_AQUA);
    for (int stage = 1; stage <= GameRules::TOTAL_STAGES; ++stage) {
        string marker = "[ ]";
        int color = GRAY;
        if (stage < current) {
            marker = "[x]";
            color = GREEN;
        } else if (stage == current) {
            marker = "[>]";
            color = LIGHT_YELLOW;
        }
        int y = 7 + (stage - 1) * 3;
        c.Write(5, y, marker + " " + to_string(stage) + ".", color);
        c.Write(5, y + 1, "    " + GameRules::StageName(stage), color);
    }
}

void HubState::DrawPlayerCard() {
    Console& c = m_Context.console;
    Player& p = m_Context.gameData.player;
    int x = 60;

    c.Write(x, 4, p.getUsername(), LIGHT_AQUA);
    c.DrawBar(x, 6, 20, p.getHealth(), p.getMaxHealth());
    c.Write(x, 7, "HP " + to_string(p.getHealth()) + " / " + to_string(p.getMaxHealth()), WHITE);

    c.Write(x, 9, "Attack    " + to_string(p.getAttackPower()), WHITE);
    c.Write(x, 10, "Defense   " + to_string(p.getDefense()), WHITE);
    c.Write(x, 12, "Coins     " + to_string(p.getCoins()), LIGHT_YELLOW);
    c.Write(x, 13, "Score     " + to_string(p.getScore()), LIGHT_YELLOW);
    c.Write(x, 15, "Kills     " + to_string(p.getZombiesDefeated()), WHITE);
    c.Write(x, 16, "Medkits   " + to_string(p.getInventory().getMedkits()), WHITE);

    int seconds = p.getPlaySeconds() + static_cast<int>(time(nullptr) - m_Context.gameData.sessionStart);
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", seconds / 60, seconds % 60);
    c.Write(x, 18, string("Time      ") + buffer, GRAY);
}

void HubState::Draw() {
    Console& c = m_Context.console;
    const Player& p = m_Context.gameData.player;

    c.DrawHeader("~ THE GRAND HOTEL ~");
    c.WriteCentered(3, "Stage " + to_string(p.getCurrentStage()) + ": " +
                           GameRules::StageName(p.getCurrentStage()),
                    LIGHT_AQUA);

    c.DrawPanelVert(30, 4, 20, BLUE);
    c.DrawPanelVert(57, 4, 20, BLUE);
    c.DrawPanelHorz(1, 24, Console::WIDTH - 2, BLUE);

    DrawRooms();
    m_Selector.Draw();
    DrawPlayerCard();

    if (!m_Context.gameData.notice.empty())
        c.WriteCentered(26, m_Context.gameData.notice, m_Context.gameData.noticeColor);

    c.WriteCentered(28, "[Up/Down] Move    [Enter] Select", GRAY);
}
