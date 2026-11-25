// Typewriter.h - reveals a piece of text one letter at a time
#pragma once

#include <chrono>
#include <string>
using namespace std;

class Typewriter {
private:
    string m_Text;
    size_t m_Visible;
    double m_Accumulated;
    int m_CharsPerSecond;
    chrono::steady_clock::time_point m_Last;

public:
    explicit Typewriter(int charsPerSecond = 60);

    void Start(const string& text);
    void Update();   // call once per frame
    void Skip();     // show everything immediately
    bool IsFinished() const { return m_Visible >= m_Text.size(); }
    string GetVisibleText() const { return m_Text.substr(0, m_Visible); }
    const string& GetFullText() const { return m_Text; }
    void SetSpeed(int charsPerSecond) { m_CharsPerSecond = charsPerSecond; }
};
