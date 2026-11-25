// Utils.h - small helper functions used by several classes
#pragma once

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

namespace Utils {

// Split a string on a delimiter (keeps empty fields, including the last one)
inline vector<string> split(const string& s, char delim) {
    vector<string> parts;
    string current;
    for (char c : s) {
        if (c == delim) {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

inline string join(const vector<string>& items, const string& sep) {
    string result;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) result += sep;
        result += items[i];
    }
    return result;
}

inline int toInt(const string& s, int fallback = 0) {
    try {
        return stoi(s);
    } catch (...) {
        return fallback;
    }
}

inline string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Random integer in [lo, hi]
inline int randomInt(int lo, int hi) { return lo + rand() % (hi - lo + 1); }

inline string currentDateTime() {
    char buffer[32];
    time_t now = time(nullptr);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buffer);
}

inline string formatDuration(int seconds) {
    return to_string(seconds / 60) + " min " + to_string(seconds % 60) + " sec";
}

inline string padRight(string s, size_t width) {
    if (s.size() > width) return s.substr(0, width);
    s.append(width - s.size(), ' ');
    return s;
}

inline string padLeft(string s, size_t width) {
    if (s.size() > width) return s.substr(0, width);
    return string(width - s.size(), ' ') + s;
}

inline bool isValidUsername(const string& name) {
    if (name.size() < 3 || name.size() > 15) return false;
    for (char c : name)
        if (!isalnum(static_cast<unsigned char>(c)) && c != '_') return false;
    return true;
}

}  // namespace Utils
