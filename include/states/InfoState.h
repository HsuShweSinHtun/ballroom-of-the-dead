// InfoState.h - a scrollable page of text (leaderboard, past reports, how to play)
#pragma once

#include <string>
#include <vector>
#include "states/IState.h"
#include "states/StateMachine.h"
using namespace std;

struct InfoLine {
    string text;
    int color;
};

class InfoState : public IState {
public:
    InfoState(StateContext& context, const string& title, const vector<InfoLine>& lines,
              bool startAtEnd = false);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

    static StatePtr CreateLeaderboard(StateContext& context);
    static StatePtr CreateReports(StateContext& context);
    static StatePtr CreateHowToPlay(StateContext& context);

private:
    StateContext& m_Context;
    string m_Title;
    vector<InfoLine> m_Lines;
    int m_Scroll;
    int m_VisibleRows;

    int MaxScroll() const;
};
