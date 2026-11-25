// MessageLog.h - a scrolling text box. New messages are typed out letter by letter.
#pragma once

#include <deque>
#include <string>
#include "ui/Console.h"
#include "utilities/Typewriter.h"
using namespace std;

struct LogLine {
    string text;
    int color;
};

class MessageLog {
public:
    MessageLog(int maxLines = 6, int width = 80, int charsPerSecond = 70);

    void Add(const string& text, int color = WHITE);  // queue a message (long text is wrapped)
    void Update();
    void Skip();                 // show all queued messages immediately
    bool IsBusy() const { return m_bTyping || !m_Pending.empty(); }
    void Clear();
    void Draw(Console& console, int x, int y) const;

private:
    int m_MaxLines;
    int m_Width;
    deque<LogLine> m_History;
    deque<LogLine> m_Pending;
    Typewriter m_Typewriter;
    LogLine m_Active;
    bool m_bTyping;

    void PushHistory(const LogLine& line);
};
