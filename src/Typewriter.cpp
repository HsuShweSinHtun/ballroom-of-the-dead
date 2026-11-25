#include "utilities/Typewriter.h"

#include <algorithm>

Typewriter::Typewriter(int charsPerSecond)
    : m_Text(""),
      m_Visible(0),
      m_Accumulated(0.0),
      m_CharsPerSecond(charsPerSecond),
      m_Last(chrono::steady_clock::now()) {}

void Typewriter::Start(const string& text) {
    m_Text = text;
    m_Visible = 0;
    m_Accumulated = 0.0;
    m_Last = chrono::steady_clock::now();
}

void Typewriter::Update() {
    auto now = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(now - m_Last).count();
    m_Last = now;
    if (seconds > 0.1) seconds = 0.1;  // avoid a big jump after a pause

    m_Accumulated += seconds * m_CharsPerSecond;
    size_t whole = static_cast<size_t>(m_Accumulated);
    m_Accumulated -= static_cast<double>(whole);
    m_Visible = min(m_Text.size(), m_Visible + whole);
}

void Typewriter::Skip() { m_Visible = m_Text.size(); }
