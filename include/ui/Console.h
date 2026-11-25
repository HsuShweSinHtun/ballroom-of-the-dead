// Console.h - a screen buffer. States write text into the buffer, then Draw() shows it.
#pragma once

#include <string>
#include <vector>
#include "ui/Colors.h"
using namespace std;

class Console {
public:
    static constexpr int WIDTH = 90;
    static constexpr int HEIGHT = 30;

    Console();

    int GetScreenWidth() const { return WIDTH; }
    int GetScreenHeight() const { return HEIGHT; }
    int GetHalfWidth() const { return WIDTH / 2; }
    int GetHalfHeight() const { return HEIGHT / 2; }

    void ClearBuffer();
    void Write(int x, int y, const string& text, int color = WHITE);
    void WriteCentered(int y, const string& text, int color = WHITE);
    void WriteLines(int x, int y, const vector<string>& lines, int color = WHITE);
    void WriteLinesCentered(int centerX, int y, const vector<string>& lines, int color = WHITE);

    void DrawPanelHorz(int x, int y, int length, int color = BLUE, char character = '=');
    void DrawPanelVert(int x, int y, int height, int color = BLUE, char character = '|');
    void DrawPanel(int x, int y, int width, int height, int color = BLUE);
    void DrawBar(int x, int y, int width, int value, int maxValue);
    void DrawHeader(const string& title, int color = LIGHT_YELLOW);

    // Checks the terminal size. Returns false (and shows a message) if it is too small.
    bool UpdateSize();
    // Draws the border and sends the buffer to the terminal (only if something changed).
    void Draw();

private:
    struct Cell {
        char character;
        int color;
        bool operator==(const Cell& other) const {
            return character == other.character && color == other.color;
        }
    };

    vector<Cell> m_Screen;
    vector<Cell> m_Previous;
    bool m_bForceRedraw;
    int m_Columns;
    int m_Rows;

    void DrawBorder();
    static string AnsiColor(int color);
};
