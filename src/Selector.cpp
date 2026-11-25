#include "ui/Selector.h"
#include "inputs/Keys.h"

Selector::Selector(Console& console, Keyboard& keyboard, vector<string> items,
                   SelectorParams params)
    : m_Console(console),
      m_Keyboard(keyboard),
      m_Items(items),
      m_Params(params),
      m_bShowCursor(true),
      m_Index(0) {
    if (m_Params.columns < 1) m_Params.columns = 1;
}

void Selector::SetData(const vector<string>& items) {
    m_Items = items;
    if (m_Index >= static_cast<int>(m_Items.size())) m_Index = static_cast<int>(m_Items.size()) - 1;
    if (m_Index < 0) m_Index = 0;
}

void Selector::SetIndex(int index) {
    if (index >= 0 && index < static_cast<int>(m_Items.size())) m_Index = index;
}

void Selector::MoveVertical(int direction) {
    int count = static_cast<int>(m_Items.size());
    if (count == 0) return;
    int columns = m_Params.columns;
    int next = m_Index + direction * columns;

    if (next < 0) {  // wrap to the bottom of the same column
        next = m_Index;
        while (next + columns < count) next += columns;
    } else if (next >= count) {  // wrap to the top of the same column
        next = m_Index % columns;
    }
    m_Index = next;
}

void Selector::MoveHorizontal(int direction) {
    int count = static_cast<int>(m_Items.size());
    if (count == 0 || m_Params.columns <= 1) return;
    int next = m_Index + direction;
    if (next < 0 || next >= count) return;
    if (next / m_Params.columns != m_Index / m_Params.columns) return;  // stay in the same row
    m_Index = next;
}

void Selector::ProcessInputs() {
    if (!m_bShowCursor || m_Items.empty()) return;

    if (m_Keyboard.IsKeyJustPressed(KEY_UP) || m_Keyboard.IsKeyJustPressed(KEY_W))
        MoveVertical(-1);
    else if (m_Keyboard.IsKeyJustPressed(KEY_DOWN) || m_Keyboard.IsKeyJustPressed(KEY_S))
        MoveVertical(1);
    else if (m_Keyboard.IsKeyJustPressed(KEY_LEFT) || m_Keyboard.IsKeyJustPressed(KEY_A))
        MoveHorizontal(-1);
    else if (m_Keyboard.IsKeyJustPressed(KEY_RIGHT) || m_Keyboard.IsKeyJustPressed(KEY_D))
        MoveHorizontal(1);
    else if (m_Keyboard.IsKeyJustPressed(KEY_ENTER) || m_Keyboard.IsKeyJustPressed(KEY_SPACE)) {
        if (m_OnSelect) m_OnSelect(m_Index);
    }
}

void Selector::Draw() {
    for (int i = 0; i < static_cast<int>(m_Items.size()); ++i) {
        int column = i % m_Params.columns;
        int row = i / m_Params.columns;
        int x = m_Params.x + column * m_Params.spacingX;
        int y = m_Params.y + row * m_Params.spacingY;

        int color = m_ColorFunc ? m_ColorFunc(i) : WHITE;
        bool selected = (i == m_Index) && m_bShowCursor;

        if (selected) {
            m_Console.Write(x - 3, y, m_Params.cursor, LIGHT_RED);
            color = LIGHT_YELLOW;
        }
        m_Console.Write(x, y, m_Items[i], color);
    }
}
