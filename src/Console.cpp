#include "ui/Console.h"
#include "utilities/Terminal.h"

#include <algorithm>

Console::Console()
    : m_Screen(WIDTH * HEIGHT, Cell{' ', WHITE}),
      m_Previous(WIDTH * HEIGHT, Cell{'\0', WHITE}),
      m_bForceRedraw(true),
      m_Columns(0),
      m_Rows(0) {}

void Console::ClearBuffer() {
    fill(m_Screen.begin(), m_Screen.end(), Cell{' ', WHITE});
}

void Console::Write(int x, int y, const string& text, int color) {
    if (y < 0 || y >= HEIGHT) return;
    for (size_t i = 0; i < text.size(); ++i) {
        int cx = x + static_cast<int>(i);
        if (cx < 0 || cx >= WIDTH) continue;
        char c = text[i];
        if (c < 32 || c > 126) c = '?';  // the buffer is plain ASCII
        m_Screen[y * WIDTH + cx] = Cell{c, color};
    }
}

void Console::WriteCentered(int y, const string& text, int color) {
    int x = (WIDTH - static_cast<int>(text.size())) / 2;
    Write(x, y, text, color);
}

void Console::WriteLines(int x, int y, const vector<string>& lines, int color) {
    for (size_t i = 0; i < lines.size(); ++i) Write(x, y + static_cast<int>(i), lines[i], color);
}

void Console::WriteLinesCentered(int centerX, int y, const vector<string>& lines, int color) {
    size_t widest = 0;
    for (const string& line : lines) widest = max(widest, line.size());
    WriteLines(centerX - static_cast<int>(widest) / 2, y, lines, color);
}

void Console::DrawPanelHorz(int x, int y, int length, int color, char character) {
    Write(x, y, string(static_cast<size_t>(max(0, length)), character), color);
}

void Console::DrawPanelVert(int x, int y, int height, int color, char character) {
    for (int i = 0; i < height; ++i) Write(x, y + i, string(1, character), color);
}

void Console::DrawPanel(int x, int y, int width, int height, int color) {
    DrawPanelHorz(x, y, width, color, '=');
    DrawPanelHorz(x, y + height - 1, width, color, '=');
    DrawPanelVert(x, y + 1, height - 2, color, '|');
    DrawPanelVert(x + width - 1, y + 1, height - 2, color, '|');
}

// Draws a bar like [#######---]. The color changes with the amount left.
void Console::DrawBar(int x, int y, int width, int value, int maxValue) {
    if (maxValue <= 0) maxValue = 1;
    value = max(0, min(value, maxValue));
    int filled = value * width / maxValue;
    if (value > 0 && filled == 0) filled = 1;

    int color = LIGHT_GREEN;
    if (value * 100 / maxValue <= 25)
        color = LIGHT_RED;
    else if (value * 100 / maxValue <= 50)
        color = LIGHT_YELLOW;

    Write(x, y, "[", WHITE);
    Write(x + 1, y, string(static_cast<size_t>(filled), '#'), color);
    Write(x + 1 + filled, y, string(static_cast<size_t>(width - filled), '-'), GRAY);
    Write(x + 1 + width, y, "]", WHITE);
}

void Console::DrawHeader(const string& title, int color) {
    WriteCentered(1, title, color);
    DrawPanelHorz(1, 2, WIDTH - 2, BLUE);
}

void Console::DrawBorder() {
    DrawPanelHorz(0, 0, WIDTH, BLUE, '=');
    DrawPanelHorz(0, HEIGHT - 1, WIDTH, BLUE, '=');
    DrawPanelVert(0, 1, HEIGHT - 2, BLUE, '|');
    DrawPanelVert(WIDTH - 1, 1, HEIGHT - 2, BLUE, '|');
}

string Console::AnsiColor(int color) {
    // Windows palette order -> ANSI color numbers
    static const int ansi[8] = {0, 4, 2, 6, 1, 5, 3, 7};
    int base = ansi[color & 7];
    int code = (color >= 8) ? 90 + base : 30 + base;
    return "\x1b[" + to_string(code) + "m";
}

bool Console::UpdateSize() {
    int columns = 0, rows = 0;
    Terminal::GetSize(columns, rows);
    if (columns != m_Columns || rows != m_Rows) {
        m_Columns = columns;
        m_Rows = rows;
        m_bForceRedraw = true;
        Terminal::Write("\x1b[0m\x1b[2J");
        if (columns < WIDTH || rows < HEIGHT) {
            string line1 = "Please make the terminal window bigger.";
            string line2 = "Needed: " + to_string(WIDTH) + " x " + to_string(HEIGHT) +
                           "   Now: " + to_string(columns) + " x " + to_string(rows);
            Terminal::Write("\x1b[1;1H" + line1 + "\x1b[2;1H" + line2);
        }
    }
    return m_Columns >= WIDTH && m_Rows >= HEIGHT;
}

void Console::Draw() {
    DrawBorder();

    if (!m_bForceRedraw && m_Screen == m_Previous) return;

    // Center the game inside bigger terminals
    int offsetX = max(0, (m_Columns - WIDTH) / 2);
    int offsetY = max(0, (m_Rows - HEIGHT) / 2);

    string out;
    out.reserve(WIDTH * HEIGHT * 2);
    int currentColor = -1;
    for (int y = 0; y < HEIGHT; ++y) {
        out += "\x1b[" + to_string(y + 1 + offsetY) + ";" + to_string(1 + offsetX) + "H";
        for (int x = 0; x < WIDTH; ++x) {
            const Cell& cell = m_Screen[y * WIDTH + x];
            if (cell.color != currentColor) {
                out += AnsiColor(cell.color);
                currentColor = cell.color;
            }
            out += cell.character;
        }
    }
    out += "\x1b[0m";
    Terminal::Write(out);

    m_Previous = m_Screen;
    m_bForceRedraw = false;
}
