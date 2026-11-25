// NOTE: the platform headers must come BEFORE our headers. Terminal.h contains
// "using namespace std;", which clashes with the Windows "byte" type if it comes first.
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <conio.h>
#else
#include <csignal>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>

#include "utilities/Terminal.h"

using namespace std;

namespace {

bool g_active = false;

#ifdef _WIN32
HANDLE g_out = INVALID_HANDLE_VALUE;
DWORD g_oldMode = 0;
#else
termios g_oldTermios;

void OnSignal(int sig) {
    Terminal::Shutdown();
    signal(sig, SIG_DFL);
    raise(sig);
}
#endif

}  // namespace

namespace Terminal {

void Write(const string& text) {
    fwrite(text.data(), 1, text.size(), stdout);
    fflush(stdout);
}

#ifdef _WIN32

bool Init() {
    if (g_active) return true;
    g_out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (g_out == INVALID_HANDLE_VALUE || !GetConsoleMode(g_out, &mode)) return false;
    g_oldMode = mode;
    if (!SetConsoleMode(g_out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return false;
    g_active = true;
    atexit(Shutdown);
    Write("\x1b[?1049h\x1b[?25l\x1b[2J");
    return true;
}

void Shutdown() {
    if (!g_active) return;
    g_active = false;
    Write("\x1b[0m\x1b[?25h\x1b[?1049l");
    SetConsoleMode(g_out, g_oldMode);
}

void GetSize(int& columns, int& rows) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(g_out, &info)) {
        columns = info.srWindow.Right - info.srWindow.Left + 1;
        rows = info.srWindow.Bottom - info.srWindow.Top + 1;
    } else {
        columns = 120;
        rows = 30;
    }
}

string ReadInput() {
    string input;
    while (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 0xE0) {  // arrow keys arrive as two codes
            int d = _getch();
            switch (d) {
                case 72: input += "\x1b[A"; break;
                case 80: input += "\x1b[B"; break;
                case 77: input += "\x1b[C"; break;
                case 75: input += "\x1b[D"; break;
                default: break;
            }
        } else {
            input += static_cast<char>(c);
        }
    }
    return input;
}

#else  // macOS / Linux

bool Init() {
    if (g_active) return true;
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) return false;
    if (tcgetattr(STDIN_FILENO, &g_oldTermios) != 0) return false;

    termios raw = g_oldTermios;
    raw.c_lflag &= ~(ICANON | ECHO);  // no line buffering, no echo
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;               // read() returns immediately
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0) return false;

    g_active = true;
    atexit(Shutdown);
    signal(SIGINT, OnSignal);
    signal(SIGTERM, OnSignal);
    signal(SIGHUP, OnSignal);
    Write("\x1b[?1049h\x1b[?25l\x1b[2J");  // alternate screen, hide cursor, clear
    return true;
}

void Shutdown() {
    if (!g_active) return;
    g_active = false;
    const char reset[] = "\x1b[0m\x1b[?25h\x1b[?1049l";
    ssize_t ignored = write(STDOUT_FILENO, reset, sizeof(reset) - 1);
    (void)ignored;
    tcsetattr(STDIN_FILENO, TCSANOW, &g_oldTermios);
}

void GetSize(int& columns, int& rows) {
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 && ws.ws_row > 0) {
        columns = ws.ws_col;
        rows = ws.ws_row;
    } else {
        columns = 80;
        rows = 24;
    }
}

string ReadInput() {
    string input;
    char buffer[256];
    while (true) {
        ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (n <= 0) break;
        input.append(buffer, static_cast<size_t>(n));
        if (n < static_cast<ssize_t>(sizeof(buffer))) break;
    }
    return input;
}

#endif

void RequestSize(int columns, int rows) {
    Write("\x1b[8;" + to_string(rows) + ";" + to_string(columns) + "t");
}

}  // namespace Terminal
