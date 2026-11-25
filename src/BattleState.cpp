#include "states/BattleState.h"
#include "GameData.h"
#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "states/GameOverState.h"
#include "states/StateMachine.h"
#include "ui/Art.h"
#include "ui/Console.h"
#include "utilities/GameRules.h"
#include "utilities/Utils.h"

BattleState::BattleState(StateContext& context, int stage)
    : m_Context(context),
      m_Stage(stage),
      m_ZombieIndex(0),
      m_Turn(0),
      m_Defending(false),
      m_Phase(Phase::Messaging),
      m_Log(6, 84, 75),
      m_ActionSelector(context.console, context.keyboard,
                       {"Attack", "Defend", "Medkit", "Run Away"},
                       SelectorParams{10, 26, 2, 30, 1}),
      m_WeaponSelector(context.console, context.keyboard, {}, SelectorParams{6, 26, 3, 28, 1}),
      m_ZombieFlash(0),
      m_PlayerFlash(0) {
    m_ActionSelector.SetSelectionFunc([this](int index) { OnActionSelect(index); });
    m_WeaponSelector.SetSelectionFunc([this](int index) { OnWeaponSelect(index); });
}

// ---------------------------------------------------------------------------
//  Flow
// ---------------------------------------------------------------------------

void BattleState::BuildStage() {
    m_Zombies.clear();
    switch (m_Stage) {
        case 1:
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            break;
        case 2:
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            m_Zombies.push_back(make_unique<Runner>(m_Stage));
            break;
        case 3:
            m_Zombies.push_back(make_unique<Runner>(m_Stage));
            m_Zombies.push_back(make_unique<Brute>(m_Stage));
            m_Zombies.push_back(make_unique<Runner>(m_Stage));
            break;
        case 4:
            m_Zombies.push_back(make_unique<Runner>(m_Stage));
            m_Zombies.push_back(make_unique<Runner>(m_Stage));
            m_Zombies.push_back(make_unique<Brute>(m_Stage));
            m_Zombies.push_back(make_unique<Brute>(m_Stage));
            break;
        default:
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            m_Zombies.push_back(make_unique<Walker>(m_Stage));
            m_Zombies.push_back(make_unique<BallroomKing>(m_Stage));
            break;
    }
}

void BattleState::OnEnter() {
    BuildStage();
    m_Log.Add("*** STAGE " + to_string(m_Stage) + ": " + GameRules::StageName(m_Stage) + " ***",
              LIGHT_YELLOW);
    m_Log.Add(GameRules::StageIntro(m_Stage), GRAY);
    Continue([this]() { NextEncounter(); });
}

void BattleState::OnExit() {}

// After the messages finish typing, run 'next'
void BattleState::Continue(function<void()> next) {
    m_Phase = Phase::Messaging;
    m_After = next;
}

Zombie& BattleState::CurrentZombie() {
    int index = min(m_ZombieIndex, static_cast<int>(m_Zombies.size()) - 1);
    return *m_Zombies[index];
}

void BattleState::NextEncounter() {
    if (m_ZombieIndex >= static_cast<int>(m_Zombies.size())) {
        StageCleared();
        return;
    }
    Zombie& zombie = CurrentZombie();
    m_Turn = 0;
    m_Defending = false;
    m_Log.Add("Enemy " + to_string(m_ZombieIndex + 1) + " of " + to_string(m_Zombies.size()) +
                  ": " + zombie.getIntro(),
              LIGHT_RED);
    Continue([this]() { EnterChooseAction(); });
}

void BattleState::EnterChooseAction() {
    int medkits = m_Context.gameData.player.getInventory().getMedkits();
    m_ActionSelector.SetData({"Attack", "Defend", "Medkit (" + to_string(medkits) + ")", "Run Away"});
    m_ActionSelector.SetIndex(0);
    m_Phase = Phase::ChooseAction;
}

void BattleState::OnActionSelect(int index) {
    switch (index) {
        case 0: BeginWeaponChoice(); break;
        case 1: DoDefend(); break;
        case 2: DoMedkit(); break;
        case 3: DoRunAway(); break;
        default: break;
    }
}

void BattleState::BeginWeaponChoice() {
    Inventory& inventory = m_Context.gameData.player.getInventory();
    m_WeaponIndices.clear();

    vector<string> items;
    for (int i = 0; i < inventory.getWeaponCount(); ++i) {
        Weapon* weapon = inventory.getWeapon(i);
        if (weapon->isBroken()) continue;
        m_WeaponIndices.push_back(i);
        items.push_back(weapon->getName() + " " + to_string(weapon->getDurability()) + "/" +
                        to_string(weapon->getMaxDurability()));
    }

    if (items.empty()) {
        m_Log.Add("All your weapons are broken! You fight with your bare hands.", LIGHT_YELLOW);
        Continue([this]() { DoAttack(nullptr); });
        return;
    }

    m_WeaponIndices.push_back(-1);
    items.push_back("Bare Hands");
    m_WeaponSelector.SetData(items);
    m_WeaponSelector.SetIndex(0);
    m_Phase = Phase::ChooseWeapon;
}

void BattleState::OnWeaponSelect(int index) {
    int inventoryIndex = m_WeaponIndices[index];
    Weapon* weapon =
        (inventoryIndex < 0) ? nullptr : m_Context.gameData.player.getInventory().getWeapon(inventoryIndex);
    DoAttack(weapon);
}

// ---------------------------------------------------------------------------
//  Combat
// ---------------------------------------------------------------------------

void BattleState::DoAttack(Weapon* weapon) {
    Player& player = m_Context.gameData.player;
    Zombie& zombie = CurrentZombie();

    string weaponName = weapon ? weapon->getName() : "bare hands";
    int damage = player.attack(weapon);
    zombie.takeDamage(damage);
    m_ZombieFlash = 8;

    m_Log.Add("You strike the " + zombie.getType() + " with your " + weaponName + " for " +
                  to_string(damage) + " damage!",
              LIGHT_GREEN);
    if (weapon && weapon->isBroken()) m_Log.Add("Your " + weaponName + " just broke!", LIGHT_YELLOW);

    if (!zombie.isAlive())
        Continue([this]() { ZombieDefeated(); });
    else
        Continue([this]() { ZombieTurn(); });
}

void BattleState::DoDefend() {
    m_Defending = true;
    m_Log.Add("You brace yourself for the next hit.", LIGHT_AQUA);
    Continue([this]() { ZombieTurn(); });
}

void BattleState::DoMedkit() {
    Player& player = m_Context.gameData.player;
    if (player.getInventory().useMedkit()) {
        player.heal(GameRules::MEDKIT_HEAL);
        m_Log.Add("You use a medkit and recover up to " + to_string(GameRules::MEDKIT_HEAL) + " HP.",
                  LIGHT_GREEN);
        Continue([this]() { ZombieTurn(); });
    } else {
        m_Log.Add("You have no medkits! You can buy some in the shop.", LIGHT_YELLOW);
        Continue([this]() { EnterChooseAction(); });  // does not use up the turn
    }
}

void BattleState::DoRunAway() {
    Player& player = m_Context.gameData.player;
    Zombie& zombie = CurrentZombie();

    if (!zombie.canFlee()) {
        m_Log.Add("There is no escape from the " + zombie.getType() + "!", LIGHT_RED);
        Continue([this]() { EnterChooseAction(); });
        return;
    }

    if (Utils::randomInt(1, 100) <= 50) {
        player.addScore(-GameRules::FLEE_PENALTY);
        m_Log.Add("You escape! (-" + to_string(GameRules::FLEE_PENALTY) + " score)", LIGHT_YELLOW);
        Continue([this]() {
            m_Context.gameData.SaveProgress();
            m_Context.gameData.SetNotice("You escaped, but the room fills with zombies again.",
                                         LIGHT_YELLOW);
            m_Context.stateMachine.PopState();
        });
    } else {
        m_Log.Add("You try to run, but the " + zombie.getType() + " blocks your way!", LIGHT_RED);
        Continue([this]() { ZombieTurn(); });
    }
}

void BattleState::ZombieDefeated() {
    Player& player = m_Context.gameData.player;
    Zombie& zombie = CurrentZombie();

    player.addScore(zombie.getScoreValue());
    player.addCoins(zombie.getCoinReward());
    player.recordKill();

    m_Log.Add("The " + zombie.getType() + " collapses. Defeated!", LIGHT_GREEN);
    m_Log.Add("+" + to_string(zombie.getScoreValue()) + " score, +" +
                  to_string(zombie.getCoinReward()) + " coins.",
              LIGHT_YELLOW);

    ++m_ZombieIndex;
    Continue([this]() { NextEncounter(); });
}

void BattleState::ZombieTurn() {
    Player& player = m_Context.gameData.player;
    Zombie& zombie = CurrentZombie();

    ++m_Turn;
    string message;
    int rawDamage = zombie.attack(m_Turn, message);
    int damage = GameRules::CalculateZombieDamage(rawDamage, player.getDefense(), m_Defending);
    player.takeDamage(damage);
    m_PlayerFlash = 8;
    m_Defending = false;

    m_Log.Add(message + " You take " + to_string(damage) + " damage.", LIGHT_RED);

    if (!player.isAlive()) {
        m_Log.Add("You collapse to the floor...", RED);
        Continue([this]() { PlayerDied(); });
    } else {
        Continue([this]() { EnterChooseAction(); });
    }
}

void BattleState::PlayerDied() {
    m_Context.stateMachine.ReplaceAll(make_unique<GameOverState>(m_Context, false));
}

void BattleState::StageCleared() {
    Player& player = m_Context.gameData.player;

    int scoreBonus = 20 * m_Stage;
    int coinBonus = 15 * m_Stage;
    player.addScore(scoreBonus);
    player.addCoins(coinBonus);

    m_Log.Add("*** STAGE " + to_string(m_Stage) + " CLEARED! ***", LIGHT_YELLOW);
    m_Log.Add("Bonus: +" + to_string(scoreBonus) + " score, +" + to_string(coinBonus) + " coins.",
              LIGHT_YELLOW);
    if (m_Stage < GameRules::TOTAL_STAGES) {
        player.heal(player.getMaxHealth() / 2);
        m_Log.Add("You catch your breath and recover some health.", LIGHT_GREEN);
    }

    player.advanceStage();
    m_Context.gameData.SaveProgress();

    Continue([this]() {
        if (m_Context.gameData.player.getCurrentStage() > GameRules::TOTAL_STAGES) {
            m_Context.stateMachine.ReplaceAll(make_unique<GameOverState>(m_Context, true));
        } else {
            m_Context.gameData.SetNotice("Stage cleared! Progress saved.", LIGHT_GREEN);
            m_Context.stateMachine.PopState();
        }
    });
}

// ---------------------------------------------------------------------------
//  Update / Input / Draw
// ---------------------------------------------------------------------------

void BattleState::Update() {
    m_Log.Update();
    if (m_ZombieFlash > 0) --m_ZombieFlash;
    if (m_PlayerFlash > 0) --m_PlayerFlash;

    if (m_Phase == Phase::Messaging && !m_Log.IsBusy() && m_After) {
        function<void()> next = m_After;
        m_After = nullptr;
        next();
    }
}

void BattleState::ProcessInputs() {
    Keyboard& kb = m_Context.keyboard;

    switch (m_Phase) {
        case Phase::Messaging:
            if (kb.IsKeyJustPressed(KEY_ENTER) || kb.IsKeyJustPressed(KEY_SPACE)) m_Log.Skip();
            break;
        case Phase::ChooseAction:
            m_ActionSelector.ProcessInputs();
            break;
        case Phase::ChooseWeapon:
            if (kb.IsKeyJustPressed(KEY_ESCAPE) || kb.IsKeyJustPressed(KEY_BACKSPACE))
                EnterChooseAction();
            else
                m_WeaponSelector.ProcessInputs();
            break;
    }
}

void BattleState::DrawPlayerPanel() {
    Console& c = m_Context.console;
    Player& player = m_Context.gameData.player;
    int x = 4;

    c.Write(x, 5, player.getUsername(), m_PlayerFlash > 0 ? LIGHT_RED : LIGHT_AQUA);
    c.DrawBar(x, 7, 20, player.getHealth(), player.getMaxHealth());
    c.Write(x, 8, "HP " + to_string(player.getHealth()) + " / " + to_string(player.getMaxHealth()),
            WHITE);
    c.Write(x, 10, "Attack    " + to_string(player.getAttackPower()), WHITE);
    c.Write(x, 11, "Defense   " + to_string(player.getDefense()), WHITE);
    c.Write(x, 12, "Medkits   " + to_string(player.getInventory().getMedkits()), WHITE);
    c.Write(x, 14, "Coins     " + to_string(player.getCoins()), LIGHT_YELLOW);
    c.Write(x, 15, "Score     " + to_string(player.getScore()), LIGHT_YELLOW);
    if (m_Defending) c.Write(x, 17, "** DEFENDING **", LIGHT_AQUA);
}

void BattleState::DrawZombiePanel() {
    Console& c = m_Context.console;
    Zombie& zombie = CurrentZombie();
    int x = 63;

    bool isBoss = zombie.getType() == "Ballroom King";
    c.Write(x, 5, zombie.getType(), isBoss ? LIGHT_PURPLE : LIGHT_RED);
    c.DrawBar(x, 7, 20, zombie.getHealth(), zombie.getMaxHealth());
    c.Write(x, 8, "HP " + to_string(zombie.getHealth()) + " / " + to_string(zombie.getMaxHealth()),
            WHITE);
    c.Write(x, 10, "Attack    " + to_string(zombie.getAttackPower()), WHITE);
    c.Write(x, 12, "Reward", GRAY);
    c.Write(x, 13, "+" + to_string(zombie.getScoreValue()) + " score", LIGHT_YELLOW);
    c.Write(x, 14, "+" + to_string(zombie.getCoinReward()) + " coins", LIGHT_YELLOW);
}

void BattleState::DrawZombieArt() {
    Zombie& zombie = CurrentZombie();
    string type = zombie.getType();

    const vector<string>* art = &Art::WALKER;
    int color = GREEN;
    if (type == "Runner") {
        art = &Art::RUNNER;
        color = LIGHT_GREEN;
    } else if (type == "Brute") {
        art = &Art::BRUTE;
        color = PURPLE;
    } else if (type == "Ballroom King") {
        art = &Art::BALLROOM_KING;
        color = LIGHT_PURPLE;
    }

    if (!zombie.isAlive())
        color = GRAY;
    else if (m_ZombieFlash > 0)
        color = LIGHT_RED;

    m_Context.console.WriteLinesCentered(46, 5, *art, color);
}

void BattleState::Draw() {
    Console& c = m_Context.console;

    c.DrawHeader("STAGE " + to_string(m_Stage) + ": " + GameRules::StageName(m_Stage));
    int shown = min(m_ZombieIndex + 1, static_cast<int>(m_Zombies.size()));
    if (!m_Zombies.empty())
        c.WriteCentered(3, "Enemy " + to_string(shown) + " of " + to_string(m_Zombies.size()), GRAY);

    if (!m_Zombies.empty()) {
        c.DrawPanelVert(32, 4, 14, BLUE);
        c.DrawPanelVert(60, 4, 14, BLUE);
        DrawPlayerPanel();
        DrawZombiePanel();
        DrawZombieArt();
    }

    c.DrawPanelHorz(1, 18, Console::WIDTH - 2, BLUE);
    m_Log.Draw(c, 3, 19);
    c.DrawPanelHorz(1, 25, Console::WIDTH - 2, BLUE);

    switch (m_Phase) {
        case Phase::Messaging:
            c.WriteCentered(27, "[Enter] Skip text", GRAY);
            break;
        case Phase::ChooseAction:
            m_ActionSelector.Draw();
            c.WriteCentered(28, "[Arrows] Move    [Enter] Choose", GRAY);
            break;
        case Phase::ChooseWeapon:
            c.Write(3, 25, " Choose a weapon ", LIGHT_YELLOW);
            m_WeaponSelector.Draw();
            c.WriteCentered(28, "[Arrows] Move    [Enter] Strike    [Esc] Back", GRAY);
            break;
    }
}
