#include "states/StateMachine.h"

void StateMachine::PushState(StatePtr state) {
    m_Pending.push_back(Operation{OpType::Push, move(state)});
}

void StateMachine::PopState() { m_Pending.push_back(Operation{OpType::Pop, nullptr}); }

void StateMachine::ChangeState(StatePtr state) {
    m_Pending.push_back(Operation{OpType::Change, move(state)});
}

void StateMachine::ReplaceAll(StatePtr state) {
    m_Pending.push_back(Operation{OpType::ReplaceAll, move(state)});
}

void StateMachine::RemoveTop() {
    if (m_States.empty()) return;
    m_States.top()->OnExit();
    m_States.pop();
}

void StateMachine::ApplyPending() {
    vector<Operation> operations;
    operations.swap(m_Pending);

    for (Operation& op : operations) {
        switch (op.type) {
            case OpType::Push:
                m_States.push(move(op.state));
                m_States.top()->OnEnter();
                break;
            case OpType::Pop:
                RemoveTop();
                if (!m_States.empty()) m_States.top()->OnResume();
                break;
            case OpType::Change:
                RemoveTop();
                m_States.push(move(op.state));
                m_States.top()->OnEnter();
                break;
            case OpType::ReplaceAll:
                while (!m_States.empty()) RemoveTop();
                m_States.push(move(op.state));
                m_States.top()->OnEnter();
                break;
        }
    }
}
