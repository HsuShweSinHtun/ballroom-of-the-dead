// BattleState.h - one whole stage: a series of turn-based fights against zombies
#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "actor/Zombie.h"
#include "items/Weapon.h"
#include "states/IState.h"
#include "ui/Selector.h"
#include "utilities/MessageLog.h"
using namespace std;

class BattleState : public IState {
public:
    BattleState(StateContext& context, int stage);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    enum class Phase { Messaging, ChooseAction, ChooseWeapon };

    StateContext& m_Context;
    int m_Stage;
    vector<unique_ptr<Zombie>> m_Zombies;
    int m_ZombieIndex;
    int m_Turn;
    bool m_Defending;

    Phase m_Phase;
    function<void()> m_After;       // what happens when the messages finish typing
    MessageLog m_Log;
    Selector m_ActionSelector;
    Selector m_WeaponSelector;
    vector<int> m_WeaponIndices;    // inventory index of each weapon in the menu (-1 = bare hands)

    int m_ZombieFlash;
    int m_PlayerFlash;

    // Flow
    void BuildStage();
    void Continue(function<void()> next);
    void NextEncounter();
    void EnterChooseAction();
    void OnActionSelect(int index);
    void BeginWeaponChoice();
    void OnWeaponSelect(int index);

    // Combat
    void DoAttack(Weapon* weapon);
    void DoDefend();
    void DoMedkit();
    void DoRunAway();
    void ZombieDefeated();
    void ZombieTurn();
    void PlayerDied();
    void StageCleared();

    // Drawing
    Zombie& CurrentZombie();
    void DrawPlayerPanel();
    void DrawZombiePanel();
    void DrawZombieArt();
};
