#include "utilities/MessageLog.h"

#include <sstream>

MessageLog::MessageLog(int maxLines, int width, int charsPerSecond)
    : m_MaxLines(maxLines),
      m_Width(width),
      m_Typewriter(charsPerSecond),
      m_Active{"", WHITE},
      m_bTyping(false) {}

void MessageLog::Add(const string& text, int color) {
    // Word wrap
    istringstream words(text);
    string word, line;
    while (words >> word) {
        if (!line.empty() && static_cast<int>(line.size() + 1 + word.size()) > m_Width) {
            m_Pending.push_back({line, color});
            line.clear();
        }
        line += (line.empty() ? "" : " ") + word;
    }
    m_Pending.push_back({line, color});  // an empty text becomes a blank line
}

void MessageLog::PushHistory(const LogLine& line) {
    m_History.push_back(line);
    while (static_cast<int>(m_History.size()) > m_MaxLines) m_History.pop_front();
}

void MessageLog::Update() {
    if (m_bTyping) {
        m_Typewriter.Update();
        if (m_Typewriter.IsFinished()) {
            PushHistory(m_Active);
            m_bTyping = false;
        }
    }
    if (!m_bTyping && !m_Pending.empty()) {
        m_Active = m_Pending.front();
        m_Pending.pop_front();
        m_Typewriter.Start(m_Active.text);
        m_bTyping = true;
    }
}

void MessageLog::Skip() {
    if (m_bTyping) {
        PushHistory(m_Active);
        m_bTyping = false;
    }
    while (!m_Pending.empty()) {
        PushHistory(m_Pending.front());
        m_Pending.pop_front();
    }
}

void MessageLog::Clear() {
    m_History.clear();
    m_Pending.clear();
    m_bTyping = false;
}

void MessageLog::Draw(Console& console, int x, int y) const {
    int slots = m_MaxLines - (m_bTyping ? 1 : 0);
    int first = max(0, static_cast<int>(m_History.size()) - slots);
    int row = 0;
    for (int i = first; i < static_cast<int>(m_History.size()); ++i, ++row)
        console.Write(x, y + row, m_History[i].text, m_History[i].color);
    if (m_bTyping) console.Write(x, y + row, m_Typewriter.GetVisibleText(), m_Active.color);
}
