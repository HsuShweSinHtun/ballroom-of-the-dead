// Terminal.h - the only place with platform-specific code (macOS / Linux / Windows)
#pragma once

#include <string>
using namespace std;

namespace Terminal {

bool Init();                          // raw keyboard mode, alternate screen, hidden cursor
void Shutdown();                      // restores the terminal (also called automatically on exit)
void GetSize(int& columns, int& rows);
void RequestSize(int columns, int rows);  // asks the terminal to resize itself (not all terminals obey)
void Write(const string& text);
string ReadInput();                   // non-blocking: returns the bytes typed since the last call

}  // namespace Terminal
