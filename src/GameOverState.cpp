#include "states/GameOverState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/StateMachine.h"
#include "states/TitleState.h"
#include "ui/Art.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

#include <sstream>

GameOverState::GameOverState(StateContext& context, bool won)
    : m_Context(context), m_bWon(won), m_Log(11, 50, 110), m_Frame(0) {}

// Ending the game: calculate the final score, make the report, and save everything
void GameOverState::OnEnter() {
    GameData& data = m_Context.gameData;
    Player& player = data.player;
    m_PlayerName = player.getUsername();

    data.UpdatePlaytime();
    player.addScore(GameRules::CalculateFinalBonus(player.getHealth(), m_bWon));

    string date = Utils::currentDateTime();
    string report = data.reporter.generateReport(player, m_bWon, player.getPlaySeconds(), date);

    data.reporter.saveReport(report);
    data.fileManager.appendHistory(date + "|" + player.getUsername() + "|" +
                                   (m_bWon ? "WIN" : "LOSE") + "|" + to_string(player.getScore()) +
                                   "|" + to_string(player.getStagesCompleted()) + "|" +
                                   to_string(player.getZombiesDefeated()) + "|" +
                                   to_string(player.getPlaySeconds()));
    data.reporter.updateLeaderboard(player);
    m_Top = data.reporter.getTopPlayers(5);

    // The report is typed out line by line
    istringstream stream(report);
    string line;
    while (getline(stream, line)) {
        if (line.rfind("=====", 0) == 0) continue;
        int color = WHITE;
        if (line == "GAME REPORT")
            color = LIGHT_YELLOW;
        else if (line.rfind("Status: WIN", 0) == 0)
            color = LIGHT_GREEN;
        else if (line.rfind("Status: LOSE", 0) == 0)
            color = LIGHT_RED;
        m_Log.Add(line, color);
    }

    // Start a fresh run for next time
    player.incrementGamesPlayed();
    player.resetProgress();
    data.GiveStarterWeapon();
    data.fileManager.savePlayer(player);
}

void GameOverState::OnExit() {}

void GameOverState::Update() {
    ++m_Frame;
    m_Log.Update();
}

void GameOverState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;
    if (kb.IsKeyJustPressed(KEY_ENTER) || kb.IsKeyJustPressed(KEY_SPACE)) {
        if (m_Log.IsBusy())
            m_Log.Skip();
        else
            m_Context.stateMachine.ReplaceAll(make_unique<TitleState>(m_Context));
    }
}

void GameOverState::Draw() {
    Console& c = m_Context.console;
    int center = c.GetHalfWidth();

    if (m_bWon) {
        int color = ((m_Frame / 10) % 2 == 0) ? LIGHT_YELLOW : LIGHT_GREEN;
        c.WriteLinesCentered(center, 2, Art::VICTORY, color);
        c.WriteCentered(7, "The Ballroom King is dead. You escaped the hotel alive!", LIGHT_GREEN);
    } else {
        c.WriteLinesCentered(center, 2, Art::GAME_OVER, LIGHT_RED);
        c.WriteCentered(7, "The horde overwhelmed you...", RED);
    }
    c.DrawPanelHorz(1, 8, Console::WIDTH - 2, BLUE);

    // Report (left)
    c.Write(5, 9, "YOUR REPORT", LIGHT_AQUA);
    m_Log.Draw(c, 5, 11);

    // Leaderboard (right)
    c.DrawPanelVert(58, 9, 16, BLUE);
    c.Write(62, 9, "TOP PLAYERS", LIGHT_AQUA);
    for (size_t i = 0; i < m_Top.size(); ++i) {
        bool you = Utils::toLower(m_Top[i].username) == Utils::toLower(m_PlayerName);
        int color = you ? LIGHT_YELLOW : WHITE;
        string row = "#" + to_string(i + 1) + " " + Utils::padRight(m_Top[i].username, 15) +
                     Utils::padLeft(to_string(m_Top[i].score), 6);
        c.Write(62, 11 + static_cast<int>(i) * 2, row, color);
    }

    c.DrawPanelHorz(1, 26, Console::WIDTH - 2, BLUE);
    if (m_Log.IsBusy())
        c.WriteCentered(28, "[Enter] Skip", GRAY);
    else
        c.WriteCentered(28, "[Enter] Return to the main menu", LIGHT_YELLOW);
}
