// Keyboard.h - reads the keys typed this frame
#pragma once

#include <set>
#include <string>
using namespace std;

class Keyboard {
private:
    set<int> m_JustPressed;
    string m_Typed;  // printable characters typed this frame (used for the username box)

public:
    void Poll();  // call once per frame, before the state processes its inputs
    bool IsKeyJustPressed(int key) const { return m_JustPressed.count(key) > 0; }
    bool AnyKeyPressed() const { return !m_JustPressed.empty(); }
    const string& GetTyped() const { return m_Typed; }
};
