#include "states/StatusState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/StateMachine.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

void StatusState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;
    if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE) ||
        kb.IsKeyJustPressed(KEY_ENTER))
        m_Context.stateMachine.PopState();
}

void StatusState::Draw() {
    Console& c = m_Context.console;
    Player& p = m_Context.gameData.player;

    c.DrawHeader("~ STATUS ~");
    c.DrawPanelVert(44, 3, 22, BLUE);

    // Left: the character
    c.Write(6, 4, p.getUsername(), LIGHT_AQUA);
    c.DrawBar(6, 6, 24, p.getHealth(), p.getMaxHealth());
    c.Write(6, 7, "HP " + to_string(p.getHealth()) + " / " + to_string(p.getMaxHealth()), WHITE);
    c.Write(6, 9, "Attack power   " + to_string(p.getAttackPower()), WHITE);
    c.Write(6, 10, "Defense        " + to_string(p.getDefense()), WHITE);
    c.Write(6, 12, "Coins          " + to_string(p.getCoins()), LIGHT_YELLOW);
    c.Write(6, 13, "Score          " + to_string(p.getScore()), LIGHT_YELLOW);
    c.Write(6, 15, "Medkits        " + to_string(p.getInventory().getMedkits()), WHITE);
    c.Write(6, 16, "Weapons owned  " + to_string(p.getInventory().getWeaponCount()), WHITE);
    c.Write(6, 18, "Games played   " + to_string(p.getGamesPlayed()), GRAY);

    // Right: this run
    int seconds = p.getPlaySeconds() + static_cast<int>(time(nullptr) - m_Context.gameData.sessionStart);
    c.Write(48, 4, "THIS RUN", LIGHT_AQUA);
    c.Write(48, 6, "Stage            " + to_string(p.getCurrentStage()) + " / " +
                       to_string(GameRules::TOTAL_STAGES),
            WHITE);
    c.Write(48, 7, "Zombies defeated " + to_string(p.getZombiesDefeated()), WHITE);
    c.Write(48, 8, "Damage dealt     " + to_string(p.getTotalDamageDealt()), WHITE);
    c.Write(48, 9, "Time played      " + Utils::formatDuration(seconds), WHITE);

    c.Write(48, 11, "Weapons used so far:", GRAY);
    string used = p.getWeaponsUsed().empty() ? "None yet" : Utils::join(p.getWeaponsUsed(), ", ");
    c.Write(48, 12, used.substr(0, 38), LIGHT_YELLOW);

    c.DrawPanelHorz(1, 27, Console::WIDTH - 2, BLUE);
    c.WriteCentered(28, "[Esc / Enter] Back", GRAY);
}
