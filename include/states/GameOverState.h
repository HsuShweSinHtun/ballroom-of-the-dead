// GameOverState.h - the end screen: result, report and leaderboard
#pragma once

#include <string>
#include <vector>
#include "states/IState.h"
#include "utilities/FileManager.h"
#include "utilities/MessageLog.h"
using namespace std;

class GameOverState : public IState {
public:
    GameOverState(StateContext& context, bool won);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    StateContext& m_Context;
    bool m_bWon;
    MessageLog m_Log;
    vector<LeaderboardEntry> m_Top;
    string m_PlayerName;
    int m_Frame;
};
