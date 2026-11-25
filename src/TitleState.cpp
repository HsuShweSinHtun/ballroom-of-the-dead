#include "states/TitleState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/InfoState.h"
#include "states/LoginState.h"
#include "states/StateMachine.h"
#include "ui/Art.h"
#include "ui/Console.h"

TitleState::TitleState(StateContext& context)
    : m_Context(context),
      m_Selector(context.console, context.keyboard,
                 {"New Game / Continue", "Leaderboard", "Past Reports", "How to Play", "Exit"},
                 SelectorParams{34, 19, 1, 0, 2}),
      m_Frame(0),
      m_NoticeStart(-1) {
    m_Selector.SetSelectionFunc([this](int index) { OnMenuSelect(index); });
}

void TitleState::OnEnter() {}
void TitleState::OnExit() {}

void TitleState::Update() {
    ++m_Frame;
    if (!m_Context.gameData.notice.empty()) {
        if (m_NoticeStart < 0) m_NoticeStart = m_Frame;
        if (m_Frame - m_NoticeStart > 120) {  // about 4 seconds
            m_Context.gameData.notice.clear();
            m_NoticeStart = -1;
        }
    }
}

void TitleState::DrawTitleScreen() {
    Console& c = m_Context.console;
    int center = c.GetHalfWidth();

    // The chandelier bulbs flicker
    int chandelierColor = ((m_Frame / 18) % 2 == 0) ? LIGHT_YELLOW : YELLOW;
    c.WriteLinesCentered(center, 1, Art::CHANDELIER, chandelierColor);

    // The title flickers dark red now and then, like a failing light
    int titleColor = (m_Frame % 110 < 4 || (m_Frame % 47 == 0)) ? RED : LIGHT_RED;
    c.WriteLinesCentered(center, 8, Art::BALLROOM, titleColor);
    c.WriteLinesCentered(center, 13, Art::OF_THE_DEAD, BRIGHT_WHITE);

    c.DrawPanelHorz(1, 17, Console::WIDTH - 2, RED);
}

void TitleState::Draw() {
    DrawTitleScreen();
    m_Selector.Draw();
    if (!m_Context.gameData.notice.empty())
        m_Context.console.WriteCentered(18, m_Context.gameData.notice, m_Context.gameData.noticeColor);
    m_Context.console.WriteCentered(28, "[Up/Down] Move    [Enter] Select    [Esc] Exit", GRAY);
}

void TitleState::ProcessInputs() {
    if (m_Context.keyboard.IsKeyJustPressed(KEY_ESCAPE)) {
        m_Context.quitRequested = true;
        return;
    }
    m_Selector.ProcessInputs();
}

void TitleState::OnMenuSelect(int index) {
    switch (index) {
        case 0: m_Context.stateMachine.PushState(make_unique<LoginState>(m_Context)); break;
        case 1: m_Context.stateMachine.PushState(InfoState::CreateLeaderboard(m_Context)); break;
        case 2: m_Context.stateMachine.PushState(InfoState::CreateReports(m_Context)); break;
        case 3: m_Context.stateMachine.PushState(InfoState::CreateHowToPlay(m_Context)); break;
        case 4: m_Context.quitRequested = true; break;
        default: break;
    }
}
