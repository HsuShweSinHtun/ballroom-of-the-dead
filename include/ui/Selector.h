// Selector.h - a menu with a "->" cursor. Move with arrow keys (or W/A/S/D),
// choose with Enter or Space.
#pragma once

#include <functional>
#include <string>
#include <vector>
#include "ui/Console.h"
#include "inputs/Keyboard.h"
using namespace std;

struct SelectorParams {
    int x = 20;
    int y = 10;
    int columns = 1;
    int spacingX = 20;
    int spacingY = 2;
    string cursor = "->";
};

class Selector {
public:
    Selector(Console& console, Keyboard& keyboard, vector<string> items,
             SelectorParams params = SelectorParams());

    void SetData(const vector<string>& items);
    vector<string>& GetData() { return m_Items; }
    void SetSelectionFunc(function<void(int)> onSelect) { m_OnSelect = onSelect; }
    void SetItemColorFunc(function<int(int)> colorFunc) { m_ColorFunc = colorFunc; }

    void ShowCursor() { m_bShowCursor = true; }
    void HideCursor() { m_bShowCursor = false; }
    int GetIndex() const { return m_Index; }
    void SetIndex(int index);

    void ProcessInputs();
    void Draw();

private:
    Console& m_Console;
    Keyboard& m_Keyboard;
    vector<string> m_Items;
    SelectorParams m_Params;
    function<void(int)> m_OnSelect;
    function<int(int)> m_ColorFunc;
    bool m_bShowCursor;
    int m_Index;

    void MoveVertical(int direction);
    void MoveHorizontal(int direction);
};
