// StateMachine.h - a stack of states. The top state is the one on screen.
// Changes are queued and applied at the end of the frame so a state can safely
// remove itself from inside its own ProcessInputs().
#pragma once

#include <memory>
#include <stack>
#include <vector>
#include "states/IState.h"
using namespace std;

using StatePtr = unique_ptr<IState>;

class StateMachine {
public:
    void PushState(StatePtr state);     // put a new state on top
    void PopState();                    // remove the top state (the one below resumes)
    void ChangeState(StatePtr state);   // replace the top state
    void ReplaceAll(StatePtr state);    // clear the whole stack, then push
    void ApplyPending();                // called by Game once per frame

    bool Empty() const { return m_States.empty(); }
    bool HasPending() const { return !m_Pending.empty(); }
    IState* Current() { return m_States.empty() ? nullptr : m_States.top().get(); }

private:
    enum class OpType { Push, Pop, Change, ReplaceAll };
    struct Operation {
        OpType type;
        StatePtr state;
    };

    stack<StatePtr> m_States;
    vector<Operation> m_Pending;

    void RemoveTop();
};
