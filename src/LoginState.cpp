#include "states/LoginState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/HubState.h"
#include "states/StateMachine.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

LoginState::LoginState(StateContext& context)
    : m_Context(context), m_MessageColor(WHITE), m_Frame(0) {}

void LoginState::OnEnter() {}
void LoginState::OnExit() {}
void LoginState::Update() { ++m_Frame; }

void LoginState::Draw() {
    Console& c = m_Context.console;
    c.DrawHeader("PLAYER LOGIN");

    c.DrawPanel(14, 6, 62, 14, BLUE);
    c.WriteCentered(8, "Who is entering the hotel?", LIGHT_AQUA);

    // The text box with a blinking cursor
    string shown = m_Name + (((m_Frame / 15) % 2 == 0) ? "_" : " ");
    c.Write(30, 11, "Username:", WHITE);
    c.Write(40, 11, "[", GRAY);
    c.Write(41, 11, Utils::padRight(shown, 16), LIGHT_YELLOW);
    c.Write(57, 11, "]", GRAY);

    c.WriteCentered(13, "3-15 letters, numbers or _", GRAY);
    c.WriteCentered(14, "New name = new player.  Known name = continue.", GRAY);

    if (!m_Message.empty()) c.WriteCentered(17, m_Message, m_MessageColor);

    c.DrawPanelHorz(1, 27, Console::WIDTH - 2, BLUE);
    c.WriteCentered(28, "[Enter] Confirm    [Backspace] Delete    [Esc] Back", GRAY);
}

void LoginState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;

    if (kb.IsKeyJustPressed(KEY_ESCAPE)) {
        m_Context.stateMachine.PopState();
        return;
    }
    if (kb.IsKeyJustPressed(KEY_BACKSPACE) && !m_Name.empty()) m_Name.pop_back();

    for (char ch : kb.GetTyped()) {
        if ((isalnum(static_cast<unsigned char>(ch)) || ch == '_') && m_Name.size() < 15)
            m_Name += ch;
    }

    if (kb.IsKeyJustPressed(KEY_ENTER)) Submit();
}

void LoginState::Submit() {
    GameData& data = m_Context.gameData;

    if (!Utils::isValidUsername(m_Name)) {
        m_Message = "Username must be 3-15 letters, numbers or underscores.";
        m_MessageColor = LIGHT_RED;
        return;
    }

    Player loaded;
    if (data.fileManager.loadPlayer(m_Name, loaded)) {
        data.player = loaded;
        if (data.player.getCurrentStage() > GameRules::TOTAL_STAGES || !data.player.isAlive()) {
            data.player.resetProgress();
            data.GiveStarterWeapon();
        }
        string text = "Welcome back, " + data.player.getUsername() + "! ";
        if (data.player.getCurrentStage() > 1 || data.player.getScore() > 0)
            text += "Continuing from Stage " + to_string(data.player.getCurrentStage()) + ".";
        else
            text += "Starting a fresh run.";
        data.SetNotice(text, LIGHT_GREEN);
    } else {
        data.player = Player(m_Name);
        data.GiveStarterWeapon();
        data.fileManager.savePlayer(data.player);
        data.SetNotice("New player registered: " + m_Name + ". Good luck!", LIGHT_GREEN);
    }

    data.StartSession();
    m_Context.stateMachine.ChangeState(make_unique<HubState>(m_Context));
}
