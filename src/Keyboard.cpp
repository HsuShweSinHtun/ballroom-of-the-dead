#include "inputs/Keyboard.h"
#include "inputs/Keys.h"
#include "utilities/Terminal.h"

#include <cctype>

void Keyboard::Poll() {
    m_JustPressed.clear();
    m_Typed.clear();

    string input = Terminal::ReadInput();
    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(input[i]);

        if (c == 0x1b) {  // escape: either the Esc key or the start of an arrow-key sequence
            if (i + 1 < input.size() && (input[i + 1] == '[' || input[i + 1] == 'O')) {
                size_t j = i + 2;
                while (j < input.size() && (input[j] < 0x40 || input[j] > 0x7e)) ++j;
                if (j < input.size()) {
                    switch (input[j]) {
                        case 'A': m_JustPressed.insert(KEY_UP); break;
                        case 'B': m_JustPressed.insert(KEY_DOWN); break;
                        case 'C': m_JustPressed.insert(KEY_RIGHT); break;
                        case 'D': m_JustPressed.insert(KEY_LEFT); break;
                        default: break;
                    }
                }
                i = j;
            } else {
                m_JustPressed.insert(KEY_ESCAPE);
            }
        } else if (c == '\r' || c == '\n') {
            m_JustPressed.insert(KEY_ENTER);
        } else if (c == 0x7f || c == 0x08) {
            m_JustPressed.insert(KEY_BACKSPACE);
        } else if (c == ' ') {
            m_JustPressed.insert(KEY_SPACE);
            m_Typed += ' ';
        } else if (isalpha(c)) {
            m_JustPressed.insert(tolower(c));
            m_Typed += static_cast<char>(c);
        } else if (c > 32 && c < 127) {
            m_JustPressed.insert(c);
            m_Typed += static_cast<char>(c);
        }
    }
}
