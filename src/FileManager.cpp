#include "utilities/FileManager.h"
#include "utilities/Utils.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

FileManager::FileManager(const string& directory) : dataDir(directory) {}

void FileManager::ensureFiles() {
    try {
        filesystem::create_directories(dataDir);
    } catch (...) {
    }

    // Create any missing file so later reads never fail
    const char* names[] = {"players.txt", "game_history.txt", "leaderboard.txt", "reports.txt"};
    for (const char* name : names) {
        ifstream check(path(name));
        if (!check) ofstream create(path(name));
    }

    ifstream weaponsCheck(path("weapons.txt"));
    if (!weaponsCheck) {
        ofstream out(path("weapons.txt"));
        out << "Pistol|8|40|0\n"
            << "Baseball Bat|10|25|30\n"
            << "Shotgun|18|15|90\n"
            << "Katana|15|30|120\n"
            << "Chainsaw|25|12|200\n";
    }
}

// ---------- players.txt ----------

bool FileManager::playerExists(const string& username) const {
    Player temp;
    return loadPlayer(username, temp);
}

bool FileManager::loadPlayer(const string& username, Player& out) const {
    ifstream in(path("players.txt"));
    if (!in) return false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        string name = line.substr(0, line.find('|'));
        if (Utils::toLower(name) == Utils::toLower(username))
            return Player::fromFileString(line, out);
    }
    return false;
}

// Rewrites the file through a temporary file, replacing the player's line
// (or adding a new line if the player is not in the file yet).
void FileManager::savePlayer(const Player& player) {
    string original = path("players.txt");
    string temp = path("players.tmp");

    ifstream in(original);
    ofstream out(temp);
    if (!out) {
        return;
    }

    bool replaced = false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        string name = line.substr(0, line.find('|'));
        if (Utils::toLower(name) == Utils::toLower(player.getUsername())) {
            out << player.toFileString() << "\n";
            replaced = true;
        } else {
            out << line << "\n";
        }
    }
    if (!replaced) out << player.toFileString() << "\n";

    in.close();
    out.close();
    remove(original.c_str());
    rename(temp.c_str(), original.c_str());
}

// ---------- weapons.txt ----------

vector<Weapon> FileManager::loadWeapons() const {
    vector<Weapon> weapons;
    ifstream in(path("weapons.txt"));
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> f = Utils::split(line, '|');
        if (f.size() < 4) continue;
        weapons.push_back(Weapon(f[0], Utils::toInt(f[1]), Utils::toInt(f[2]), Utils::toInt(f[3])));
    }
    return weapons;
}

// ---------- history and reports (append) ----------

void FileManager::appendHistory(const string& line) {
    ofstream out(path("game_history.txt"), ios::app);
    out << line << "\n";
}

void FileManager::appendReport(const string& text) {
    ofstream out(path("reports.txt"), ios::app);
    out << text << "\n";
}

string FileManager::readReports() const {
    ifstream in(path("reports.txt"));
    stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

// ---------- leaderboard.txt ----------

vector<LeaderboardEntry> FileManager::loadLeaderboard() const {
    vector<LeaderboardEntry> entries;
    ifstream in(path("leaderboard.txt"));
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> f = Utils::split(line, '|');
        if (f.size() < 2) continue;
        entries.push_back({f[0], Utils::toInt(f[1])});
    }
    return entries;
}

void FileManager::saveLeaderboard(const vector<LeaderboardEntry>& entries) {
    ofstream out(path("leaderboard.txt"));
    for (const LeaderboardEntry& e : entries) out << e.username << "|" << e.score << "\n";
}
